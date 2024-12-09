
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// save data when opening and closing the program so that it will store in the global variable


// TODO: Will continue on create_event and stuffs.

#include "data_handler.h"

static char type_events[4][100] = {"Wedding","Birthday","Graduation","Party"};

// check if an event exist
// I could probably add a config here and let the event organizer 
//  edit his/her type of events he / she is catering.
//  check if it's empty

// prompt for the f.f: name, cost, venue.

#define MAX_LINE 256
#define MAX_TYPE_EVENTS 10
#define MAX_ID_RANGE 100


int translate_event_type(const char key[]);

void get_event_type_status();

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

void get_event_type_status()
{
    int counts[4];

    FILE *event_file = fopen(EVENTID_FILE, "r");

    if(event_file == NULL)
    {
        perror("Error opening event id file");
    }

    char buffer[MAX_LINE];

    while(fgets(buffer, MAX_LINE, event_file))
    {
        if(strcmp(buffer, "") == 0) break;
        int ids = atoi(buffer);

        char event_filename[50];  

        sprintf(event_filename, "%s%d", EVENT_DIR, ids);

        for(int i = 0; i < sizeof(type_events) / sizeof(type_events[0]); i++)
        {
            if(strcmp(type_events[i], read_event(ids, "type")) == 0)
            {
                counts[translate_event_type(type_events[i])]++;
            }
        }
    }
    fclose(event_file);
    // implement sort; use selection sort. Popular to least popular.
    printf("Event Type Status: \n");
    
    for(int i = 0; i < sizeof(type_events) / sizeof(type_events[0]); i++)
    {
        int key = translate_event_type(type_events[i]);
        printf("Event Type:%s. Count: %d\n", type_events[i], counts[key]);
    }
}

int translate_event_type(const char key[])
{
    for(int i = 0; i < sizeof(type_events) / sizeof(type_events[0]); i++)
    {
        if(strcmp(type_events[i], key) != 0)
        {
            return -1;
        } else {
            return i + 1; // start at 1
        }
    }
    return 0;
}

int count_events()
{
    FILE *event_file = fopen(EVENTID_FILE, "r");
    
    if(event_file == NULL)
    {
        return 0;
    }

    char buffer[MAX_LINE];
    int count = 0;
    while(fgets(buffer, MAX_LINE, event_file))
    {
        if(strcmp(buffer, "\n") == 0 && strcmp(buffer, "") == 0)
        {
            break;
        }
        count++;
    }

    return count;
}


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

        sprintf(event_filename, "%s%d.txt", EVENT_DIR, ids);

        FILE *event_file = fopen(event_filename, "r");

        if(event_file == NULL)
        {
            continue;
        }


        printf("ID\tType\tClient Name\n");
        printf("%d\t%s\t%s", ids, read_event(ids, "type"), read_event(ids, "client_name"));
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
void create_event(int id, int type_event_key, char client_name[30], float cost, float balance, int no_attendee, char venue[40], char completion_date[40])
{
    char event_filename[50];

    sprintf(event_filename, "%s%d.txt", EVENT_DIR, id);

    FILE *event_file = fopen(event_filename, "w");

    FILE *eventid_file = fopen(EVENTID_FILE, "a");

    if(eventid_file == NULL)
    {
        perror("Error opening event id file");        
    } else 
    {
        fprintf(eventid_file, "%d\n", id);
        fclose(eventid_file);
    }

    if(event_file == NULL)
    {
        perror("Error opening event file");
    }


    // int valid_char = 0;

    /*
    for(int i = 0; i <  strlen(client_name); i++)
    {
        if((client_name[i] < 'a' && client_name[i] > 'z') || (client_name[i] < 'A' && client_name[i]  > 'Z'))
        {
        
        }
    }

    if(!valid_char)
    {
        printf("Invalid client name");
    }
    */
    

    time_t g_t;
    time(&g_t);

    char *generated_date = ctime(&g_t);

    fprintf(event_file, "type:%d\n", type_event_key);
    fprintf(event_file, "client_name:%s\n", client_name);
    fprintf(event_file, "cost:%.2f\n", cost);
    fprintf(event_file, "balance:%.2f\n", balance);
    fprintf(event_file, "no_attendee:%d\n", no_attendee);
    fprintf(event_file, "venue:%s\n", venue);
    fprintf(event_file, "completion_date:%s\n", completion_date);
    fprintf(event_file, "date_created:%s\n", generated_date);

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


// PROBLEM: where shit returns to null every fking time
// Reads the event
char *read_event(int id, char key[20]) {
    if (is_valid_id(id) == 0) {
        return NULL;
    }

    char event_filename[50];
    sprintf(event_filename, "%s%d.txt", EVENT_DIR, id); 

    char *string_keys[] = {"type", "client_name", "cost", "balance", "no_attendee", "venue", "completion_date"};

    int valid_key = 0;
    for (int i = 0; i < 7; i++) {
        if (strcmp(key, string_keys[i]) == 0) {
            valid_key = 1;
            break; 
        }
    }

    if (!valid_key) {
        return NULL; 
    }

    
    FILE *event_file = fopen(event_filename, "r");
    if (event_file == NULL) {
        perror("Error opening event file");
        return NULL;
    }

    static char type[50], client_name[50], cost[20], balance[20], no_attendee[10], venue[50], completion_date[20];

    while (fscanf(event_file,
                  "type:%49s\nclient_name:%49s\ncost:%19s\nbalance:%19s\nno_attendee:%9s\nvenue:%49s\ncompletion_date:%19s\n",
                  type, client_name, cost, balance, no_attendee, venue, completion_date) == 7) {

        if (strcmp(key, "type") == 0) {
            fclose(event_file);
            return type;
        } else if (strcmp(key, "client_name") == 0) {
            fclose(event_file);
            return client_name;
        } else if (strcmp(key, "cost") == 0) {
            fclose(event_file);
            return cost;
        } else if (strcmp(key, "balance") == 0) {
            fclose(event_file);
            return balance;
        } else if (strcmp(key, "no_attendee") == 0) {
            fclose(event_file);
            return no_attendee;
        } else if (strcmp(key, "venue") == 0) {
            fclose(event_file);
            return venue;
        } else if (strcmp(key, "completion_date") == 0) {
            fclose(event_file);
            return completion_date;
        }
    }

    fclose(event_file);
    return NULL; 
}


// Updates the event
void update_event(int id, char key[20], char value[50])
{
    char event_filename[50];

    sprintf(event_filename, "%s%d.txt", EVENT_DIR, id);

    char *string_keys[] = {"type", "client_name", "cost", "balance", "no_attendee", "venue", "completion_date"};

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
            fprintf(event_file, "%s:%s", key, value);
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
    static int is_seeded = 0;

    if (!is_seeded) {
        srand(time(NULL)); 
        is_seeded = 1;
    }

    int event_id;

    do {
        event_id = rand() % MAX_ID_RANGE;
    } while (is_valid_id(event_id) == 1);

    return event_id;
}

// y:m:d
/*
char translate_date(char format[])
{

}
*/


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