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

#define MAX_LENGTH 100

// Checks whether an ID is a duplicate.

/*

char **read_type_of_events() {
    static char buffer[MAX_LINE];

    
    int size = 0;
    char** arr = NULL;
    arr = (char**)malloc(size * sizeof(char*));

    FILE *config = fopen(CONFIG_FILE, "r");
    if (config == NULL) {
        perror("Error opening file");
        return NULL;
    }

    while (fgets(buffer, sizeof(buffer), config)) 
    {
        if (strcmp(buffer, "type_of_events:") == 0) {
            if(fgets(arr[size], MAX_LENGTH, config) != NULL)
            {
                size++;
            }
            continue;
        }
    }

    fclose(config);
    return arr;
}
*/

// Read config.txt
char *read_config(const char key[20]) {
    char filename[30];
    sprintf(filename, "%sconfig.txt", DATA_DIR);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    static char stored_max_event[5], stored_max_attendee[5];
    while (fscanf(file, "max_events:%s\nmax_attendee:%s", stored_max_event, stored_max_attendee) == 2) {
        if (strcmp(key, "max_events") == 0) 
        {
            fclose(file);
            return stored_max_event;
        } else if (strcmp(key, "max_attendee") == 0) {
            fclose(file);
            return stored_max_attendee;
        }
    }
    fclose(file);
    return NULL;
}
