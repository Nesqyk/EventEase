#include <stdio.h>
#include <direct.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> 

#include "organizer.h"
#include "utils.h"
#include "users.h"
#include "events.h"


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

int delete_typeevent(int event_id) 
{
    char event_dir[256];
    char type_event_ids_path[256];
    char temp_ids_path[256];

    // Paths for event directory and type_event_ids.txt
    sprintf(event_dir, "data/events/%d/", event_id);
    sprintf(type_event_ids_path, "data/type_event_ids.txt");
    sprintf(temp_ids_path, "data/type_event_ids_temp.txt");

    // Attempt to remove the entire event directory
    char command[300];
    sprintf(command, "rm -rf %s", event_dir);
    if (system(command) != 0) 
    {
        perror("[ERROR] Failed to remove event directory");
        return -1;
    }

    // Remove the event ID from type_event_ids.txt
    FILE *ids_file = fopen(type_event_ids_path, "r");
    if (ids_file == NULL) 
    {
        perror("[ERROR] Unable to open type_event_ids.txt");
        return -1;
    }

    FILE *temp_ids_file = fopen(temp_ids_path, "w");
    if (temp_ids_file == NULL) 
    {
        perror("[ERROR] Unable to create temporary file");
        fclose(ids_file);
        return -1;
    }

    int id;
    while (fscanf(ids_file, "%d\n", &id) == 1) 
    {
        if (id != event_id) 
        {
            fprintf(temp_ids_file, "%d\n", id);
        }
    }

    fclose(ids_file);
    fclose(temp_ids_file);

    // Replace original type_event_ids.txt with updated file
    if (remove(type_event_ids_path) != 0 || rename(temp_ids_path, type_event_ids_path) != 0) 
    {
        perror("[ERROR] Failed to update type_event_ids.txt");
        return -1;
    }

    printf("[SUCCESS] Event ID %d and its associated files have been deleted.\n", event_id);
    return 1;
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
    fprintf(info_file, "dp_percentage:%d\n", typeevent.dp_percentage);
    fprintf(info_file, "payment_deadline_days:%d\n", typeevent.payment_deadline_days);
    fprintf(info_file, "date_created:%s", generated_date);


    FILE *id_file = fopen(TYPE_EVENT_ID_FILE, "a");

    if(id_file == NULL)
    {
        return -1;
    }

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
    if (valid_typeevent_id(event_id) != 1) 
    {
        return NULL; // not this
    }

    char type_dir[256];   
    // data/events/id/type_info,txt
    
    sprintf(type_dir, "%s%d/%s", TYPE_EVENTS_DIR, event_id, TYPE_INFO_FILE);

    FILE *info_file = fopen(type_dir, "r");

    if (info_file == NULL) 
    {
        return NULL;
    }

    char buffer_value[256];
    char key_buffer[256];

    while (!feof(info_file)) 
    {
        if (fscanf(info_file, "%256[^:]:%256[^\n]\n", key_buffer, buffer_value) == 2) 
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


#define MAX_LINE_LENGTH 50



void wrap_text(char *dest, const char *src, int max_width) {
    int len = strlen(src);
    int pos = 0;

    while (len > 0) {
        int segment_length = (len > max_width) ? max_width : len;

        // Find the last space in the current segment
        int end_pos = segment_length;
        for (int i = segment_length - 1; i > 0; i--) {
            if (src[pos + i] == ' ') {
                end_pos = i + 1;
                break;
            }
        }

        strncat(dest, src + pos, end_pos);
        strcat(dest,"\t");
        strcat(dest, "\n");
        pos += end_pos;
        len -= end_pos;
    }
}

char *preview_event_type() {
    FILE *ids_file = fopen("data/type_event_ids.txt", "r");
    if (ids_file == NULL) {
        perror("[ERROR] Unable to open type_event_ids.txt");
        return strdup(
            "==================================================\n"
            "              📭 No Event Types Found            \n"
            "==================================================\n\n"
            "      No event types are currently recorded.\n"
            "==================================================\n");
    }

    char *result = malloc(16384); // Adjust size for expected data
    if (result == NULL) {
        perror("[ERROR] Memory allocation failed");
        fclose(ids_file);
        return NULL;
    }

    snprintf(result, 16384,
             "==================================================\n"
             "               📅 Available Event Types           \n"
             "==================================================\n");

    int id;
    char line[256];
    while (fscanf(ids_file, "%d\n", &id) != EOF) {
        char info_filename[256];
        sprintf(info_filename, "data/events/%d/type_info.txt", id);

        FILE *info_file = fopen(info_filename, "r");
        if (info_file == NULL) {
            perror("[ERROR] Unable to open type_info.txt");
            continue;
        }

        char key_buffer[50];
        char value_buffer[256];

        char event_name[100] = "Unknown Event";
        char description[500] = "No description available.";
        char venues[200] = "Not specified.";

        while (fscanf(info_file, "%49[^:]:%255[^\n]\n", key_buffer, value_buffer) == 2) {
            if (strcmp(key_buffer, "event_name") == 0) {
                strncpy(event_name, value_buffer, sizeof(event_name) - 1);
            } else if (strcmp(key_buffer, "description") == 0) {
                strncpy(description, value_buffer, sizeof(description) - 1);
            } else if (strcmp(key_buffer, "venues") == 0) {
                strncpy(venues, value_buffer, sizeof(venues) - 1);
            }
        }

        fclose(info_file);

        // Format event details
        char formatted_event[1024];
        snprintf(formatted_event, sizeof(formatted_event),
                 "\n🆔 Event ID        : %d\n"
                 "🎉 Event Name      : %s\n"
                 "📜 Description     : %s\n"
                 "🌆 Venues          : %s\n"
                 "\n--------------------------------------------------\n",
                 id, event_name, description, venues);

        // Check for line length and wrap if needed
        size_t desc_length = strlen(description);
        if (desc_length > 50) {
            char wrapped_description[500] = "";
            size_t pos = 0;
            while (pos < desc_length) {
                strncat(wrapped_description, &description[pos], 50);
                strcat(wrapped_description, "\n                     ");
                pos += 50;
            }
            snprintf(formatted_event, sizeof(formatted_event),
                     "\n🆔 Event ID        : %d\n"
                     "🎉 Event Name      : %s\n"
                     "📜 Description     : %s\n"
                     "🌆 Venues          : %s\n"
                     "--------------------------------------------------\n",
                     id, event_name, wrapped_description, venues);
        }

        strcat(result, formatted_event);
    }

    fclose(ids_file);

    if (strlen(result) == strlen(
            "==================================================\n"
            "               📅 Available Event Types           \n"
            "==================================================\n")) {
        free(result);
        return strdup(
            "==================================================\n"
            "              📭 No Event Types Found            \n"
            "==================================================\n\n"
            "      No event types are currently recorded.\n"
            "==================================================\n");
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
    FILE *file = fopen(TYPE_EVENT_ID_FILE, "a+"); 
    if (file == NULL) 
    {
        perror("Error opening TYPE_EVENT_ID file");
        return -1;
    }

    int id;
    int unique = 0;
    srand(time(NULL)); 

    while (!unique) 
    {
        
        id = rand() % 9000 + 1000; 

        rewind(file); 
        int existing_id;
        unique = 1; 

        while (fscanf(file, "%d\n", &existing_id) == 1) 
        {
            if (existing_id == id) 
            {
                unique = 0; // ID is not unique
                break;
            }
        }
    }

    fprintf(file, "%d\n", id);
    fclose(file); // Close the file
    return id;
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

#define PAGE_SIZE 10

void display_packages_with_details(const char *packages, char choice[20]) 
{
    
   
}


int add_pkg(int event_id, Package pkg)
{
    char pkg_dir[256];
    sprintf(pkg_dir, "%s%d/packages/", TYPE_EVENTS_DIR, event_id);

    #ifdef _WIN32
    _mkdir(pkg_dir); // Create directory in Windows
    #else
    mkdir(pkg_dir, 0777); // Create directory in Linux/Unix
    #endif

    char pkg_path[256];
    sprintf(pkg_path, "%s/%d.txt", pkg_dir, pkg.id);

    FILE *pkg_file = create_file(pkg_path);
    if (pkg_file == NULL) {
        perror("[ERROR] Failed to create the package file");
        return -1; // Indicate failure
    }

    time_t g_t;
    time(&g_t);

    char *generated_date = ctime(&g_t);

    fprintf(pkg_file,
        "package_id:%d\n"
        "package_name:%s\n"
        "price:%.2f\n"
        "event_type:%d\n"
        "description:%s\n"
        "availability:%d\n"
        "max_guest:%d\n"
        "duration:%d\n"
        "inclusions:%s\n"
        "created_date:%s\n",
        pkg.id,
        pkg.package_name,
        pkg.price,
        pkg.event_type,
        pkg.description,
        pkg.availability, // Ensure this is properly initialized
        pkg.max_guest,
        pkg.duration,
        pkg.inclusions,
        generated_date
    );
    fclose(pkg_file);

    char id_path[50];
    sprintf(id_path, "%s%d/%s", TYPE_EVENTS_DIR, event_id, TYPE_PKG_ID_FILE);

    FILE *id_file = fopen(id_path, "a"); // Open in append mode
    if (id_file == NULL) {
        perror("[ERROR] Failed to open ID file");
        return -1; // Indicate failure
    }
    fclose(id_file);

    return 1; // Indicate success
}

char *preview_single_pkg(int event_id, int pkg_id) {
    char event_dir[50];
    sprintf(event_dir, "data/events/%d/", event_id);

    char info_filename[256];
    sprintf(info_filename, "%spackages/%d.txt", event_dir, pkg_id);

    FILE *info_file = fopen(info_filename, "r");
    if (info_file == NULL) {
        perror("[ERROR] Unable to open package file");
        return NULL;
    }

    char *result = malloc(1);
    if (result == NULL) {
        perror("[ERROR] Memory allocation failed");
        fclose(info_file);
        return NULL;
    }

    result[0] = '\0';

    char key_buffer[50];
    char value_buffer[1000];

    Field package_field[] = {
        {"package_id", "\033[32mPackage ID\033[0m"},
        {"package_name", "📛 Package Name"},
        {"price", "💰 Price"},
        {"event_type", "🏷️ Event Type"},
        {"description", "📜 Description"},
        {"availability", "📅 Availability"},
        {"max_guest", "🧑‍🤝‍🧑 Max Guest"},
        {"duration", "⏱️ Duration"},
        {"inclusions", "🎁 Inclusions"}};

    while (fscanf(info_file, "%49[^:]:%1000[^\n]\n", key_buffer, value_buffer) == 2) {
        if (strcmp(key_buffer, "created_date") == 0) {
            continue;
        }

        if (strcmp(key_buffer, "inclusions") == 0) {
            // Format inclusions with bullet points
            char *token = strtok(value_buffer, ",");
            strcat(result, "🎁 Inclusions:\n");
            while (token) {
                size_t additional_length = strlen(token) + 4; // For "  - " and newline
                char *temp = realloc(result, strlen(result) + additional_length + 1);
                if (temp == NULL) {
                    perror("[ERROR] Memory reallocation failed");
                    free(result);
                    fclose(info_file);
                    return NULL;
                }
                result = temp;
                strcat(result, "  - ");
                strcat(result, token);
                strcat(result, "\n");
                token = strtok(NULL, ",");
            }
        } else {
            // Process other fields
            size_t additional_length = strlen(value_buffer) + strlen(key_buffer) + 20;
            char *temp = realloc(result, strlen(result) + additional_length + 1);
            if (temp == NULL) {
                perror("[ERROR] Memory reallocation failed");
                free(result);
                fclose(info_file);
                return NULL;
            }
            result = temp;

            for (size_t i = 0; i < sizeof(package_field) / sizeof(package_field[0]); i++) {
                if (strcmp(key_buffer, package_field[i].key) == 0) {
                    sprintf(result + strlen(result), "%s: %s\n", package_field[i].display_name, value_buffer);
                    break;
                }
            }
        }
    }

    fclose(info_file);

    // Remove trailing newline if present
    size_t result_length = strlen(result);
    if (result_length > 0 && result[result_length - 1] == '\n') {
        result[result_length - 1] = '\0';
    }

    if (strlen(result) == 0) {
        free(result);
        return NULL;
    }

    return result;
}
char *preview_pkgs(int event_id) {
    char event_dir[50];
    sprintf(event_dir, "data/events/%d/", event_id);

    char pkg_id_file[50];
    sprintf(pkg_id_file, "%s%s", event_dir, TYPE_PKG_ID_FILE);

    FILE *ids_file = fopen(pkg_id_file, "r");
    if (ids_file == NULL) {
        return strdup(
            "==================================================\n"
            "              📭 No Packages Found               \n"
            "==================================================\n\n"
            "      This event currently has no packages.\n"
            "==================================================\n");
    }

    char *result = malloc(1);
    if (result == NULL) {
        perror("[ERROR] Memory allocation failed");
        fclose(ids_file);
        return strdup(
            "==================================================\n"
            "              📭 No Packages Found               \n"
            "==================================================\n\n"
            "      This event currently has no packages.\n"
            "==================================================\n");
    }

    result[0] = '\0'; // Initialize result as an empty string
    int id;
    int packages_found = 0;

    while (fscanf(ids_file, "%d\n", &id) != EOF) {
        char info_filename[256];
        sprintf(info_filename, "%spackages/%d.txt", event_dir, id);

        FILE *info_file = fopen(info_filename, "r");
        if (info_file == NULL) {
            perror("[ERROR] Unable to open package file");
            continue;
        }

        packages_found = 1; // Mark that we found at least one package

        char key_buffer[50];
        char value_buffer[1000];

        Field package_field[] = {
            {"package_id", "\033[32mPackage ID\033[0m"},
            {"package_name", "📛 Package Name"},
            {"price", "💰 Price"},
            {"event_type", "🏷️ Event Type"},
            {"description", "📜 Description"},
            {"availability", "📅 Availability"},
            {"max_guest", "🧑‍🤝‍🧑 Max Guest"},
            {"duration", "⏱️ Duration"},
            {"inclusions", "🎁 Inclusions"}};

        // Separate packages with a blank line
        size_t current_length = strlen(result);
        char *temp = realloc(result, current_length + 2);
        if (temp == NULL) {
            perror("[ERROR] Memory reallocation failed");
            free(result);
            fclose(info_file);
            fclose(ids_file);
            return strdup(
                "==================================================\n"
                "              📭 No Packages Found               \n"
                "==================================================\n\n"
                "      This event currently has no packages.\n"
                "==================================================\n");
        }
        result = temp;
        strcat(result, "\n");

        while (fscanf(info_file, "%49[^:]:%1000[^\n]\n", key_buffer, value_buffer) == 2) {
            if (strcmp(key_buffer, "created_date") == 0) {
                continue;
            }

            if (strcmp(key_buffer, "inclusions") == 0) {
                // Format inclusions with bullet points
                char *token = strtok(value_buffer, ",");
                strcat(result, "🎁 Inclusions:\n");
                while (token) {
                    size_t additional_length = strlen(token) + 6; // For "  - " and newline
                    temp = realloc(result, strlen(result) + additional_length + 1);
                    if (temp == NULL) {
                        perror("[ERROR] Memory reallocation failed");
                        free(result);
                        fclose(info_file);
                        fclose(ids_file);
                        return strdup(
                            "==================================================\n"
                            "              📭 No Packages Found               \n"
                            "==================================================\n\n"
                            "      This event currently has no packages.\n"
                            "==================================================\n");
                    }
                    result = temp;
                    strcat(result, "  - ");
                    strcat(result, token);
                    strcat(result, "\n");
                    token = strtok(NULL, ",");
                }
            } else {
                // Process other fields
                size_t additional_length = strlen(value_buffer) + strlen(key_buffer) + 20;
                temp = realloc(result, strlen(result) + additional_length + 1);
                if (temp == NULL) {
                    perror("[ERROR] Memory reallocation failed");
                    free(result);
                    fclose(info_file);
                    fclose(ids_file);
                    return strdup(
                        "==================================================\n"
                        "              📭 No Packages Found               \n"
                        "==================================================\n\n"
                        "      This event currently has no packages.\n"
                        "==================================================\n");
                }
                result = temp;

                for (size_t i = 0; i < sizeof(package_field) / sizeof(package_field[0]); i++) {
                    if (strcmp(key_buffer, package_field[i].key) == 0) {
                        sprintf(result + strlen(result), "%s: %s\n", package_field[i].display_name, value_buffer);
                        break;
                    }
                }
            }
        }

        fclose(info_file);
    }

    fclose(ids_file);

    if (!packages_found) {
        free(result);
        return strdup(
            "==================================================\n"
            "              📭 No Packages Found               \n"
            "==================================================\n\n"
            "      This event currently has no packages.\n"
            "==================================================\n");
    }

    // Remove trailing newline if present
    size_t result_length = strlen(result);
    if (result_length > 0 && result[result_length - 1] == '\n') {
        result[result_length - 1] = '\0';
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
    sprintf(type_dir, "%s%d/packages/%d.txt", TYPE_EVENTS_DIR, event_id, pkg_id);
    FILE *info_file = fopen(type_dir, "r");

    if (info_file == NULL) 
    {
        return NULL;
    }

    char buffer_value[50];
    char key_buffer[50];

    while (!feof(info_file)) 
    {
        if (fscanf(info_file, "%49[^:]:%50[^\n]\n", key_buffer, buffer_value) == 2) 
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
    sprintf(type_dir, "%s%d/packages/%d.txt", TYPE_EVENTS_DIR, event_id, pkg_id);
    // printf("%s", type_dir);
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
    sprintf(type_dir, "%s%d/packages/%d.txt", TYPE_EVENTS_DIR, event_id, pkg_id);
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
    int id;
    int unique = 0;
    srand(time(NULL)); 

    while (!unique) 
    {
        
        id = rand() % 9000 + 1000; 

        rewind(file); 
        int existing_id;
        unique = 1; 

        while (fscanf(file, "%d\n", &existing_id) == 1) 
        {
            if (existing_id == id) 
            {
                unique = 0; // ID is not unique
                break;
            }
        }
    }

    fprintf(file, "%d\n", id);
    fclose(file); 

    return id;
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


// dashboard overview

// total events booked functions

// will print the counts for each event_type.

void total_bookings_by_event_type() {

    FILE *event_types_file = fopen("data/type_event_ids.txt", "r");
    if (event_types_file == NULL) {
        perror("[ERROR] Unable to open type_event_ids.txt");
        return;
    }

    // Dynamically store event types and their counts
    char event_names[100][50]; // Maximum 100 event types, each up to 50 characters
    int event_counts[100] = {0};
    int total_event_types = 0;

    int event_type_id;
    while (fscanf(event_types_file, "%d\n", &event_type_id) != EOF) {
        char *event_name = read_typevent(event_type_id, "event_name");
        if (event_name) {
            if (total_event_types < 100) {
                strncpy(event_names[total_event_types], event_name, sizeof(event_names[total_event_types]) - 1);
                event_names[total_event_types][sizeof(event_names[total_event_types]) - 1] = '\0'; // Ensure null-termination
                total_event_types++;
            } else {
                printf("[WARNING] Maximum event types reached. Some types will not be counted.\n");
            }
            free(event_name);
        }
    }
    fclose(event_types_file);

    // Open the user IDs file to read bookings
    FILE *user_ids_file = fopen("data/users_id.txt", "r");
    if (user_ids_file == NULL) {
        perror("[ERROR] Unable to open users_id.txt");
        return;
    }

    int user_id;
    while (fscanf(user_ids_file, "%d:%*s\n", &user_id) != EOF) {
        char user_events_file[256];
        sprintf(user_events_file, "data/users/%d/book_event_id.txt", user_id);

        FILE *events_file = fopen(user_events_file, "r");
        if (events_file == NULL) continue;

        int event_id;
        while (fscanf(events_file, "%d\n", &event_id) != EOF) {
            char *type_event_id_str = read_event(user_id, event_id, "event_type_id");
            if (type_event_id_str) {
                int type_event_id = atoi(type_event_id_str);
                free(type_event_id_str);

                for (int i = 0; i < total_event_types; i++) {
                    char *event_id_str = read_typevent(type_event_id, "event_id");
                    if (event_id_str) {
                        int stored_event_id = atoi(event_id_str);
                        free(event_id_str);

                        if (type_event_id == stored_event_id) {
                            event_counts[i]++;
                            break;
                        }
                    }
                }
            }
        }
        fclose(events_file);
    }
    fclose(user_ids_file);

    // Print the report
    printf("==================================================\n");
    printf("           📊 Total Bookings by Event Type       \n");
    printf("==================================================\n");
    for (int i = 0; i < total_event_types; i++) {
        printf("%-20s: %d bookings\n", event_names[i], event_counts[i]);
    }
    printf("==================================================\n");
    printf("\nPlease enter [ANY KEY] to stop viewing.");
}


// total_bookings by venue will print how many booked events per venue.



void total_bookings_by_venue() {
    struct VenueCount {
        char venue[100];
        int count;
    } venue_counts[100];
    int venue_count_size = 0;

    FILE *user_ids_file = fopen("data/users_id.txt", "r");
    if (user_ids_file == NULL) {
        perror("[ERROR] Unable to open users_id.txt");
        return;
    }

    int user_id;
    while (fscanf(user_ids_file, "%d:%*s\n", &user_id) != EOF) {
        char user_events_file[256];
        sprintf(user_events_file, "data/users/%d/book_event_id.txt", user_id);

        FILE *events_file = fopen(user_events_file, "r");
        if (events_file == NULL) continue;

        int event_id;
        while (fscanf(events_file, "%d\n", &event_id) != EOF) {
            char *venue = read_event(user_id, event_id, "venue");

            int found = 0;
            for (int i = 0; i < venue_count_size; i++) {
                if (strcmp(venue_counts[i].venue, venue) == 0) {
                    venue_counts[i].count++;
                    found = 1;
                    break;
                }
            }
            if (!found && venue_count_size < 100) {
                strncpy(venue_counts[venue_count_size].venue, venue, sizeof(venue_counts[venue_count_size].venue));
                venue_counts[venue_count_size].count = 1;
                venue_count_size++;
            }
            free(venue);
        }
        fclose(events_file);
    }
    fclose(user_ids_file);

    printf("==================================================\n");
    printf("             🏢 Bookings by Venue                \n");
    printf("==================================================\n");
    for (int i = 0; i < venue_count_size; i++) {
        printf("%-30s: %d bookings\n", venue_counts[i].venue, venue_counts[i].count);
    }
    printf("==================================================\n");
        printf("\nPlease enter [ANY KEY] to stop viewing.");

}

// will count based on months.
void total_bookings_by_month() {
    const char *months[] = {"January", "February", "March", "April", "May", "June",
                            "July", "August", "September", "October", "November", "December"};
    int bookings_per_month[12] = {0}; // Array to store booking counts for each month

    FILE *user_ids_file = fopen("data/users_id.txt", "r");
    if (user_ids_file == NULL) {
        perror("[ERROR] Unable to open users_id.txt");
        return;
    }

    int user_id;
    while (fscanf(user_ids_file, "%d:%*s\n", &user_id) != EOF) {
        char user_events_file[256];
        sprintf(user_events_file, "data/users/%d/book_event_id.txt", user_id);

        FILE *events_file = fopen(user_events_file, "r");
        if (events_file == NULL) continue;

        int event_id;
        while (fscanf(events_file, "%d\n", &event_id) != EOF) {
            char *booking_date = read_event(user_id, event_id, "booking_date");
            if (booking_date) {
                char month_str[4];
                int month_index = -1;

                // Extract the abbreviated month (e.g., "Jan", "Feb") from the booking date
                if (sscanf(booking_date, "%*s %3s", month_str) == 1) {
                    // Match the abbreviated month to find the index
                    for (int i = 0; i < 12; i++) {
                        if (strncmp(month_str, months[i], 3) == 0) {
                            month_index = i;
                            break;
                        }
                    }
                    if (month_index != -1) {
                        bookings_per_month[month_index]++; // Increment the count for the month
                    }
                }
                free(booking_date);
            }
        }
        fclose(events_file);
    }
    fclose(user_ids_file);

    // Print the report
    printf("==================================================\n");
    printf("              📅 Total Bookings by Month          \n");
    printf("==================================================\n");
    for (int i = 0; i < 12; i++) {
        printf("%-10s: %d bookings\n", months[i], bookings_per_month[i]);
    }
    printf("==================================================\n");
    printf("\nPlease enter [ANY KEY] to stop viewing.\n");
}


// exports report.

void generate_event_report(int organizer_id) 
{
    // Locate the HOME directory
    #ifdef _WIN32
    const char *home = getenv("USERPROFILE"); // Primary for Windows
        if (home == NULL) {
            const char *drive = getenv("HOMEDRIVE");
            const char *path = getenv("HOMEPATH");
            static char fallback[512];
            if (drive && path) {
                snprintf(fallback, sizeof(fallback), "%s%s", drive, path);
                home = fallback;
            }
        }
    #else
        const char *home = getenv("HOME"); // Primary for Linux/MacOS
    #endif

    printf("%s\n", home);
    if (home == NULL) {
        perror("[ERROR] Unable to locate the home directory");
        return;
    }

    // Construct the path for the reports folder
    char reports_folder[512];
    snprintf(reports_folder, sizeof(reports_folder), "%s/Downloads/event-ease/reports", home);

    // Create the directory if it doesn't exist
#ifdef _WIN32
    _mkdir(reports_folder);
#else
    mkdir(reports_folder, 0777);
#endif

    // Generate the filename with the current timestamp
    char report_filename[512];
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    snprintf(report_filename, sizeof(report_filename), "%s/events_report_%04d%02d%02d_%02d%02d%02d.csv",
             reports_folder,
             local_time->tm_year + 1900,
             local_time->tm_mon + 1,
             local_time->tm_mday,
             local_time->tm_hour,
             local_time->tm_min,
             local_time->tm_sec);

    FILE *report_file = fopen(report_filename, "w");
    if (report_file == NULL) {
        perror("[ERROR] Failed to create report file");
        return;
    }

    fprintf(report_file, "EventEase Report\n");
    fprintf(report_file, "Generated on: %04d-%02d-%02d %02d:%02d:%02d\n\n",
            local_time->tm_year + 1900,
            local_time->tm_mon + 1,
            local_time->tm_mday,
            local_time->tm_hour,
            local_time->tm_min,
            local_time->tm_sec);

    // Read event types dynamically
    FILE *event_types_file = fopen("data/type_event_ids.txt", "r");
    if (event_types_file == NULL) {
        perror("[ERROR] Unable to read event types");
        fclose(report_file);
        return;
    }

    fprintf(report_file, "Events by Type\n");
    fprintf(report_file, "Event Type, Total Booked\n");

    char type_line[256];
    while (fgets(type_line, sizeof(type_line), event_types_file)) {
        int type_id;
        char type_name[100] = "Unknown Type";
        sscanf(type_line, "%d\n", &type_id);

        // Read the event type name
        char *type_name_read = read_typevent(type_id, "event_name");
        if (type_name_read != NULL) {
            strncpy(type_name, type_name_read, sizeof(type_name));
            free(type_name_read);
        }

        // Count events of this type
        int count = 0;
        FILE *users_file = fopen("data/users_id.txt", "r");
        if (users_file != NULL) {
            int user_id;
            while (fscanf(users_file, "%d:%*s\n", &user_id) != EOF) {
                char events_path[256];
                snprintf(events_path, sizeof(events_path), "data/users/%d/book_event_id.txt", user_id);
                FILE *events_file = fopen(events_path, "r");
                if (events_file != NULL) {
                    int event_id;
                    while (fscanf(events_file, "%d\n", &event_id) != EOF) {
                        char *type_id_read = read_event(user_id, event_id, "event_type_id");
                        if (type_id_read != NULL && atoi(type_id_read) == type_id) {
                            count++;
                            free(type_id_read);
                        }
                    }
                    fclose(events_file);
                }
            }
            fclose(users_file);
        }

        fprintf(report_file, "%s, %d\n", type_name, count);
    }
    fclose(event_types_file);

    fprintf(report_file, "\nEvents by Venue\n");
    fprintf(report_file, "Venue, Total Booked\n");

    // Read venues dynamically
    FILE *venues_file = fopen("data/type_event_ids.txt", "r");
    if (venues_file != NULL) 
    {
        char venue_line[256];
        while (fgets(venue_line, sizeof(venue_line), venues_file)) 
        {
            int venue_id;
            char venue_name[100] = "Unknown Venue";
            sscanf(venue_line, "%d\n", &venue_id);

            char *venue_name_read = read_typevent(venue_id, "venues");
            if (venue_name_read != NULL) {
                strncpy(venue_name, venue_name_read, sizeof(venue_name));
                free(venue_name_read);
            }

            fprintf(report_file, "%s, %d\n", venue_name, 0); // Add counting logic here if required
        }
        fclose(venues_file);
    }

    fclose(report_file);

    printf("==================================================\n");
    printf("         📤 Event Report Generated Successfully\n");
    printf("==================================================\n");
    printf("📁 Report Location: %s\n", report_filename);
    printf("==================================================\n");
    printf("Please enter [ANY KEY] to go back...");
}
