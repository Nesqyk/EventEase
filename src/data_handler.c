// will handle data stuffs.

// might include these f.f functions: read_events,read_reviews, save_event, update_reviews

// will also check if the data.txt exist
// will also check if data.txt isnt empty.

#include <stdlib.h.>
#include <string.h>
#include <stdio.h>

#define DATA_DIR "data\""

void HandleData()
{
    FILE *file;

    // Check if the f.f txt files exist.
    char files[][20] = {"reports.txt", "events.txt"};

    for(int i = 0; i < sizeof(files) / sizeof(files[0]); i++)
    {
        file = fopen(strcat(DATA_DIR, files[i]), "r");
        
        if(file == NULL)
        {
            printf("Error: %s does not exisit.", files[i]);
            return 1;
        } else {
            fclose(file);  
        }
    }
}
