#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#define DATA_DIR "data/"
#define EVENT_DIR "data/events"
#define MAX_LINE 256

FILE *check_file_existence(const char *file_name, const char *access_mode);

int *is_duplicated(int id);

char read_config(char key[30]);

#endif // DATA_HANDLER_H

