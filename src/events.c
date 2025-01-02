
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
    fprintf(e_file, "balance:%d\n",event.balance);
    fprintf(e_file, "start_time:%s\n", event.start_time);
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

char **preview_events(int client_id, int *event_count, int **book_ids) {
    char path[50];
    sprintf(path, "data/users/%d/book_event_id.txt", client_id);

    FILE *ids_file = fopen(path, "r");
    if (ids_file == NULL) {
        perror("[ERROR] Unable to open book_event_id.txt");
        *event_count = 0;
        *book_ids = NULL;
        return NULL;
    }

    char *events[101];  // Fixed-size array of pointers for events
    int temp_book_ids[101];  // Temporary array to store book IDs
    *event_count = 0;
    int id;

    while (fscanf(ids_file, "%d\n", &id) != EOF) {
        if (*event_count >= 100) {
            printf("[WARNING] Maximum number of events reached. Some events will not be loaded.\n");
            break;
        }

        char info_filename[256];
        sprintf(info_filename, "data/users/%d/events/%d.txt", client_id, id);

        FILE *info_file = fopen(info_filename, "r");
        if (info_file == NULL) {
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

        while (fscanf(info_file, "%49[^:]:%999[^\n]\n", key_buffer, value_buffer) == 2) {
            if (strcmp(key_buffer, "name") == 0) {
                strncpy(event_name, value_buffer, sizeof(event_name));
            } else if (strcmp(key_buffer, "event_date") == 0) {
                strncpy(event_date, value_buffer, sizeof(event_date));
            } else if (strcmp(key_buffer, "id") == 0) {
                temp_book_ids[*event_count] = atoi(value_buffer);
            } else if (strcmp(key_buffer, "start_time") == 0) {
                strncpy(start_time, value_buffer, sizeof(start_time));
            } else if (strcmp(key_buffer, "venue") == 0) {
                strncpy(venue, value_buffer, sizeof(venue));
            } else if (strcmp(key_buffer, "package_id") == 0) {
                // Resolve package ID to a name if necessary
                sprintf(package_name, "%s", read_pkg(atoi(value_buffer), atoi(value_buffer), "package_name"));
            } else if (strcmp(key_buffer, "status") == 0) {
                int int_status = atoi(value_buffer);
                switch (int_status) {
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
                 "Event: %s\n"
                 "   Book ID: %d\n"
                 "   Date: %s | Time: %s\n"
                 "   Venue: %s\n"
                 "   Package: %s\n"
                 "   Status: %s\n"
                 "\n--------------------------------------------------\n",
                 event_name, temp_book_ids[*event_count], event_date, start_time, venue, package_name, status);

        events[*event_count] = strdup(formatted_event);
        if (events[*event_count] == NULL) {
            perror("[ERROR] Memory allocation failed");
            break;
        }
        (*event_count)++;
    }
    fclose(ids_file);

    if (*event_count == 0) {
        *book_ids = NULL;
        return NULL;
    }

    // Allocate memory for book_ids
    *book_ids = malloc(sizeof(int) * (*event_count));
    if (*book_ids == NULL) {
        perror("[ERROR] Memory allocation failed for book IDs");
        for (int i = 0; i < *event_count; i++) {
            free(events[i]);
        }
        *event_count = 0;
        return NULL;
    }

    // Copy book IDs to the dynamically allocated array
    for (int i = 0; i < *event_count; i++) {
        (*book_ids)[i] = temp_book_ids[i];
    }

    // Allocate memory for event strings
    char **result = malloc(sizeof(char *) * (*event_count + 1)); // +1 for NULL terminator
    if (result == NULL) {
        perror("[ERROR] Memory allocation failed for events array");
        for (int i = 0; i < *event_count; i++) {
            free(events[i]);
        }
        free(*book_ids);
        *book_ids = NULL;
        *event_count = 0;
        return NULL;
    }

    for (int i = 0; i < *event_count; i++) {
        result[i] = events[i];
    }
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

    // Path to the event directory
    char booked_event_dir[256];
    sprintf(booked_event_dir, "data/users/%d/events/%d/", client_id, event_id);

    // Path to the cancelled events folder
    char cancelled_event_dir[256];
    sprintf(cancelled_event_dir, "data/events/cancelled_events/%d/", event_id);

    struct stat st = {0};
    if (stat("data/events/cancelled_events/", &st) == -1) 
    {
        if (mkdir("data/events/cancelled_events/") != 1) 
        {
            perror("[ERROR] Failed to create cancelled events directory");
            return -1;
        }
    }

    // Rename the event directory to move it
    if (rename(booked_event_dir, cancelled_event_dir) != 0) 
    {  
        perror("[ERROR] Failed to move event directory to cancelled_events folder");
        return -1;
    }
    
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
