#ifndef EVENTS_H
#define EVENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

static char type_events[4][100] = {"Wedding","Birthday","Graduation","Party"};

// Creates event
void create_event(int id, int type_event_key, char client_name[30], float cost, float balance, int no_attendee, char venue[40], char completion_date[40]);

void get_event_type_status();

int translate_event_type(const char key[]);

int count_events();


#endif // EVENTS_H