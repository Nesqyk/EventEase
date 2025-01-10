
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#include "events.h"
#include "users.h"
#include "utils.h"
#include "organizer.h"

#define MAX_ID_RANGE 1000
#define MAX_PATH 256

int book_event(int client_id, BookEvent event) 
{
    char user_path[256];   
    // ../data/users/id
    sprintf(user_path, "%s%d/events/%d.txt", USERS_DIR, client_id, event.id);


    FILE *e_file = fopen(user_path, "w");

    if(e_file == 0)
    {
        return 0;
    }

    time_t g_t;
    time(&g_t);

    char *generated_date = ctime(&g_t);

    fprintf(e_file, "id:%d\n", event.id);
    fprintf(e_file, "package_id:%d\n", event.package_id);
    fprintf(e_file, "event_type_id:%d\n", event.event_type_id);
    fprintf(e_file, "venue:%s\n", event.venue);
    fprintf(e_file, "name:%s\n", event.name);
    fprintf(e_file, "event_date:%s\n", event.event_date);
    fprintf(e_file, "end_time:%s\n", event.end_time);
    fprintf(e_file, "start_time:%s\n", event.start_time);
    fprintf(e_file, "balance:%d\n",event.balance);
    fprintf(e_file, "status:%d\n", event.status);
    fprintf(e_file ,"payment_deadline:%s\n", event.payment_deadline);
    fprintf(e_file, "booking_date:%s\n", generated_date);

    fclose(e_file);

    char dir_name[256];   
    // ../data/users/id
    sprintf(dir_name, "%s%d/book_event_id.txt", USERS_DIR, client_id);

    FILE *event_file = fopen(dir_name, "a");

    fprintf(event_file, "%d\n", event.id);
    fclose(event_file);

}

char *preview_single_event(int client_id, int event_id) {
    char path[100];
    snprintf(path, sizeof(path), "data/users/%d/events/%d.txt", client_id, event_id);

    FILE *info_file = fopen(path, "r");
    if (info_file == NULL) {
        perror("[ERROR] Unable to open event details file");
        return strdup(
            "==================================================\n"
            "              📭 Event Not Found                \n"
            "==================================================\n\n"
            "      The specified event could not be found.\n      Please check the event ID and try again.\n"
            "==================================================\n");
    }

    char event_name[100] = "Unknown Event";
    char venue[100] = "Unknown Venue";
    char event_date[50] = "Unknown Date";
    char start_time[20] = "Unknown Time";
    char package_name[100] = "Unknown Package";
    char balance[20] = "Unknown";
    char payment_deadline[50] = "Unknown";
    char booking_date[50] = "Unknown";
    char status[50] = "Unknown Status";
    int package_id = -1, event_type_id = -1;

    char key_buffer[50];
    char value_buffer[1000];

    while (fscanf(info_file, "%49[^:]:%999[^\n]\n", key_buffer, value_buffer) == 2) {
        if (strcmp(key_buffer, "name") == 0)
            strncpy(event_name, value_buffer, sizeof(event_name));
        else if (strcmp(key_buffer, "venue") == 0)
            strncpy(venue, value_buffer, sizeof(venue));
        else if (strcmp(key_buffer, "event_date") == 0)
            strncpy(event_date, value_buffer, sizeof(event_date));
        else if (strcmp(key_buffer, "start_time") == 0)
            strncpy(start_time, value_buffer, sizeof(start_time));
        else if (strcmp(key_buffer, "package_id") == 0)
            package_id = atoi(value_buffer);
        else if (strcmp(key_buffer, "event_type_id") == 0)
            event_type_id = atoi(value_buffer);
        else if (strcmp(key_buffer, "balance") == 0)
            strncpy(balance, value_buffer, sizeof(balance));
        else if (strcmp(key_buffer, "payment_deadline") == 0)
            strncpy(payment_deadline, value_buffer, sizeof(payment_deadline));
        else if (strcmp(key_buffer, "booking_date") == 0)
            strncpy(booking_date, value_buffer, sizeof(booking_date));
        else if (strcmp(key_buffer, "status") == 0) {
            int int_status = atoi(value_buffer);
            switch (int_status) {
                    case 0: strncpy(status, "Pending ⏳", sizeof(status)); break;
                    case 1: strncpy(status, "Confirmed ✅", sizeof(status)); break;
                    case 2: strncpy(status, "In Progress ⚙️", sizeof(status)); break;
                    case 3: strncpy(status, "Incomplete Payment 💳❌", sizeof(status)); break;
                    case 4: strncpy(status, "Ongoing 🚀", sizeof(status)); break;
                    case 5: strncpy(status, "Done 🎉", sizeof(status)); break;
                    default: strncpy(status, "Unknown Status ❓", sizeof(status)); break;
            }
        }
    }
    fclose(info_file);

    char *result = malloc(1024); // Adjust size as needed
    if (!result) {
        perror("[ERROR] Memory allocation failed");
        return strdup("An error occurred while previewing the event.");
    }

    snprintf(result, 1024,
             "==================================================\n"
             "               🎉 Event Details                  \n"
             "==================================================\n"
             "\n🎉 Event Name    : %s\n"
             "   📍 Venue        : %s\n"
             "   📅 Date         : %s | 🕒 Time: %s\n"
             "   💰 Balance      : PHP %s\n"
             "   📦 Package ID   : %d\n"
             "   📋 Type ID      : %d\n"
             "   🛠 Status        : %s\n"
             "   📆 Payment Deadline   : %s\n"
             "   🕒 Booked On  : %s\n\n"
             "==================================================\n",
             event_name, venue, event_date, start_time,
             balance, package_id, event_type_id, status,
             payment_deadline, booking_date);

    return result;
}


char *prev_events(int client_id)
{
    char path[100];
    snprintf(path, sizeof(path), "data/users/%d/book_event_id.txt", client_id);

    FILE *ids_file = fopen(path, "r");
    if (ids_file == NULL)
    {
        perror("[ERROR] Unable to open book_event_id.txt");
        return strdup(
            "==================================================\n"
            "               📭 No Upcoming Events Found       \n"
            "==================================================\n\n"
            "      You currently have no upcoming events.\n        Start planning your first event now!\n"
            "\n==================================================\n");
    }

    // Allocate memory for concatenated results
    char *result = malloc(8192); // Adjust size based on expected data
    if (!result)
    {
        perror("[ERROR] Memory allocation failed");
        fclose(ids_file);
        return strdup("An error occurred while loading your upcoming events.");
    }

    snprintf(result, 8192, 
        "==================================================\n"
        "               📅 Upcoming Events                \n"
        "==================================================\n");

    char line[256];
    int events_found = 0;

    while (fgets(line, sizeof(line), ids_file))
    {
        int id = atoi(line);

        char info_filename[256];
        snprintf(info_filename, sizeof(info_filename), "data/users/%d/events/%d.txt", client_id, id);

        FILE *info_file = fopen(info_filename, "r");
        if (info_file == NULL)
        {
            perror("[ERROR] Unable to open event details file");
            continue;
        }

        char key_buffer[50];
        char value_buffer[1000];

        char event_name[100] = "Unknown Event";
        char status[50] = "Unknown Status";
        char event_date[50] = "Unknown Date";
        char venue[100] = "Unknown Venue";
        char start_time[10], end_time[10];

        while (fscanf(info_file, "%49[^:]:%999[^\n]\n", key_buffer, value_buffer) == 2)
        {
            if (strcmp(key_buffer, "name") == 0)
                strncpy(event_name, value_buffer, sizeof(event_name));
            else if (strcmp(key_buffer, "status") == 0)
            {
                int int_status = atoi(value_buffer);
                size_t status_size = sizeof(status);
                switch (int_status)
                {
                    case 0: strncpy(status, "Pending ⏳", status_size); break;
                    case 1: strncpy(status, "Confirmed ✅", status_size); break;
                    case 2: strncpy(status, "In Progress ⚙️", status_size); break;
                    case 3: strncpy(status, "Incomplete Payment 💳❌", status_size); break;
                    case 4: strncpy(status, "Ongoing 🚀", status_size); break;
                    case 5: strncpy(status, "Done 🎉", status_size); break;
                    default: strncpy(status, "Unknown Status ❓", status_size); break;
                }
            }
            else if (strcmp(key_buffer, "event_date") == 0)
                strncpy(event_date, value_buffer, sizeof(event_date));
            else if (strcmp(key_buffer, "venue") == 0)
                strncpy(venue, value_buffer, sizeof(venue));
            else if (strcmp(key_buffer, "start_time") == 0)
                strncpy(start_time, value_buffer, sizeof(start_time));
            else if (strcmp(key_buffer, "end_time") == 0)
                strncpy(end_time, value_buffer, sizeof(end_time));
        }
        fclose(info_file);

        char formatted_event[512];
        snprintf(formatted_event, sizeof(formatted_event),
                 "\n📌 %s\n"
                    "   📅 Date       : %s\n"
                    "   ⏳ Duration   : %s - %s\n"
                    "   📍 Venue      : %s\n"
                    "   🆔 Book ID    : %d\n"
                    "   🔖 Status     : %s\n",
                 event_name, event_date, start_time, end_time, venue, id, status);
        // strcat(formatted_event,  "\n==================================================");
        strcat(result, formatted_event);
        events_found++;
    }
    fclose(ids_file);

    if (events_found == 0)
    {
        free(result);
        return strdup(
            "==================================================\n"
            "            📭 No Upcoming Events Found       \n"
            "==================================================\n\n"
            "      You currently have no upcoming events.\n        Start planning your first event now!\n"
            );
    }

    return result;
}

char *preview_all_events(int client_id) {
    char path[100];
    snprintf(path, sizeof(path), "data/users/%d/book_event_id.txt", client_id);

    FILE *ids_file = fopen(path, "r");
    if (ids_file == NULL) {
        perror("[ERROR] Unable to open book_event_id.txt");
        return strdup(
            "==================================================\n"
            "              📭 No Events Found                \n"
            "==================================================\n\n"
            "      There are currently no events recorded.\n      Create an event to get started!\n\n"
            "\n==================================================\n");
    }

    // Allocate memory for concatenated results
    char *result = malloc(16384); // Adjust size based on expected data
    if (!result) {
        perror("[ERROR] Memory allocation failed");
        fclose(ids_file);
        return strdup("An error occurred while loading events.");
    }

    snprintf(result, 16384,
             "==================================================\n"
             "               📅 All Managed Events             \n"
             "==================================================\n");

    char line[256];
    int events_found = 0;

    while (fgets(line, sizeof(line), ids_file)) {
        int id = atoi(line);

        char info_filename[256];
        snprintf(info_filename, sizeof(info_filename), "data/users/%d/events/%d.txt", client_id, id);

        FILE *info_file = fopen(info_filename, "r");
        if (info_file == NULL) {
            perror("[ERROR] Unable to open event details file");
            continue;
        }

        char key_buffer[50];
        char value_buffer[1000];

        char event_name[100] = "Unknown Event";
        char venue[100] = "Unknown Venue";
        char event_date[50] = "Unknown Date";
        char start_time[20] = "Unknown Time";
        char package_name[100] = "Unknown Package";
        char balance[20] = "Unknown";
        char payment_deadline[50] = "Unknown";
        char booking_date[50] = "Unknown";
        char status[50] = "Unknown Status";
        int package_id = -1, event_type_id = -1;

        while (fscanf(info_file, "%49[^:]:%999[^\n]\n", key_buffer, value_buffer) == 2) {
            if (strcmp(key_buffer, "name") == 0)
                strncpy(event_name, value_buffer, sizeof(event_name));
            else if (strcmp(key_buffer, "venue") == 0)
                strncpy(venue, value_buffer, sizeof(venue));
            else if (strcmp(key_buffer, "event_date") == 0)
                strncpy(event_date, value_buffer, sizeof(event_date));
            else if (strcmp(key_buffer, "start_time") == 0)
                strncpy(start_time, value_buffer, sizeof(start_time));
            else if (strcmp(key_buffer, "package_id") == 0)
                package_id = atoi(value_buffer);
            else if (strcmp(key_buffer, "event_type_id") == 0)
                event_type_id = atoi(value_buffer);
            else if (strcmp(key_buffer, "balance") == 0)
                strncpy(balance, value_buffer, sizeof(balance));
            else if (strcmp(key_buffer, "payment_deadline") == 0)
                strncpy(payment_deadline, value_buffer, sizeof(payment_deadline));
            else if (strcmp(key_buffer, "booking_date") == 0)
                strncpy(booking_date, value_buffer, sizeof(booking_date));
            else if (strcmp(key_buffer, "status") == 0) {
                int int_status = atoi(value_buffer);
                switch (int_status) {
                    case 1: strncpy(status, "Confirmed ✅", sizeof(status)); break;
                    case 2: strncpy(status, "In Progress 🔄", sizeof(status)); break;
                    case 3: strncpy(status, "Incomplete Payment ❌", sizeof(status)); break;
                    case 4: strncpy(status, "Ongoing 🔵", sizeof(status)); break;
                    case 5: strncpy(status, "Completed ✔️", sizeof(status)); break;
                    default: strncpy(status, "Unknown Status", sizeof(status)); break;
                }
            }
        }
        fclose(info_file);

        char formatted_event[1024];
        snprintf(formatted_event, sizeof(formatted_event),
                 "\n🎉 Event Name   : %s\n"
                 "   📍 Venue      : %s\n"
                 "   📅 Date       : %s | 🕒 Time: %s\n"
                 "   💰 Balance    : PHP %s\n"
                 "   📦 Package ID : %d\n"
                 "   📋 Type ID    : %d\n"
                 "   🛠 Status     : %s\n"
                 "   📆 Deadline   : %s\n"
                 "   🕒 Booked On  : %s\n",
                 event_name, venue, event_date, start_time,
                 balance, package_id, event_type_id, status,
                 payment_deadline, booking_date);

        strcat(result, formatted_event);
        strcat(result, "\n--------------------------------------------------\n");
        events_found++;
    }
    fclose(ids_file);

    if (events_found == 0) {
        free(result);
        return strdup(
            "==================================================\n"
            "              📭 No Events Found                \n"
            "==================================================\n\n"
            "      There are currently no events recorded.\n      Create an event to get started!\n"
            "==================================================\n");
    }

    return result;
}


// get all client id if I want to preview all client.
char **preview_events(int client_id, int *event_count, int **book_ids)
{
    char path[100];
    snprintf(path, sizeof(path), "data/users/%d/book_event_id.txt", client_id);

    FILE *ids_file = fopen(path, "r");
    if (ids_file == NULL)
    {
        perror("[ERROR] Unable to open book_event_id.txt");
        *event_count = 0;
        *book_ids = NULL;
        return NULL;
    }

    char *events[101]; // Fixed-size array of pointers for events
    int temp_book_ids[101]; // Temporary array to store book IDs
    *event_count = 0;
    int id;

    while (fscanf(ids_file, "%d\n", &id) != EOF)
    {
        if (*event_count >= 100)
        {
            printf("[WARNING] Maximum number of events reached. Some events will not be loaded.\n");
            break;
        }

        char info_filename[256];
        snprintf(info_filename, sizeof(info_filename), "data/users/%d/events/%d.txt", client_id, id);

        FILE *info_file = fopen(info_filename, "r");
        if (info_file == NULL)
        {
            perror("[ERROR] Unable to open event details file");
            continue;
        }

        char key_buffer[50];
        char value_buffer[1000];

        char event_name[100] = "Unknown Event";
        char event_date[50] = "Unknown Date";
        char start_time[20] = "Unknown Time";
        char venue[100] = "Unknown Venue";
        char package_name[100] = "Unknown Package";
        char status[50] = "Unknown Status";

        while (fscanf(info_file, "%49[^:]:%999[^\n]\n", key_buffer, value_buffer) == 2)
        {
            if (strcmp(key_buffer, "name") == 0)
                strncpy(event_name, value_buffer, sizeof(event_name));
            else if (strcmp(key_buffer, "event_date") == 0)
                strncpy(event_date, value_buffer, sizeof(event_date));
            else if (strcmp(key_buffer, "id") == 0)
                temp_book_ids[*event_count] = atoi(value_buffer);
            else if (strcmp(key_buffer, "start_time") == 0)
                strncpy(start_time, value_buffer, sizeof(start_time));
            else if (strcmp(key_buffer, "venue") == 0)
                strncpy(venue, value_buffer, sizeof(venue));
            else if (strcmp(key_buffer, "package_id") == 0)
                snprintf(package_name, sizeof(package_name), "%s", read_pkg(atoi(value_buffer), atoi(value_buffer), "package_name"));
            else if (strcmp(key_buffer, "status") == 0)
            {
                int int_status = atoi(value_buffer);
                switch (int_status)
                {
                    case 1: strncpy(status, "\033[32mConfirmed\033[0m", sizeof(status)); break;
                    case 2: strncpy(status, "In Progress", sizeof(status)); break;
                    case 3: strncpy(status, "Incomplete Payment", sizeof(status)); break;
                    default: strncpy(status, "Unknown Status", sizeof(status)); break;
                }
            }
        }
        fclose(info_file);

        char formatted_event[512];
        snprintf(formatted_event, sizeof(formatted_event),
                 "\n🎉 Event Name  : %s\n"
                "🆔 Book ID     : %d\n"
                "📅 Date        : %s\n"
                "⏰ Time        : %s\n"
                "📍 Venue       : %s\n"
                "📦 Package     : %s\n"
                "🔖 Status      : %s\n\n"
                 "--------------------------------------------------",
                 event_name, temp_book_ids[*event_count], event_date, start_time, venue, package_name, status);

        events[*event_count] = strdup(formatted_event);
        if (events[*event_count] == NULL)
        {
            perror("[ERROR] Memory allocation failed");
            break;
        }
        (*event_count)++;
    }
    fclose(ids_file);

    if (*event_count == 0)
    {
        printf("==================================================\n");
        printf("               📭 No Booked Events Found         \n");
        printf("==================================================\n");
        printf("You currently have no booked events. Start planning your first event now!\n");
        printf("==================================================\n");
        *book_ids = NULL;
        return NULL;
    }

    // Allocate memory for book_ids
    *book_ids = malloc(sizeof(int) * (*event_count));
    if (*book_ids == NULL)
    {
        perror("[ERROR] Memory allocation failed for book IDs");
        for (int i = 0; i < *event_count; i++)
            free(events[i]);
        *event_count = 0;
        return NULL;
    }

    // Copy book IDs to the dynamically allocated array
    for (int i = 0; i < *event_count; i++)
        (*book_ids)[i] = temp_book_ids[i];

    // Allocate memory for event strings
    char **result = malloc(sizeof(char *) * (*event_count + 1)); // +1 for NULL terminator
    if (result == NULL)
    {
        perror("[ERROR] Memory allocation failed for events array");
        for (int i = 0; i < *event_count; i++)
            free(events[i]);
        free(*book_ids);
        *book_ids = NULL;
        *event_count = 0;
        return NULL;
    }

    for (int i = 0; i < *event_count; i++)
        result[i] = events[i];
    result[*event_count] = NULL; // Add NULL terminator

    return result;
}



int update_event(int client_id, int event_id, char *key, char *value)
{

    if (valid_user_id(client_id) != 1) 
    {
        perror("[ERROR] Invalid ID");
        return 0;
    }

    char info_filename[256];   
    // data/users/id/events/id.txt
    sprintf(info_filename, "%s%d/events/%d.txt", USERS_DIR, client_id, event_id);

    FILE *info_file = fopen(info_filename, "r");

    if (info_file == NULL) 
    {
        return 0;
    }

    char temp_filename[50];

    sprintf(temp_filename, "%s%d/events/%d_temp_file.txt", USERS_DIR, client_id, event_id);

    FILE *temp_file = fopen(temp_filename, "w");

    if(temp_file == NULL)
    {
        fclose(info_file);
        return 0;
    }

    char buffer_value[50];
    char key_buffer[50];

    while (fscanf(info_file, "%49[^:]:%49[^\n]\n", key_buffer, buffer_value) == 2) 
    {
        if (strcmp(key, key_buffer) == 0) 
        {   
            if(value != buffer_value)
            {
                fprintf(temp_file, "%s:%s\n", key, value); 
            }
        } else 
        {
            fprintf(temp_file, "%s:%s\n", key_buffer, buffer_value);
        }
    }

    fclose(info_file);
    fclose(temp_file);

    if (remove(info_filename) != 0 || rename(temp_filename, info_filename) != 0) 
    {
        printf("[ERROR] Failed to update %d's %s", event_id, key);
        return 0;
    }
    return 1;
}

char *read_event(int client_id, int event_id, char *key)
{
    if (valid_user_id(client_id) != 1)
     {
        return NULL;
    }

    char info_filename[256];   
    // data/users/id/events/id
    sprintf(info_filename, "%s%d/events/%d.txt", USERS_DIR, client_id, event_id);

    FILE *info_file = fopen(info_filename, "r");

    if (info_file == NULL) 
    {
        return NULL;
    }

    char buffer_value[50];
    char key_buffer[50];

    while (!feof(info_file)) {
        if (fscanf(info_file, "%49[^:]:%49[^\n]\n", key_buffer, buffer_value) == 2) 
        {
            if (strcmp(key, key_buffer) == 0) 
            {
                char *result = malloc(strlen(buffer_value) + 1);
                if (result) 
                {
                    strcpy(result, buffer_value);
                }
                fclose(info_file);
                return result;
            }
        }
    }
    fclose(info_file);
    return NULL;
}

int cancel_event(int client_id, int event_id) 
{
    if (valid_user_id(client_id) != 1) 
    {
        perror("[ERROR] Invalid client ID");
        return -1;
    }

    char dir_name[256];   
    sprintf(dir_name, "%s%d/book_event_id.txt", USERS_DIR, client_id);

    FILE *event_file = fopen(dir_name, "r");
    if (event_file == NULL) 
    {
        perror("[ERROR] Error opening book_event_id.txt for reading");
        return -1;
    }

    char temp_path[256];
    sprintf(temp_path, "%s%d/book_event_id_temp.txt", USERS_DIR, client_id);

    FILE *temp_file = fopen(temp_path, "w");
    if (temp_file == NULL) 
    {
        perror("[ERROR] Error opening temporary file for writing");
        fclose(event_file);
        return -1;
    }

    int buffer_id;
    int is_found = 0;

    while (fscanf(event_file, "%d\n", &buffer_id) == 1) 
    {
        if (buffer_id != event_id) 
        {
            fprintf(temp_file, "%d\n", buffer_id);
        } 
        else 
        {
            is_found = 1;  
        }
    }

    fclose(event_file);
    fclose(temp_file);

    if (!is_found) 
    {
        perror("[ERROR] Event ID not found in booked events");
        remove(temp_path); 
        return -1;
    }

    // Replace the original file with the updated temp file
    if (remove(dir_name) != 0 || rename(temp_path, dir_name) != 0) 
    {
        perror("[ERROR] Failed to update book_event_id.txt");
        return -1;
    }

    // // Path to the event directory
    // char booked_event_dir[256];
    // sprintf(booked_event_dir, "data/users/%d/events/%d/", client_id, event_id);

    // // Path to the cancelled events folder
    // char cancelled_event_dir[256];
    // sprintf(cancelled_event_dir, "data/events/cancelled_events/%d/", event_id);

    // _mkdir(cancelled_event_dir);
    // struct stat st = {0};

    // // Rename the event directory to move it
    // if (rename(booked_event_dir, cancelled_event_dir) != 0) 
    // {  
    //     perror("[ERROR] Failed to move event directory to cancelled_events folder");
    //     return -1;
    // }
    
    return 1;
}


int valid_event_id(int client_id, int event_id) 
{
     char dir_name[256];   
    // ../data/users/id
    sprintf(dir_name, "%s%d/book_event_id.txt", USERS_DIR, client_id);

    FILE *event_file = fopen(dir_name, "r");

    if (event_file == NULL) 
    {
        perror("Error opening user id file");
        return 0;
    }

    int buffer_id;

    while (fscanf(event_file, "%d\n", &buffer_id) == 1) 
    {
        if (buffer_id == event_id) 
        {
            fclose(event_file);
            return 1;
        }
    }
    fclose(event_file);
    return 0;
}


int generate_event_id() 
{
    static int is_seeded = 0;

    if (!is_seeded) 
    {
        srand(time(NULL));
        is_seeded = 1;
    }

    int event_id;

    do 
    {
        event_id = rand() % MAX_ID_RANGE;
    } while (valid_user_id(event_id) == 1);

    return event_id;
}


char* generate_statistics_summary() {
    // Estimate required buffer size
    size_t buffer_size = 1024; // Adjust as needed for your text size
    char *result = malloc(buffer_size);
    if (result == NULL) {
        perror("[ERROR] Memory allocation failed for statistics summary");
        return NULL;
    }

    int total_events = count_total_events();
    int active_events = count_active_events();
    double total_revenue = calculate_total_revenue();
    const char *top_venue = get_most_booked_event_type();  // Replace with actual logic to find the top venue


    FILE *feedback_file = fopen("data/feedback.txt", "r");
    if (feedback_file == NULL) {
        perror("[ERROR] Unable to open feedback file");
        return strdup("[ERROR] Failed to retrieve statistics.");
    }

    int total_ratings = 0;
    int rating_count = 0;

    char line[256];
    while (fgets(line, sizeof(line), feedback_file)) {
        int user_id, stars;
        char message[100], date[50];

        if (sscanf(line, "%d,%d,\"%99[^\"]\",\"%49[^\"]\"", &user_id, &stars, message, date) == 4) {
            total_ratings += stars;
            rating_count++;
        }
    }
    fclose(feedback_file);

    double avg_rating = (rating_count > 0) ? (double)total_ratings / rating_count : 0.0;


    // Format the statistics into the allocated buffer
    snprintf(result, buffer_size,
        "==================================================\n"
        "               📊 EventEase Statistics             \n"
        "==================================================\n\n"
        "📅 Total Events Managed      : %d\n"
        "🎉 Active Events             : %d\n"
        "%s\n"
        "💵 Total Revenue             : PHP %s\n"
        "✨ Average Event Rating      : %.1f ⭐",
        total_events, active_events, top_venue, format_number(total_revenue), avg_rating);

    return result; // Return the formatted statistics as a string
}


int count_total_events() {
    FILE *user_ids_file = fopen("data/users_id.txt", "r");
    if (user_ids_file == NULL) {
        perror("[ERROR] Unable to open users_id.txt");
        return 0;
    }

    int total_events = 0;
    char line[MAX_PATH];

    while (fgets(line, sizeof(line), user_ids_file)) {
        int user_id;
        if (sscanf(line, "%d:", &user_id) != 1) {
            continue;
        }

        char book_event_ids_path[MAX_PATH];
        snprintf(book_event_ids_path, sizeof(book_event_ids_path), "data/users/%d/book_event_id.txt", user_id);

        FILE *events_file = fopen(book_event_ids_path, "r");
        if (events_file == NULL) {
            continue;  // Skip users with no booked events
        }

        while (fgets(line, sizeof(line), events_file)) {
            total_events++;
        }

        fclose(events_file);
    }

    fclose(user_ids_file);
    return total_events;
}


int count_active_events() {
    FILE *user_ids_file = fopen("data/users_id.txt", "r");
    if (user_ids_file == NULL) {
        perror("[ERROR] Unable to open users_id.txt");
        return 0;
    }

    int active_events = 0;
    char line[MAX_PATH];

    while (fgets(line, sizeof(line), user_ids_file)) {
        int user_id;
        if (sscanf(line, "%d:", &user_id) != 1) {
            continue;
        }

        char book_event_ids_path[MAX_PATH];
        snprintf(book_event_ids_path, sizeof(book_event_ids_path), "data/users/%d/book_event_id.txt", user_id);

        FILE *events_file = fopen(book_event_ids_path, "r");
        if (events_file == NULL) {
            continue;  // Skip users with no booked events
        }

        while (fgets(line, sizeof(line), events_file)) {
            int event_id = atoi(line);

            char *status = read_event(user_id, event_id, "status");
            if (status != NULL) {
                int event_status = atoi(status);
                free(status);
                if (event_status == 1 || event_status == 2 || event_status == 3) {
                    active_events++;
                }
            }
        }

        fclose(events_file);
    }

    fclose(user_ids_file);
    return active_events;
}


double calculate_total_revenue() {
    FILE *user_ids_file = fopen("data/users_id.txt", "r");
    if (user_ids_file == NULL) {
        perror("[ERROR] Unable to open users_id.txt");
        return 0.0;
    }

    double total_revenue = 0.0;
    char line[MAX_PATH];

    while (fgets(line, sizeof(line), user_ids_file)) {
        int user_id;
        if (sscanf(line, "%d:", &user_id) != 1) {
            continue;
        }

        char book_event_ids_path[MAX_PATH];
        snprintf(book_event_ids_path, sizeof(book_event_ids_path), "data/users/%d/book_event_id.txt", user_id);

        FILE *events_file = fopen(book_event_ids_path, "r");
        if (events_file == NULL) {
            continue;  // Skip users with no booked events
        }

        while (fgets(line, sizeof(line), events_file)) {
            int event_id = atoi(line);

            char *balance = read_event(user_id, event_id, "balance");
            if (balance != NULL && atoi(balance) == 0) {
                char *event_type_id = read_event(user_id, event_id, "event_type_id");
                char *package_id = read_event(user_id, event_id, "package_id");

                if (event_type_id != NULL && package_id != NULL) {
                    total_revenue += atoi(read_pkg(atoi(event_type_id), atoi(package_id), "price"));
                }

                free(event_type_id);
                free(package_id);
            }

            free(balance);
        }

        fclose(events_file);
    }

    fclose(user_ids_file);
    return total_revenue;
}

char* read_event_field(int user_id, int event_id, const char *field) {
    char event_path[MAX_PATH];
    snprintf(event_path, sizeof(event_path), "data/users/%d/events/%d.txt", user_id, event_id);

    FILE *event_file = fopen(event_path, "r");
    if (event_file == NULL) {
        return NULL;
    }

    char key[MAX_PATH];
    char value[MAX_PATH];
    while (fscanf(event_file, "%[^:]:%[^\n]\n", key, value) == 2) {
        if (strcmp(key, field) == 0) {
            fclose(event_file);
            return strdup(value);
        }
    }

    fclose(event_file);
    return NULL;
}

int  read_package_price(int event_type_id, int package_id) {
    char package_path[MAX_PATH];
    snprintf(package_path, sizeof(package_path), "data/events/%d/packages/%d.txt", event_type_id, package_id);

    FILE *package_file = fopen(package_path, "r");
    if (package_file == NULL) {
        return 0.0;
    }

    char key[MAX_PATH];
    char value[MAX_PATH];
    while (fscanf(package_file, "%[^:]:%[^\n]\n", key, value) == 2) {
        if (strcmp(key, "price") == 0) {
            fclose(package_file);
            return atof(value);
        }
    }

    fclose(package_file);
    return 0;
}


char *get_most_booked_event_type() {
    FILE *event_types_file = fopen("data/type_event_ids.txt", "r");
    if (event_types_file == NULL) {
        perror("[ERROR] Unable to open type_event_ids.txt");
        return strdup("[ERROR] Unable to generate report.");
    }

    char event_names[100][50]; // Maximum 100 event types, each up to 50 characters
    int event_counts[100] = {0};
    int total_event_types = 0;

    int event_type_id;
    while (fscanf(event_types_file, "%d\n", &event_type_id) != EOF) {
        char *event_name = read_typevent(event_type_id, "event_name");
        if (event_name) {
            if (total_event_types < 100) {
                strncpy(event_names[total_event_types], event_name, sizeof(event_names[total_event_types]) - 1);
                event_names[total_event_types][sizeof(event_names[total_event_types]) - 1] = '\0'; // Ensure null-termination
                total_event_types++;
            } else {
                printf("[WARNING] Maximum event types reached. Some types will not be counted.\n");
            }
            free(event_name);
        }
    }
    fclose(event_types_file);

    FILE *user_ids_file = fopen("data/users_id.txt", "r");
    if (user_ids_file == NULL) {
        perror("[ERROR] Unable to open users_id.txt");
        return strdup("[ERROR] Unable to generate report.");
    }

    int user_id;
    while (fscanf(user_ids_file, "%d:%*s\n", &user_id) != EOF) {
        char user_events_file[256];
        sprintf(user_events_file, "data/users/%d/book_event_id.txt", user_id);

        FILE *events_file = fopen(user_events_file, "r");
        if (events_file == NULL) continue;

        int event_id;
        while (fscanf(events_file, "%d\n", &event_id) != EOF) {
            char *type_event_id_str = read_event(user_id, event_id, "event_type_id");
            if (type_event_id_str) {
                int type_event_id = atoi(type_event_id_str);
                free(type_event_id_str);

                for (int i = 0; i < total_event_types; i++) {
                    char *event_id_str = read_typevent(type_event_id, "event_id");
                    if (event_id_str) {
                        int stored_event_id = atoi(event_id_str);
                        free(event_id_str);

                        if (type_event_id == stored_event_id) {
                            event_counts[i]++;
                            break;
                        }
                    }
                }
            }
        }
        fclose(events_file);
    }
    fclose(user_ids_file);

    // Find the most booked event type
    int max_count = 0;
    int max_count_index = -1;
    int tie_count = 0;

    for (int i = 0; i < total_event_types; i++) {
        if (event_counts[i] > max_count) {
            max_count = event_counts[i];
            max_count_index = i;
            tie_count = 1; // Reset tie count
        } else if (event_counts[i] == max_count) {
            tie_count++;
        }
    }

    const char *most_booked_event = (tie_count > 1 || max_count_index == -1) ? "None" : event_names[max_count_index];

    // Format the result
    char *result = malloc(256);
    if (result == NULL) {
        perror("[ERROR] Memory allocation failed");
        return strdup("[ERROR] Unable to generate report.");
    }

    snprintf(result, 256, "📈 Most Booked Event Type    : %s", most_booked_event);
    return result;
}