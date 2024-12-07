#ifndef EVENTS_H
#define EVENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

// Creates event
void create_event(char type[40], char client_name[30], float cost, float balance, int no_attendee, char venue[40], char completion_date[40]);

#endif // EVENTS_H