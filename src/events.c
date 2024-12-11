
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "data_handler.h"
#include "utils.h"

static char type_events[4][100] = {"Wedding","Birthday","Graduation","Party"};


#define MAX_LINE 256
#define MAX_TYPE_EVENTS 10
#define MAX_ID_RANGE 100



void count_event_types();

char *read_event(int id, char key[20]);

int is_valid_id(int id);
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

void count_event_types()
{
    int counts[4] = {0};

    FILE *event_id_file = fopen(EVENTID_FILE, "r");

    if(event_id_file == NULL)
    {
        perror("Error opening event id file");
    }

    char buffer[MAX_LINE];

    while(fgets(buffer, MAX_LINE, event_id_file))
    {
        if(strcmp(buffer, "") == 0) break;
        int ids = atoi(buffer);
  

        // char event_filename[50];  
        int event_type = atoi(read_event(ids, "type"));
        // later if(event_type > sizeof(type_events) / sizeof(type_events[0])) continue;

        // sprintf(event_filename, "%s%d.txt", EVENT_DIR, ids);
        for(int i = 0; i < sizeof(type_events) / sizeof(type_events[0]); i++)
        {
            if(i == event_type)
            {
                counts[i]++; // ?
                break;
            }
        }
    }
    
    fclose(event_id_file);
    // implement sort; use selection sort. Popular to least popular.
    // learn how to sort stuffs
    printf("Event Type Status: \n");
    
    for(int i = 0; i < sizeof(type_events) / sizeof(type_events[0]); i++)
    {
        // compare the highest count
        int highest = 0;
        if(highest < counts[i])
        {
            highest = counts[i];
        }
        printf("Most Popular Event: %s Count: %d\n", type_events[i], counts[i]);
        printf("Event Type:%s. Count: %d\n", type_events[i], counts[i]);
    }
}

/*
might find use for this later.
int translate_event_type(const char key[])
{
    for(int i = 0; i < sizeof(type_events) / sizeof(type_events[0]); i++)
    {
        if(strcmp(type_events[i], key) == 0)
        {
             return i; 
        }
    }
    return 0;
}

*/

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

void preview_events()
{
    FILE *event_file = fopen(EVENTID_FILE, "r");

    if(event_file == NULL)
    {
        perror("Error opening event id file");
    }

    char buffer[MAX_LINE];

    // Implement a better print format than this 
    printf("ID\tType\tClient Name\tBalance\t\tCompletion Date:\n");
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
        printf("%d\t%s\t%s\t\t\t%s\t\t%s\n", ids, read_event(ids, "type"), read_event(ids, "client_name"), read_event(ids,"balance"), read_event(ids,"date_created"));
    }
    fclose(event_file);
}


//
int create_event(int id, int type_event_key, char client_name[30], float cost, float balance, int no_attendee, char venue[40], char completion_date[40])
{
    char event_filename[50];

    sprintf(event_filename, "%s%d.txt", EVENT_DIR, id);

    FILE *event_file = fopen(event_filename, "w");

    FILE *eventid_file = fopen(EVENTID_FILE, "a");

    if(eventid_file == NULL)
    {
        perror("Error opening event id file");      
    }
    fprintf(eventid_file, "\n%d", id);
    fclose(eventid_file);

    if(event_file == NULL)
    {
        perror("Error opening event file");
        return 0;
    }    

    time_t g_t;
    time(&g_t);

    char *generated_date = ctime(&g_t);

    fprintf(event_file, "type:%d\n", type_event_key);
    fprintf(event_file, "client_name:%s\n", client_name);
    fprintf(event_file, "cost:%.2f\n", cost);
    fprintf(event_file, "balance:%.2f\n", balance);
    fprintf(event_file, "no_attendee:%d\n", no_attendee);
    fprintf(event_file, "venue:%s\n", venue);
    // don't forget to provide format for completion date! i.e y:m:d etc...
    fprintf(event_file, "completion_date:%s\n", completion_date);
    fprintf(event_file, "date_created:%s\n", generated_date);

    fclose(event_file);
    return 1;
}

// Deletes the event 
void delete_event(int id)
{
    if(is_valid_id(id) == 0) return;

    char event_filename[50];

    sprintf(event_filename, "%s%d.txt", EVENT_DIR, id);

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

    char *string_keys[] = {"type", "client_name", "cost", "balance", "no_attendee", "venue", "completion_date","date_created"};

    int valid_key = 0;
    for (int i = 0; i < sizeof(string_keys) / sizeof(string_keys[0]); i++) {
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

    static char type[50], client_name[50], cost[20], balance[20], no_attendee[10], venue[50], completion_date[20], date_created[50];

    // need to implement a better way to read things
    while (fscanf(event_file,
                  "type:%[^\n]\nclient_name:%[^\n]\ncost:%[^\n]\nbalance:%[^\n]\nno_attendee:%[^\n]\nvenue:%[^\n]\ncompletion_date:%[^\n]\ndate_created:%[^\n]",
                  type, client_name, cost, balance, no_attendee, venue, completion_date, date_created) == 8) {

        if(strcmp(key, "date_created") == 0)
        {
            fclose(event_file);
            return date_created;
        }
        else if (strcmp(key, "type") == 0) {
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

    // recursion?
    // if(read_event(id, key) == NULL)
    // {
     //   return "";
    // }
    fclose(event_file);
    return NULL; 
}


// Updates the event
int update_event(int id, char key[20], char value[50])
{
    char event_filename[50];

    sprintf(event_filename, "%s%d.txt", EVENT_DIR, id);

    char *string_keys[] = {"type", "client_name", "cost", "balance", "no_attendee", "venue", "completion_date"};

    int valid_key = 0;
    for(int i = 0; i < sizeof(string_keys) / sizeof(string_keys[0]); i++)
    {
        if(strcmp(key, string_keys[i]) != 0)
        {
            return 0;
        }
    }

    FILE *event_file = fopen(event_filename, "a");

    if(event_file == NULL)
    {
        perror("Error opening event file");
        return 0;
    }

    char buffer[MAX_LINE];
    
    while(fgets(buffer, MAX_LINE, event_file))
    {
        if(strcmp(buffer, key) == 0)
        {
            fprintf(event_file, "%s:%s\n", key, value);
            fclose(event_file);
            return 1;
        }
    }
    fclose(event_file);
    return 0;
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


int init_myevent()
{
    int choice;
    int id;

    while(1)
    {
        printf("\n\n===My Events Menu===\n");
        printf("1. Preview Events\n");
        printf("2. View Event\n");
        printf("3. Edit Event\n");
        printf("4. Back to Main Menu\n");

        // check if si choice kay valid
        printf("\nEnter Choice: ");
        scanf("%d", &choice);


        switch(choice)
        {
            case 1:
                // will list the events for the client.
                preview_events();
                continue;
            case 2:
                printf("Please input an ID to view:");
                scanf("%d", &id);

                while(is_valid_id(id) == 0)
                {
                    printf("Please input a valid event ID: ");
                    scanf("%d", &id);
                }

                preview_event(id);
                continue;

            case 3:
                printf("Please pick an ID from the f.f:");
                preview_events();

                printf("Id: ");
                scanf("%d", &id);

                while(is_valid_id(id) == 0)
                {
                    printf("Please input a valid event ID: ");
                    scanf("%d", &id);
                }

                printf("Which value would you want to update?\n");
                char *valid_string_keys[] = {"type", "client_name", "cost", "balance", "no_attendee", "venue", "completion_date"};
                valid_key:
                    for (int i = 0; i < sizeof(valid_string_keys) / sizeof(valid_string_keys[0]); i++) 
                    {
                        printf("%s, ", valid_string_keys[i]);
                    }

                printf("\nValue to Change: ");
                char key[20];
                fgets(key, sizeof(key), stdin);
                
                for (int i = 0; i < sizeof(valid_string_keys) / sizeof(valid_string_keys[0]); i++) 
                {
                    if (strcmp(key, valid_string_keys[i]) != 0) 
                    {
                        printf("Invalid input. Please try again.\n");
                        goto valid_key;
                    }
                }

                char *original_value = read_event(id, key);

                printf("Enter new value: ");
                char value[50];
                scanf("%s", value);

                int update = update_event(id, key, value);
                while(update != 1)
                {
                    goto valid_key;
                }
                printf("Successfully updated: %s, from: %s -> %s", key, original_value, value);

                continue;
            case 4:
                return -1;
            break;
            default:
                printf("Invalid Choice. Please try again.\n");
                continue;
        }
    return 0;
    }
}

// y:m:d
/*
char translate_date(char format[])
{

}
*/

