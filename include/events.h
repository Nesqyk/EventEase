#ifndef EVENTS_H
#define EVENTS_H

// TODO: organize the order of the prototypes.

#define MAX_LINE 256

typedef struct {
    int id; // generated
    int package_id; // generated
    int event_type_id; // generated
    char venue[50]; // input
    char name[50]; // input
    char event_date[50]; // input
    int balance; // package balance or down payment
    char start_time[50]; // input
    char payment_deadline[50];
    int status; // 1 -> Confirmed  2 - > In Progress 3 - > Incomplete Payment!
} BookEvent;

int book_event(int client_id, BookEvent event);

int update_event(int client_id, int event_id, char *key, char *value);

char *read_event(int client_id, int event_id, char *key);

int cancel_event(int client_id, int event_id);

int valid_event_id(int client_id, int event_id);

int generate_event_id();

char **preview_events(int client_id, int *event_count, int **book_id);

char *prev_events(int client_id);



#endif // EVENTS_H