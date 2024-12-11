#ifndef EVENTS_H
#define EVENTS_H

// TODO: organize the order of the prototypes.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

// Creates event 0: failed 1: success
int create_event(int id, int type_event_key, char client_name[30], float cost, float balance, int no_attendee, char venue[40], char completion_date[40]);

// Reads a specific value from event : id
char *read_event(int id, char key[20]);

// Update a specific value from event : id
int update_event(int id, char key[20], char value[50]);

// Deletes event : id
void delete_event(int id);

// Prints all of the events
void preview_events();

void count_event_types();
int translate_event_type(const char key[]);
int count_events();

// Checks if event : id is valid.
int is_valid_id(int id);

// Should put sa data_handler
int generate_unique_id();

int init_myevent();


#endif // EVENTS_H