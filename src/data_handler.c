// will handle data stuffs.

// might include these f.f functions: read_events,read_reviews, save_event, update_reviews

// will also check if the data.txt exist
// will also check if data.txt isnt empty.

#include <stdlib.h.>
#include <string.h>
#include <stdio.h>

const char *files[][20] = {"reports.txt", "events.txt","config.txt"};

#define DATA_DIR "data/"

FILE *check_file_existence(const char *file_name, const char *access_mode)
{
    char file_path[50];

    snprintf(file_path, sizeof(file_path), "%s%s", DATA_DIR, file_name);

    FILE *file = fopen(file_path, access_mode);

    if(file == NULL)
    {
        printf("Error: %s does not exist.\n", file_path);
        return NULL;
    } else {
        return file;
    }
}

// which file? 
void write()
{

}