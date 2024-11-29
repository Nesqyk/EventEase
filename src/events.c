
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// save data when opening and closing the program so that it will store in the global variable


#include "include/data_handler.h"

// check if an event exist
// I could probably add a config here and let the event organizer 
//  edit his/her type of events he / she is catering.
//  check if it's empty

// prompt for the f.f: name, cost, venue.

#define MAX_LINE 256
#define MAX_TYPE_EVENTS 10
#define MAX_ID_RANGE 1000

const int max_attendee = 50;

int generate_unique_id();

// might also return this to event as a whole;
// later na ang scanf sa main nalang kay para wala na hasol.
// how should the events.txt look too?
// if client name has special chars
// if client name is valid?
// if no_attendee > max_attendee

void create_event(int type, char client_name[], long cost, int no_attendee, char venue[])
{
    char filename[20];
    // how can I randomize this id?
    int id = generate_unique_id();
    
    sprintf(filename, "%s%d.txt", EVENT_DIR, id);

    FILE *event = fopen(filename, "a");

    if(event == NULL)
    {
        printf("Error opening %s", filename);
    }

    FILE *file_id = fopen("data/events_ids.txt", "a");

    if(file_id != NULL) 
    {
        fprintf(file_id, "%d\n", id);
    } else 
    {
        printf("Error opening events_ids.txt");
    }

    fclose(file_id);



    // 1 = done, 0 = wala pa
    int classfication = 0;
    char *review;

    // time_t = time(NULL);
    

    // how to randomize id; mt_rand maybe?
    //  also don't forget to store the date using a string
}

// i'm trying to find out hgow to generae unique id efficiently
int generate_unique_id()
{
    int event_id;

    do {
        event_id =  rand() % MAX_ID_RANGE;
    } while (is_duplicated(event_id) == 1);

    return event_id;
}

int event_ids[];

void save_event_id()
{
    FILE *event_id_file = fopen("data/event_ids.txt" ,"r");

    int max_event = atoi(read_config("max_event:"));
    event_ids[max_event];

    int index = 0;

    if(!event_id_file)
    {
        printf("Error opening event_ids.txt");
        return 0;
    }

    char buffer[MAX_LINE];

    while(fgets(buffer, sizeof(buffer), event_id_file))
    {
        buffer[strcspn(buffer,"\n")] = 0;

        if(max_event >= index)
        {
            event_ids[index]  = atoi(buffer);
            index++;
        } else {
            break;
        }
    }
}
// it must return to event[];
// TODO: find a better approch to this; a better way to 
// allocate memory without losing the data from event[]; 
// since  there are restraints; we can only use things that was taught to us.


// Prints the event list 
void get_event_type_list()
{
    char *type_event = getEventType();

    for(int i = 0; sizeof(type_event) / sizeof(type_event[0]); i++)
    {
        printf("%d. %s", i + 1, type_event[i]);
    }
}

// standard deviation is grade[index] - average
// variance (deviation * deviation) + (deviation)
// variance += deviation * deviation;
// deviation[i] = grade[i] - average;

