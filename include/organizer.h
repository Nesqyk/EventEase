#ifndef ORGANIZER_H
#define ORGANIZER_H

const char *type_keys[] = {"event_name","event_description","event_event_id","venues"};
int type_keys_size = sizeof(type_keys) / sizeof(type_keys[0]);

#define MAX_LINE 256

#define TYPE_EVENT_ID_FILE "data/type_event_ids.txt"
#define TYPE_PKG_ID_FILE "pkg_ids.txt"
#define TYPE_EVENTS_DIR "data/events/"
#define TYPE_INFO_FILE "type_info.txt"
#define VENUE_FILE "venue_choices.txt"
#define PACKAGE_ID_FILE "package_ids.txt"

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
// typeevent prototypes

char *read_eventtype_all(const char key[50]);
int create_typeevent(TypeEvent typeevent);

char *read_typevent(int event_id, char key[50]);

int update_typeevent(int event_id, char key[50], char *value);

int valid_typeevent_id(int event_id);

int generate_typeevent_id();


// packages prototype
int add_pkg(int event_id, Package pkg);

char *read_pkg(int event_id, int pkg_id, char key[50]);

int update_pkg(int event_id, int pkg_id, char key[50], char *value);

int delete_pkg(int event_id, int pkg_id);

int generate_pkg_id(int event_id);

int valid_pkg_id(int event_id, int pkg_id);



#endif //ORGANIZER.H