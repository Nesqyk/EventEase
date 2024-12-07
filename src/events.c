
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// save data when opening and closing the program so that it will store in the global variable


// TODO: Will continue on create_event and stuffs.

#include "data_handler.h"

// check if an event exist
// I could probably add a config here and let the event organizer 
//  edit his/her type of events he / she is catering.
//  check if it's empty

// prompt for the f.f: name, cost, venue.

#define MAX_LINE 256
#define MAX_TYPE_EVENTS 10
#define MAX_ID_RANGE 1000

const int max_attendee = 50;

//
char *read_event(int id, char key[20]);
int is_valid_id(int id);
// int is_valid_event_type(char *type_eventname);
int generate_unique_id();
// void get_event_type_list();

/* might also return this to event as a whole;
// later na ang scanf sa main nalang kay para wala na hasol.
// how should the events.txt look too?
// if client name has special chars
// if client name is valid?
// if no_attendee > max_attendee
*/


// pangan ni gibayad ni client - variable
// ang date ayaw kalimti!
// date - created
//      - kung when ang event

void list_events()
{
    FILE *event_file = fopen(EVENTID_FILE, "r");

    if(event_file == NULL)
    {
        perror("Error opening event id file");
    }

    char buffer[MAX_LINE];

    while(fgets(buffer, MAX_LINE, event_file))
    {
        int ids = atoi(buffer);

        char event_filename[50];  

        sprintf(event_filename, "%s%d", EVENT_DIR, ids);

        FILE *event_file = fopen(event_filename, "r");

        if(event_file == NULL)
        {
            continue;
        }

        printf("ID\tType\tClient Name\n");
        printf("%d\t%s\t%s", ids, read_event(ids, "type:"), read_event(ids, "client_name:"));
    }
    fclose(event_file);
}

/*
void search_event(int id)
{
    // check kung naa ba ang id
    // if naa, print ang details
    // if wala, print not found
    // store ang last searched id
    // lastSearchedId <- no need nani

    int id, i;
    printf("Enter Event ID to search: ");
    scanf("%d", &id);

    // list every id in the event_ids.txt

    if(!is_valid_id(id))
    {
        // false
    }

    for (i = 0; i < count; i++)
    {
        // Check if the ID exists 
        if (ids[i] == id)
        {
            printf("Event Found: %s (ID: %d, Cost: %.2lf, Revenue: %.2lf)\n", names[i], ids[i], costs[i], balance[i]);
            lastSearchedId = id; // Store the searched ID
            return;
        }
    }

    printf("Event with ID %d not found.\n", id);
    // lastSearchedId = -1; // Reset the last searched ID if the ID is not found
}
*/

// events
//  111.txt
//  123.txt 
// two separate date -> date of when the event is created, and when will it be initiated
void create_event(char type[40], char client_name[30], float cost, float balance, int no_attendee, char venue[40], char completion_date[40])
{
    char event_filename[50];

    int id = generate_unique_id();

    sprintf(event_filename, "%s%d", EVENT_DIR, id);

    FILE *event_file = fopen(event_filename, "w");

    FILE *eventid_file = fopen(EVENTID_FILE, "a");

    if(eventid_file == NULL)
    {
        perror("Error opening event id file");        
    } else 
    {
        fprintf(eventid_file, "%d\n", id);
    }

    if(event_file == NULL)
    {
        perror("Error opening event file");
    }


    int valid_char = 0;

    for(int i = 0; i <  strlen(client_name); i++)
    {
        if((client_name[i] < 'a' && client_name[i] > 'z') || (client_name[i] < 'A' && client_name[i]  > 'Z'))
        {
            valid_char = 1;
        }
    }

    if(!valid_char)
    {
        printf("Invalid client name");
    }
    

    time_t g_t;
    time(&g_t);

    char *generated_date = ctime(&g_t);

    fprintf(event_file, "type:\n%s\n", type);
    fprintf(event_file, "client_name:\n%s\n", client_name);
    fprintf(event_file, "cost:\n%f\n", cost);
    fprintf(event_file, "balance:\n%f\n", balance);
    fprintf(event_file, "no_attendee:\n%d\n", no_attendee);
    fprintf(event_file, "venue:\n%s\n", venue);
    fprintf(event_file, "completion_date:\n%s\n", completion_date);
    fprintf(event_file, "date_created:\n%s\n", generated_date);

    fclose(event_file);
}

// Deletes the event 
void delete_event(int id)
{
    char event_filename[50];

    sprintf(event_filename, "%s%d", EVENT_DIR, id);

    if(remove(event_filename) == 0)
    {
        printf("Event %d has been deleted", id);
    } else 
    {
        printf("Event %d not found", id);
    }
}

char *read_event(int id, char key[20])
{
    static char value[50]; // Use static to persist the value.
    char event_filename[50];

    if (is_valid_id(id) == 0)
    {
        return NULL; // Invalid ID.
    }

    sprintf(event_filename, "%s%d", EVENT_DIR, id);

    FILE *event_file = fopen(event_filename, "r");

    if (event_file == NULL)
    {
        perror("Error opening event file");
        return NULL;
    }

    char buffer[MAX_LINE];
    char key_with_colon[25];
    snprintf(key_with_colon, sizeof(key_with_colon), "%s:", key);

    while (fgets(buffer, MAX_LINE, event_file))
    {
        if (strcmp(buffer, key_with_colon) == 0)
        {
            if (fgets(value, sizeof(value), event_file))
            {
                fclose(event_file);
                return value;
            }
        }
    }

    fclose(event_file);
    return NULL;
}

// Updates the event
void update_event(int id, char key[20], char value[50])
{
    char event_filename[50];

    sprintf(event_filename, "%s%d", EVENT_DIR, id);

    char *string_keys[] = {"type:", "client_name:", "cost:", "balance:", "no_attendee:", "venue:", "completion_date:"};

    int valid_key = 0;
    for(int i = 0; i < 7; i++)
    {
        if(strcmp(key, string_keys[i]) == 0)
        {
            valid_key = 1;
        }
    }

    if(valid_key == 0)
    {
        return;
    }

    FILE *event_file = fopen(event_filename, "a");

    if(event_file == NULL)
    {
        perror("Error opening event file");
    }

    char buffer[MAX_LINE];
    
    while(fgets(buffer, MAX_LINE, event_file))
    {
        if(strcmp(buffer, key) == 0)
        {
            fprintf(event_file, "%s:%s\n", key, value);
        }
    }
    fclose(event_file);
}

// convert int to characters

int is_valid_id(int id)
{
    FILE *event_file = fopen(EVENTID_FILE, "r");

    if(event_file == NULL)
    {
        perror("Error opening event id file");
        return 0;
    }

    char buffer[MAX_LINE];

    while(fgets(buffer, MAX_LINE, event_file))
    {
        if(atoi(buffer) == id)
        {
            return 1;
        }
    }
    fclose(event_file);
    return 0;
}

// i'm trying to find out hgow to generae unique id efficiently
int generate_unique_id()
{
    int event_id;

    do {
        event_id =  rand() % MAX_ID_RANGE;
    } while (is_valid_id(event_id) == 1);

    return event_id;
}

/*
int event_ids[];

// no use yet
void save_event_id()
{
    FILE *event_id_file = fopen(EVENTID_FILE ,"r");

    int max_event = atoi(read_config("max_event:"));
    event_ids[max_event];

    int index = 0;

    if(!event_id_file)
    {
        printf("Error opening event_ids.txt");
    }

    char buffer[MAX_LINE];

    while(fgets(buffer, MAX_LINE, event_id_file))
    {
        if(max_event >= index)
        {
            event_ids[index]  = atoi(buffer);
            index++;
        } else 
        {
            break;
        }
    }
} 
*/

/*
int get_event_type_list()
{
    char *event_type = read_type_of_events();

    int lines = 0;
    FILE *event_file = fopen(CONFIG_FILE, "r");

    if(event_file == NULL)
    {
        return 0;
    }
}

*/

// Prints the type of event available

// Checks if an event type is valid or not

/*
int is_valid_event_type(char *type_eventname)
{
    char *event_type = read_type_of_events();

    for(int i = 0; i < sizeof(event_type); i++)
    {
        if(strcmp(event_type[i], type_eventname) == 0)
        {
            return 1;
        }
    }
    return 0;
}

*/