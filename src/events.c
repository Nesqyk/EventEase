
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "events.h"
#include "users.h"

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

    fprintf(e_file, "id:%d\n", event.id);
    fprintf(e_file, "package_id:%d\n", event.package_id);
    fprintf(e_file, "event_type_id:%d\n", event.event_type_id);
    fprintf(e_file, "venue:%s\n", event.venue);
    fprintf(e_file, "name:%s\n", event.name);

    char date_buffer[80];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d %H:%M:%S", localtime(&event.event_date));
    fprintf(e_file, "event_date:%s\n", date_buffer);

    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d %H:%M:%S", localtime(&event.booking_date));
    fprintf(e_file, "booking_ate:%s\n", date_buffer);

    fprintf(e_file, "balance:%d\n",event.balance);

    strftime(date_buffer, sizeof(date_buffer), "%H:%M:%S", localtime(event.start_time));
    fprintf(e_file, "start_time:%s\n", date_buffer);

    fprintf(e_file, "status:%d\n", event.status);

    fclose(e_file);

    char dir_name[256];   
    // ../data/users/id
    sprintf(dir_name, "%s%d/book_event_id.txt", USERS_DIR, client_id);

    FILE *event_file = fopen(dir_name, "a");

    fprintf(event_file, "%d\n", event.id);
    fclose(event_file);

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
    sprintf(info_filename, "%s%d/%d", USERS_DIR, client_id, event_id);

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
    // Path to the client's booked events file
    sprintf(dir_name, "%s%d/book_event_id.txt", USERS_DIR, client_id);

    FILE *event_file = fopen(dir_name, "r"); // Open for reading
    if (event_file == NULL) 
    {
        perror("[ERROR] Error opening book_event_id.txt for reading");
        return -1;
    }

    // Temporary file to store updated list
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

    // Copy events to temp file, excluding the one to be canceled
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
        remove(temp_path); // Cleanup temp file
        return -1;
    }

    // Replace the original file with the updated temp file
    if (remove(dir_name) != 0 || rename(temp_path, dir_name) != 0) 
    {
        perror("[ERROR] Failed to update book_event_id.txt");
        return -1;
    }

    // Path to the booked event details
    char booked_event[256];
    sprintf(booked_event, "data/users/%d/events/%d.txt", client_id, event_id);

    // Delete the event details file
    if (remove(booked_event) != 0) 
    {  
        perror("[ERROR] Failed to remove booked event file");
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
