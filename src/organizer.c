#include <stdio.h>
#include <direct.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> 

#include "organizer.h"
#include "utils.h"



/*
add_type_event
add_package
    where na belong na event
    A B C D E ...
    price:
    inclusions: list dire.

remove_package.
remove_inclusion.
remove_type_event.
*/

/*
events/
    event_id/
        packages
*/

char *read_eventtype_all(const char key[50]) 
{
    FILE *ids_file = fopen("data/type_event_ids.txt", "r");
    if (ids_file == NULL) 
    {
        perror("[ERROR] Unable to open type_event_id.txt");
        return NULL;
    }

    char *result = malloc(1); 
    if (result == NULL) 
    {
        perror("[ERROR] Memory allocation failed");
        fclose(ids_file);
        return NULL;
    }

    result[0] = '\0';
    int id;
    while (fscanf(ids_file, "%d\n", &id) != EOF) 
    {
        char info_filename[256];

        sprintf(info_filename, "%s%d/%s", TYPE_EVENTS_DIR, id, TYPE_INFO_FILE);

        FILE *info_file = fopen(info_filename, "r");
        if (info_file == NULL) 
        {
            perror("[ERROR] Unable to open type_info.txt");
            continue; 
        }

        char key_buffer[50];
        char value_buffer[50];
        while (fscanf(info_file, "%49[^:]:%49[^\n]\n", key_buffer, value_buffer) == 2) 
        {
            if (strcmp(key, key_buffer) == 0) 
            {
                size_t current_length = strlen(result);
                size_t additional_length = strlen(value_buffer) + 50; 

                char *temp = realloc(result, current_length + additional_length);
                if (temp == NULL) 
                {
                    perror("[ERROR] Memory reallocation failed");
                    free(result);
                    fclose(info_file);
                    fclose(ids_file);
                    return NULL;
                }
                result = temp;

                sprintf(result + current_length, "%s\n", value_buffer);
            }
        }
        fclose(info_file);
    }
    fclose(ids_file);

    // If no matches were found, return NULL
    if (strlen(result) == 0) 
    {
        free(result);
        return NULL;
    }

    return result;
}


int create_typeevent(TypeEvent typeevent) 
{
    char type_path[256];   
    // "../data/events/id/
    sprintf(type_path, "%s%d/", TYPE_EVENTS_DIR, typeevent.id);

    if (_mkdir(type_path) != 0) 
    { 
        perror("[ERROR] Failed to create typeevent directory");
        return -1;
    }

    char type_file[256];

    sprintf(type_file, "%s%s", type_path, TYPE_INFO_FILE);

    FILE *info_file = create_file(type_file);

    
    time_t g_t;
    time(&g_t);

    char *generated_date = ctime(&g_t);

    fprintf(info_file, "event_id:%d\n", typeevent.id);
    fprintf(info_file, "event_name:%s\n", typeevent.name);
    fprintf(info_file, "description:%s\n", typeevent.description);
    // instead of separating we could use "," as a delimiter
    fprintf(info_file,"venues: %s\n",typeevent.venues);
    fprintf(info_file, "date_created:%s", generated_date);


    FILE *id_file = fopen(TYPE_EVENT_ID_FILE, "a");

    if(id_file == NULL)
    {
        return -1;
    }

    fprintf(id_file , "%d\n", typeevent.id);
    fclose(id_file);

    fclose(info_file);

    /* char venue_path[256]; */

    /* sprintf(venue_path, "%s/%s", type_path, VENUE_FILE);

    FILE *venue_file = create_file(venue_path);

    for(int i = 0; i < sizeof(typeevent.venues) / sizeof(typeevent.venues[0]); i++)
    {
        fprintf(venue_file, "%s\n", typeevent.venues[i]);
    }

    fclose(venue_file); */

    char pkg_dir[50];

    sprintf(pkg_dir, "%s/packages", type_path);

    if(_mkdir(pkg_dir) != 0)
    {
        perror("[ERROR] Failed to create packages directory");
        return -1;
    }

    char pkg_id_path[50];

    sprintf(pkg_id_path, "%s%s",type_path, TYPE_PKG_ID_FILE);

    FILE *pkg_id_list = fopen(pkg_id_path, "w");
    if(pkg_id_list == NULL)
    {
        return -1;
    }

    return 1;
}

char *read_typevent(int event_id, char key[50]) 
{
    if (valid_typeevent_id(event_id) != 1) {
        return NULL; // not this
    }

    char type_dir[256];   
    // data/events/id/type_info,txt
    
    sprintf(type_dir, "%s%d/%s", TYPE_EVENTS_DIR, event_id, TYPE_INFO_FILE);
    printf("%s\n", type_dir);

    FILE *info_file = fopen(type_dir, "r");

    if (info_file == NULL) 
    {
        return NULL;
    }

    char buffer_value[50];
    char key_buffer[50];

    while (!feof(info_file)) 
    {
        if (fscanf(info_file, "%49[^:]:%49[^\n]\n", key_buffer, buffer_value) == 2) 
        {
            if (strcmp(key, key_buffer) == 0) 
            {
                char *result = malloc(strlen(buffer_value) + 1);
                if (result) 
                {
                    strcpy(result, buffer_value);
                }
                fclose(info_file);
                return result;
            }
        }
    }
    fclose(info_file);
    return NULL; // this
}


char *preview_event_type()
{
    FILE *ids_file = fopen("data/type_event_ids.txt", "r");
    if (ids_file == NULL) 
    {
        perror("[ERROR] Unable to open type_event_id.txt");
        return NULL;
    }

    char *result = malloc(1); 
    if (result == NULL) 
    {
        perror("[ERROR] Memory allocation failed");
        fclose(ids_file);
        return NULL;
    }

    result[0] = '\0';
    int id;


    while (fscanf(ids_file, "%d\n", &id) != EOF) 
    {
        char info_filename[256];

        sprintf(info_filename, "%s%d/%s", TYPE_EVENTS_DIR, id, TYPE_INFO_FILE);

        FILE *info_file = fopen(info_filename, "r");
        if (info_file == NULL) 
        {
            perror("[ERROR] Unable to open type_info.txt");
            continue; 
        }

        char key_buffer[50];
        char value_buffer[50];
        while (fscanf(info_file, "%49[^:]:%49[^\n]\n", key_buffer, value_buffer) == 2) 
        {
            
            size_t current_length = strlen(result);
            size_t additional_length = strlen(value_buffer) + 50; 

            if (strcmp(key_buffer, "date_created") == 0) 
            {
                continue;
            }
            
            char *temp = realloc(result, current_length + additional_length);
            if (temp == NULL) 
            {
                perror("[ERROR] Memory reallocation failed");
                free(result);
                fclose(info_file);
                fclose(ids_file);
                return NULL;
            }
            result = temp;

            Field package_field[] = 
            {
                {"event_id", "\033[32mEvent ID\033[0m"},
                {"event_name", "Event Name"},
                {"description", "Description"},
                {"venues", "Venues"},
            };
            int valid = 0;
            for(size_t i = 0; i < sizeof(package_field) / sizeof(package_field[0]); i++)
            {   
                if(strcmp(key_buffer, package_field[i].key) == 0 )
                {
                    valid = 1;
                    sprintf(result + current_length, "\n%s: %s", package_field[i].display_name, value_buffer);
                    current_length += strlen(result + current_length);
                }
            }
            
            if(valid != 1)
            {
                sprintf(result + current_length, "\nUnknown Key : %s\n", value_buffer);
            }

        }
        fclose(info_file);
        
        size_t current_length = strlen(result);
        char *temp = realloc(result, current_length + 2); // Allocate space for '\n' and '\0'
        if (temp == NULL) 
        {
            perror("[ERROR] Memory reallocation failed");
            free(result);
            fclose(ids_file);
            return NULL;
        }
        result = temp;
        strcat(result, "\n");
    }
    fclose(ids_file);

    // If no matches were found, return NULL
    if (strlen(result) == 0) 
    {
        free(result);
        return NULL;
    }

    return result;
}

int update_typeevent(int event_id, char key[50], char *value) 
{
    if (valid_typeevent_id(event_id) != 1) 
    {
        perror("Invalid event_type id");
        return 0;
    }

    char type_dir[256];   

    sprintf(type_dir, "%s%d/%s", TYPE_EVENTS_DIR, event_id, TYPE_INFO_FILE);

    FILE *info_file = fopen(type_dir, "r");

    if (info_file == NULL) 
    {
        perror("Error Opening type_info.txt");
        return 0;
    }

    char temp_path[50];

    sprintf(temp_path, "%s%d/temp_file.txt", TYPE_EVENTS_DIR, event_id);

    FILE *temp_file = fopen(temp_path, "w");

    if(temp_file == NULL)
    {
        fclose(info_file);
        return 0;
    }

    char buffer_value[50];
    char key_buffer[50];

    while (fscanf(info_file, "%49[^:]:%49[^\n]\n", key_buffer, buffer_value) == 2) 
    {
        if (strcmp(key, key_buffer) == 0) 
        {   
            if(value != buffer_value)
            {
                fprintf(temp_file, "%s:%s\n", key, value); 
            }
        } else 
        {
            fprintf(temp_file, "%s:%s\n", key_buffer, buffer_value);
        }
    }

    fclose(info_file);
    fclose(temp_file);

    if (remove(type_dir) != 0 || rename(temp_path, type_dir) != 0) 
    {
        perror("Error updating type_info.txt");
        return 0;
    }
    return 1;
}


int generate_typeevent_id()
{
    FILE *file = fopen(TYPE_EVENT_ID_FILE, "r+"); 
    int last_event_id = 0;

    if (file == NULL) 
    {
        file = fopen(TYPE_EVENT_ID_FILE, "w");
        if (file == NULL) 
        {
            perror("Error creating EVENT_ID file");
            return -1;
        }
        last_event_id = 1;
    } else 
    {
        fscanf(file, "%d\n", &last_event_id);
        last_event_id++;
        rewind(file);
    }

    return last_event_id;
}

int valid_typeevent_id(int event_id) 
{
    FILE *file = fopen(TYPE_EVENT_ID_FILE, "r");

    if (file == NULL)
    {
        perror("Error opening type event_id file");
        return 0;
    }

    char buffer_value[MAX_LINE];

    while (fgets(buffer_value, MAX_LINE, file)) 
    {
        if (atoi(buffer_value) == event_id) 
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}


// packages



/*
i.e:
Wedding:
    Packge A:
        Inclusions:
    Package B:
*/

int add_pkg(int event_id, Package pkg)
{
    char pkg_dir[256];
    
    // ../data/events/id/packages/package_name.txt
    sprintf(pkg_dir, "%s%d/packages/", TYPE_EVENTS_DIR, event_id);

    char pkg_path[256];

    // ..data/events/id/packages/A.txt
    sprintf(pkg_path, "%s/%d.txt", pkg_dir, pkg.id);

    FILE *pkg_file = create_file(pkg_path);

    fprintf(pkg_file,
    "package_id:%d\n" 
    "package_name:%s\n"
    "price:%.2f\n"
    "event_type:%d\n"
    "description:%s\n"
    "availability:%d\n"
    "created_date:%s\n"
    "max_guest:%d\n"
    "duration:%d\n"
    "inclusions:%s\n",
    pkg.id,
    pkg.package_name,
    pkg.price,
    pkg.event_type,
    pkg.description,
    pkg.availability,
    pkg.created_date,
    pkg.max_guest,
    pkg.duration,
    pkg.inclusions
    );

    fclose(pkg_file);

    char id_path[50];
    sprintf(id_path, "%s%d/%s", TYPE_EVENTS_DIR, event_id, TYPE_PKG_ID_FILE);
    
    FILE *id_file = fopen(id_path, "r");

    fprintf(id_file, "%d\n", pkg.id);
    fclose(id_file);

    return 1;
}

char *preview_pkgs(int event_id)
{
    char event_dir[50];

    sprintf(event_dir,"data/events/%d/", event_id);

    char pkg_id_file[50];

    sprintf(pkg_id_file, "%s%s", event_dir, TYPE_PKG_ID_FILE);

    FILE *ids_file = fopen(pkg_id_file, "r");
    if (ids_file == NULL) 
    {
        perror("[ERROR] failed to open ");
        return NULL;
    }

    char *result = malloc(1); 
    if (result == NULL) 
    {
        perror("[ERROR] Memory allocation failed");
        fclose(ids_file);
        return NULL;
    }

    result[0] = '\0';
    int id;
    while (fscanf(ids_file, "%d\n", &id) != EOF) 
    {
        char info_filename[256];

        sprintf(info_filename, "%spackages/%d.txt", event_dir, id);

        FILE *info_file = fopen(info_filename, "r");
        if (info_file == NULL) 
        {
            perror("[ERROR] Unable to open type_info.txt");
            continue; 
        }

        char key_buffer[50];
        char value_buffer[50];

        // figure out how to allocate more memory for inclusions and description.
        while (fscanf(info_file, "%49[^:]:%49[^\n]\n", key_buffer, value_buffer) == 2) 
        {
            if(strcmp(key_buffer, "date_created") == 0)
            {
                continue;
            }

            size_t current_length = strlen(result);
            size_t additional_length = strlen(value_buffer) + 50; 

            char *temp = realloc(result, current_length + additional_length);
            if (temp == NULL) 
            {
                perror("[ERROR] Memory reallocation failed");
                free(result);
                fclose(info_file);
                fclose(ids_file);
                return NULL;
            }
            result = temp;

            Field package_field[] = 
            {
                {"package_id", "Package ID"},
                {"package_name", "Package Name"},
                {"price", "Price"},
                {"event_type", "Event Type"},
                {"description", "Description"},
                {"availability", "Availability"},
                {"created_date", "Date Created"},
                {"max_guest", "Max Guest"},
                {"duration", "Duration"},
                {"inclusions", "Inclusions"}
            };

            int valid = 0;
            for(size_t i = 0; i < sizeof(package_field) / sizeof(package_field[0]); i++)
            {   
                if(strcmp(key_buffer, package_field[i].key) == 0 )
                {
                    valid = 1;
                    sprintf(result + current_length, "\n%s: %s", package_field[i].display_name, value_buffer);
                    current_length += strlen(result + current_length);
                }
            }
            
            if(valid != 1)
            {
                sprintf(result + current_length, "\nUnknown Key : %s\n", value_buffer);
            }

        }
        fclose(info_file);
        
        size_t current_length = strlen(result);
        char *temp = realloc(result, current_length + 2); // Allocate space for '\n' and '\0'
        if (temp == NULL) 
        {
            perror("[ERROR] Memory reallocation failed");
            free(result);
            fclose(ids_file);
            return NULL;
        }
        result = temp;
        strcat(result, "\n");
    }
    fclose(ids_file);

    // If no matches were found, return NULL
    if (strlen(result) == 0) 
    {
        free(result);
        return NULL;
    }

    return result;
}

char *read_all_pkg(int event_id, char key[50])
{
    char event_dir[50];

    sprintf(event_dir,"data/events/%d/", event_id);

    char pkg_id_file[50];

    sprintf(pkg_id_file, "%s%s", event_dir, TYPE_PKG_ID_FILE);

    FILE *ids_file = fopen("%s", "r");
    if (ids_file == NULL) 
    {
        perror("[ERROR] Unable to open type_event_id.txt");
        return NULL;
    }

    char *result = malloc(1); 
    if (result == NULL) 
    {
        perror("[ERROR] Memory allocation failed");
        fclose(ids_file);
        return NULL;
    }

    result[0] = '\0';
    int id;
    while (fscanf(ids_file, "%d\n", &id) != EOF) 
    {
        char info_filename[256];

        sprintf(info_filename, "%spackages/%d.txt", event_dir, id);

        FILE *info_file = fopen(info_filename, "r");
        if (info_file == NULL) 
        {
            perror("[ERROR] Unable to open type_info.txt");
            continue; 
        }

        char key_buffer[50];
        char value_buffer[50];
        while (fscanf(info_file, "%49[^:]:%49[^\n]\n", key_buffer, value_buffer) == 2) 
        {
            if (strcmp(key, key_buffer) == 0) 
            {
                size_t current_length = strlen(result);
                size_t additional_length = strlen(value_buffer) + 50; 

                char *temp = realloc(result, current_length + additional_length);
                if (temp == NULL) 
                {
                    perror("[ERROR] Memory reallocation failed");
                    free(result);
                    fclose(info_file);
                    fclose(ids_file);
                    return NULL;
                }
                result = temp;

                sprintf(result + current_length, "%s\n", value_buffer);

            }
        }
        fclose(info_file);
    }
    fclose(ids_file);

    // If no matches were found, return NULL
    if (strlen(result) == 0) 
    {
        free(result);
        return NULL;
    }

    return result;
}

char *read_pkg(int event_id, int pkg_id, char key[50]) 
{
    if (valid_typeevent_id(event_id) != 1) {
        return NULL;
    }

    char type_dir[256];   
    // data/events/id/type_info,txt
    sprintf(type_dir, "%s%d/%d.txt", TYPE_EVENTS_DIR, event_id, pkg_id);

    FILE *info_file = fopen(type_dir, "r");

    if (info_file == NULL) 
    {
        return NULL;
    }

    char buffer_value[50];
    char key_buffer[50];

    while (!feof(info_file)) 
    {
        if (fscanf(info_file, "%49[^:]:%49[^\n]\n", key_buffer, buffer_value) == 2) 
        {
            if (strcmp(key, key_buffer) == 0) 
            {
                char *result = malloc(strlen(buffer_value) + 1);
                if (result) 
                {
                    strcpy(result, buffer_value);
                }
                fclose(info_file);
                return result;
            }
        }
    }
    fclose(info_file);
    return NULL;
}

int update_pkg(int event_id, int pkg_id, char key[50], char *value) 
{
    if (valid_typeevent_id(event_id) != 1) 
    {
        perror("Invalid event_type id");
        return 0;
    }

    char type_dir[256];   
    // data/events/id/type_info,txt
    sprintf(type_dir, "%s%d/%d.txt", TYPE_EVENTS_DIR, event_id, pkg_id);

    FILE *info_file = fopen(type_dir, "r");

    if (info_file == NULL) 
    {
        perror("Error Opening type_info.txt");
        return 0;
    }

    char temp_path[50];

    sprintf(temp_path, "%s%d/temp_file.txt", TYPE_EVENTS_DIR, event_id);

    FILE *temp_file = fopen(temp_path, "w");

    if(temp_file == NULL)
    {
        fclose(info_file);
        return 0;
    }

    char buffer_value[50];
    char key_buffer[50];

    while (fscanf(info_file, "%49[^:]:%49[^\n]\n", key_buffer, buffer_value) == 2) 
    {
        if (strcmp(key, key_buffer) == 0) 
        {   
            if(value != buffer_value)
            {
                fprintf(temp_file, "%s:%s\n", key, value); 
            }
        } else 
        {
            fprintf(temp_file, "%s:%s\n", key_buffer, buffer_value);
        }
    }

    fclose(info_file);
    fclose(temp_file);

    if (remove(type_dir) != 0 || rename(temp_path, type_dir) != 0) 
    {
        perror("Error updating type_info.txt");
        return 0;
    }
    return 1;
}

int delete_pkg(int event_id, int pkg_id) 
{
    if (valid_pkg_id(event_id, pkg_id) != 1) 
    {
        perror("[ERROR] Invalid package ID");
        return -1;
    }

    if (valid_typeevent_id(event_id) != 1) 
    {
        perror("[ERROR] Invalid event type ID");
        return -1;
    }

    char type_dir[256];
    char path[256];

    // Construct path to the package info file
    sprintf(path, "data/events/%d/%s", event_id, TYPE_PKG_ID_FILE);

    FILE *file = fopen(path, "r");
    if (file == NULL) 
    {
        perror("[ERROR] Error opening package ID file");
        return -1;
    }

    // Temporary file for writing updated data
    char temp_path[256];
    sprintf(temp_path, "data/events/%d/%s_temp", event_id, TYPE_PKG_ID_FILE);

    FILE *temp_file = fopen(temp_path, "w");
    if (temp_file == NULL) 
    {
        perror("[ERROR] Error opening temporary file for writing");
        fclose(file);
        return -1;
    }

    int buffer_id;
    int is_found = 0;

    // Read from original file and write to temp file, excluding `pkg_id`
    while (fscanf(file, "%d\n", &buffer_id) == 1) 
    {
        if (buffer_id != pkg_id) 
        {
            fprintf(temp_file, "%d\n", buffer_id);
        } 
        else 
        {
            is_found = 1;
        }
    }

    fclose(file);
    fclose(temp_file);

    if (!is_found) 
    {
        perror("[ERROR] Package ID not found");
        remove(temp_path); // Clean up temp file
        return -1;
    }

    // Replace original file with the updated temp file
    if (remove(path) != 0 || rename(temp_path, path) != 0) 
    {
        perror("[ERROR] Failed to update package ID file");
        return -1;
    }

    // Delete package information file
    sprintf(type_dir, "%s%d/%d.txt", TYPE_EVENTS_DIR, event_id, pkg_id);
    if (remove(type_dir) != 0) 
    {
        perror("[ERROR] Failed to remove package info file");
        return -1;
    }
    return 1;
}

int generate_pkg_id(int event_id)
{
    char path[50];

    sprintf(path, "data/events/%d/%s", event_id, TYPE_PKG_ID_FILE);

    FILE *file = fopen(path, "r+"); 
    int last_event_id = 0;

    if (file == NULL) 
    {
        file = fopen(path, "w");
        if (file == NULL) 
        {
            perror("Error creating EVENT_ID file");
            return -1;
        }
        last_event_id = 1;
    } else 
    {
        fscanf(file, "%d\n", &last_event_id);
        last_event_id++;
        rewind(file);
    }

    fclose(file);

    return last_event_id;
}

int valid_pkg_id(int event_id, int pkg_id) 
{
    char path[50];

    sprintf(path, "data/events/%d/%s", event_id, TYPE_PKG_ID_FILE);

    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        perror("Error opening type event_id file");
        return 0;
    }

    char buffer_value[MAX_LINE];

    while (fgets(buffer_value, MAX_LINE, file)) 
    {
        if (atoi(buffer_value) == pkg_id) 
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}
