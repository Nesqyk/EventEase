#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#define DATA_DIR "data/"
#define EVENT_DIR "data/events/"

#define EVENTID_FILE "data/events_ids.txt"
#define CONFIG_FILE "data/config.txt"

#define MAX_LINE 256

char* read_config(const char key[20]);

// char** read_type_of_events();

char* read_auth_key(const char key[20]);


#endif // DATA_HANDLER_H

