#ifndef EVENTS_H
#define EVENTS_H

// TODO: organize the order of the prototypes.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

// Creates event
void create_event(int id, int type_event_key, char client_name[30], float cost, float balance, int no_attendee, char venue[40], char completion_date[40]);

void get_event_type_status();

char *read_event(int id, char key[20]);

int translate_event_type(const char key[]);

int count_events();

int is_valid_id(int id);

void list_events();

void update_event(int id, char key[20], char value[50]);

int generate_unique_id();


#endif // EVENTS_H