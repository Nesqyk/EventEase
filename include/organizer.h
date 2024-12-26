#ifndef ORGANIZER_H
#define ORGANIZER_H

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
    int availability; // 1 available 0 Not Available
    char created_date[50];
    int max_guest; 
    int duration; // in hours
    char inclusions[200]; // , as a delimeter. or next line?
} Package;

// "package_id:%d\n" 
//     "package_name:%s\n"
//     "price:%.2f\n"
//     "event_type:%d\n"
//     "description:%s\n"
//     "availability:%d\n"
//     "created_date:%s\n"
//     "max_guest:%d\n"
//     "duration:%d\n"
//     "inclusions:%s\n\n",
typedef struct 
{
    const char *key;
    const char *display_name;
} Field;

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

char *preview_event_type();


// packages prototype
int add_pkg(int event_id, Package pkg);

char *preview_pkgs(int event_id);

char *read_pkg(int event_id, int pkg_id, char key[50]);

int update_pkg(int event_id, int pkg_id, char key[50], char *value);

int delete_pkg(int event_id, int pkg_id);

int generate_pkg_id(int event_id);

int valid_pkg_id(int event_id, int pkg_id);

char *read_all_pkg(int event_id, char key[50]);


#endif //ORGANIZER_H