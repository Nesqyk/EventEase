
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_handler.h"

// check if an event exist
// I could probably add a config here and let the event organizer 
//  edit his/her type of events he / she is catering.
//  check if it's empty

// prompt for the f.f: name, cost, venue.

#define MAX_LINE 256
#define MAX_TYPE_EVENTS 10

// might also return this to event as a whole;
// later na ang scanf sa main nalang kay para wala na hasol.
// how should the events.txt look too?
void createEvent(int type, char client_name, long cost, int no_attendee, char venue)
{
    FILE *data = CheckFileExistence("events.txt","a");
    // how can I randomize this id?
    int id;

    // event.txt should look like this:
    // id:
    //  client_name:
    //  cost:
    //  no_attendee

    // must also check if id1 = id2; kay pariho 
    //  if parihos then generate another random id;

    // 1 = done, 0 = wala pa
    int classfication = 0;
    char *review = "";

    char date[100];

    // how to randomize id; mt_rand maybe?
    //  also don't forget to store the date using a string
}


// it must return to event[];
// TODO: find a better approch to this; a better way to 
// allocated memory without losing the data from event[]; 
// since  there are restraints, we can only use things that was taught to us.
char *getEventType()
{
    // Don't forget to close after.
    FILE *config = CheckFileExistence("config.txt", "r");

    
    char line[MAX_LINE];

    char *events[MAX_TYPE_EVENTS];

    int event_count = 0;
    int reading_event = 0;

    while(fgets(line, sizeof(line), config))
    {
        // we're reading event here so we should;
        // check every new line
        // if new line is empty; break else continue;
        line[strcspn(line , "\n")] = 0;

        if(strncmp(line, "type_of_events:", 15) == 0)
        {
            reading_event = 1;
            continue;
        }

        if(reading_event && strlen(line) > 0)
        {
            events[event_count] = line;
            event_count++;

            if(event_count >= MAX_TYPE_EVENTS)
            {
                break;
            }
        }

        if(strlen(line) == 0)
        {
            reading_event = 0;
        }
    }
    fclose(config);

    return events;
}


// Prints the event list 
void getEventTypeList()
{
    char *type_event = getEventType();

    for(int i = 0; sizeof(type_event) / sizeof(type_event[0]); i++)
    {
        printf("%d. %s", i + 1, type_event[i]);
    }
}