// will handle data stuffs.

// might include these f.f functions: read_events,read_reviews, save_event, update_reviews

// will also check if the data.txt exist
// will also check if data.txt isnt empty.

#include <stdlib.h.>
#include <string.h>
#include <stdio.h>

const char *files[][20] = {"reports.txt", "events.txt","config.txt"};

#define DATA_DIR "data/"
#define EVENT_DIR "data/events"
#define MAX_LINE 256

FILE *check_file_existence(const char *file_name, const char *access_mode)
{
    char file_path[50];

    snprintf(file_path, sizeof(file_path), "%s%s", DATA_DIR, file_name);

    FILE *file = fopen(file_path, access_mode);

    if(file == NULL)
    {
        printf("Error: %s does not exist.\n", file_path);
        return NULL;
    } else {
        return file;
    }
}


// for event
// dont forget to store everytime a txt file is created.
int is_duplicated(int id)
{
    char file_id[20];
    FILE *event_ids = check_file_existence("event_ids", "r");

    int lines[MAX_LINE];

    while(fgets(lines, sizeof(MAX_LINE), event_ids))
    {
        if(strcmp(id, event_ids) == 0)
        {
            return 1;
        }
    }

    return 0;
    // there should be a file that contains all the id of the event
    //  this file should be crucial kay siya mo determine whether if that .txt file is a duplciate or not
}

// which key 
char *read_config(char key[30])
{
    char *valid_keys[] = {"type_of_events:", "max_attendee:","max_event:"};
    int is_valid_key = 0;

    for(int i = 0; i < sizeof(valid_keys) / sizeof(*valid_keys); i++)
    {
        if(strcmp(key, valid_keys[i]) == 0)
        {
            is_valid_key = 1;
            break;
        }      
    }

    if(!is_valid_key)
    {
        printf("Invalid key: %s", key);
        return NULL;
    }

    FILE *config = fopen("config.txt", "r");  
    if(config == NULL)
    {
        perror("Error opening file");
        return NULL;
    } 

    char buffer[MAX_LINE];

    while(fgets(buffer, sizeof(buffer), config))
    {
        buffer[strcspn(buffer, "\n")] = 0;

        if(strcmp(buffer, key) == 0)
        {
            // read for next; value
            if(fgets(buffer, sizeof(buffer), config))
            {
                buffer[strcspn(buffer,"\n")] = 0;
                fclose(config);
                return buffer;
            }
        }

    }
    printf("Invalid Key: %s", key); 
    fclose(config);

    return NULL;
}