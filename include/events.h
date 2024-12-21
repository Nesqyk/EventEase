#ifndef EVENTS_H
#define EVENTS_H

// TODO: organize the order of the prototypes.

#define MAX_LINE 256

typedef struct 
{
    int id;
    char package_name[50];
    float price;
    int event_type;
    char description[50];
    int availability;
    char created_date[50];
    int max_guest; 
    int duration; // in hours
    char inclusions[200]; // , as a delimeter. or next line?
} Package;

typedef struct 
{
    int id;
    char type[50];
    char name[50];
    char description[100];
    char venues[300];
} TypeEvent;

typedef struct {
    int id;
    int package_id;
    int event_type_id;
    char venue[50];
    char name[50];
    time_t event_date;
    time_t booking_date;
    int balance;
    time_t start_time;
    int status;
} BookEvent;

int book_event(int client_id, BookEvent event);

int update_event(int client_id, int event_id, char *key, char *value);

char *read_event(int client_id, int event_id, char *key);

int cancel_event(int client_id, int event_id);

int valid_event_id(int client_id, int event_id);

int generate_event_id();


#endif // EVENTS_H