#include <stdlib.h>
#include <string.h>
#include <stdio.h>




/*
TODO:
Recreat everthing that is in here; because it's a mess.
*/

#define DATA_DIR "data/"
#define EVENT_DIR "data/events/"

#define EVENTID_FILE "data/events_ids.txt"
#define CONFIG_FILE "data/config.txt"

#define MAX_LINE 255

// Checks whether an ID is a duplicate.
int is_valid(int id)
{
    FILE *event_ids = fopen(EVENTID_FILE,"r");

    char buffer[MAX_LINE];

    if(event_ids == NULL) 
    {
        printf("Unable to open the file");
    } else 
    {
        while(fgets(buffer, MAX_LINE, event_ids))
        {
            if (id == atoi(buffer)) {
                return 1; 
            }
        }
        fclose(event_ids);
    }
    return 0;
}

char* read_type_of_events() {
    static char buffer[MAX_LINE];
    static char result[MAX_LINE * 10];
    FILE *config = fopen("config.txt", "r");
    if (config == NULL) {
        perror("Error opening file");
        return NULL;
    }

    int key_found = 0;
    result[0] = '\0';  
    while (fgets(buffer, sizeof(buffer), config)) 
    {
        if (strcmp(buffer, "type_of_events:") == 0) {
            key_found = 1;
            continue;
        }

        if (key_found) {
            if (strcmp(buffer, "") == 0)
                break;  

            strcat(result, buffer);
            strcat(result, "\n");
        }
    }

    fclose(config);
    return result;
}

// Read config.txt
char* read_config(const char key[30]) {
    const char *valid_keys[] = {"max_attendee:", "max_events:"};
    int is_valid_key = 0;

    // Validate the key
    for (int i = 0; i < 2; i++) 
    {
        if (strcmp(key, valid_keys[i]) == 0) 
        {
            is_valid_key = 1;
            break;
        }
    }

    if (!is_valid_key) 
    {
        printf("Invalid key: %s\n", key);
        return NULL;
    }

    static char buffer[MAX_LINE];
    FILE *config = fopen("config.txt", "r");

    if (config == NULL) 
    {
        perror("Error opening file");
        return NULL;
    }

    int key_found = 0;
    while (fgets(buffer, sizeof(buffer), config)) 
    {
        for(int i = 0; i < strlen(buffer); i++)
        {
            if(buffer[i] == '\n')
            {
                buffer[i] = '\0';
                break;
            }
        }
        if (strcmp(buffer, key) == 0) 
        {
            key_found = 1;
            continue; 
        }

        if (key_found) 
        {
            if (strcmp(buffer, "") == 0) break;
                return buffer;
        }
    }

    fclose(config);
    if (!key_found) 
    {
        printf("Key not found: %s\n", key);
    }
    return NULL;
}
