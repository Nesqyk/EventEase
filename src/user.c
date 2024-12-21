#include <stdio.h>
#include <direct.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <dirent.h>
#include <ctype.h>
#include <locale.h>
#include <strings.h>
#include <conio.h> 

#include "utils.h"
#include "users.h"

// Book Event

/*
User Addition: When adding a new user, you’d add their ID to user_ids.txt and create a corresponding folder.
User Removal: For user removal, delete the corresponding folder and remove the user’s ID from user_ids.txt.
*/

// upcoming events <- no need to create for this one
// notifications.txt
// notification folder??
// 12.txt
// 


int login_user(char user_input[50], const char password[50]) {
    FILE *ids_file = fopen("data/users_id.txt", "r");
    if (ids_file == NULL) {
        perror("[ERROR] Unable to open users_id.txt");
        return -1;
    }

    char dir_name[256];
    char info_filename[256];
    int id;
    int login_success = 0;

    while (fscanf(ids_file, "%d:%*s\n", &id) != EOF) {

        sprintf(dir_name, "%s%d", USERS_DIR, id);
        sprintf(info_filename, "%s/%s", dir_name, USER_INFO_FILE);

        FILE *info_file = fopen(info_filename, "r");
        if (info_file == NULL) {
            perror("[ERROR] Unable to open user_info.txt");
            fclose(ids_file);
            return -1;
        }

        // Read and validate credentials
        char file_username[50];
        char file_password[50];
        char buffer_key[50], buffer_value[256];
        int file_id = id;

        while (fscanf(info_file, "%49[^:]:%255[^\n]\n", buffer_key, buffer_value) == 2) 
        {
            if (strcmp(buffer_key, "username") == 0) 
            {
                strcpy(file_username, buffer_value);
            } else if (strcmp(buffer_key, "password") == 0) 
            {
                strcpy(file_password, buffer_value);
            }
        }
        fclose(info_file);

        // Match username and password
        if ((is_numeric(user_input) && atoi(user_input) == file_id) || (strcmp(user_input, file_username) == 0)) 
        {
            if(strcmp(password, file_password) == 0)
            {
                login_success = 1;

                FILE *last_login  = fopen("data/last_login.txt", "w");
                if(last_login == NULL)
                {
                    perror("[ERROR] Unable to open last_login.txt");
                }

                time_t g_t;
                time(&g_t);

                char *generated_date = ctime(&g_t);

                fprintf(last_login, "id:%d\n", id);
                fprintf(last_login, "username:%s\n", file_username);
                fprintf(last_login, "signed_in:%d\n", 0);
                fprintf(last_login, "role:%s\n", read_user(id, "role"));
                fprintf(last_login, "date_logged_on:%s\n", generated_date);

                fclose(last_login);

                break;
            } else
            {
                printf("[WARNING] Invalid Password. Please Try Again\n");
                break;
            }
        }
    }
    fclose(ids_file);
    if (!login_success) 
    {
        return 0;
    }
    return 1;
}

int force_login(int id, const char username[50]) {
    char dir_name[256];
    char info_filename[256];
    FILE *info_file;
    
    sprintf(dir_name, "%s%d", USERS_DIR, id);
    sprintf(info_filename, "%s/%s", dir_name, USER_INFO_FILE);

    info_file = fopen(info_filename, "r+");  // r+ mode to read and write
    if (info_file == NULL) {
        perror("[ERROR] Unable to open user_info.txt");
        return -1;  // Error opening file
    }

    char buffer_key[50], buffer_value[256];
    char file_username[50];
    int found = 0;

    while (fscanf(info_file, "%49[^:]:%255[^\n]\n", buffer_key, buffer_value) == 2) 
    {
        if (strcmp(buffer_key, "username") == 0) 
        {
            strcpy(file_username, buffer_value);
            if (strcmp(file_username, username) == 0) 
            {
                found = 1;
                break;
            }
        }
    }

    if (!found)
    {
        printf("[ERROR] User not found.\n");
        fclose(info_file);
        return -1;  // User not found
    }

    update_user(id, "signed_in", "1");
    fclose(info_file);
    // butang ang dashboard dire.
    return 1;  // Success
}

// check if username exist already.
int register_user(User user) 
{

    if(username_exist(user.username) == 0)
    {
        return 0;
    }

    char dir_name[256];   
    // ../data/users/id
    sprintf(dir_name, "%s%d", USERS_DIR, user.id);

    printf("%s\n", dir_name);

    if (_mkdir(dir_name) != 0) 
    { 
        perror("[ERROR] Failed to create user directory");
        return -1;
    }

    char info_filename[256];   
    // data/users/id/user_info.txt
    sprintf(info_filename, "%s/%s", dir_name, USER_INFO_FILE);

    printf("%s\n", info_filename);


    FILE *info_file = fopen(info_filename, "w");

    if (info_file == NULL) 
    {
        perror("[ERROR] Opening info_file for writing");
        return -1;
    }

    FILE *ids_file = fopen("data/users_id.txt", "a");  

    if (ids_file == NULL) 
    {
        perror("[ERROR] Error opening file for writing");
        return -1;
    }
    

    time_t g_t;
    time(&g_t);

    char *generated_date = ctime(&g_t);

    fprintf(info_file, "id:%d\n", user.id);
    fprintf(info_file, "username:%s\n", user.username);
    fprintf(info_file, "password:%s\n", user.password);
    fprintf(info_file, "full_name:%s\n", user.full_name);
    fprintf(info_file, "email:%s\n", user.email);
    fprintf(info_file, "phone:%s\n", user.phone);
    fprintf(info_file, "role:%s\n", user.role);
    fprintf(info_file, "signed_in:%d\n", user.signed_in);  // %d for signed_in
    fprintf(info_file, "date_created:%s\n", generated_date);
    fclose(info_file);

    fprintf(ids_file, "%d:%s\n", user.id, user.username);
    fclose(ids_file);

    char event_dir[256];   
    // ../data/users/id/events
    sprintf(event_dir, "%s/events", dir_name);

    char book_list_path[50];

    sprintf(book_list_path, "%s/book_event_id.txt", dir_name);

    FILE *book_list_file  = fopen(book_list_path, "w");

    if(book_list_file == NULL)
    {
        return 0;
    }

    if (_mkdir(event_dir) != 0) 
    {
        return 0;
    }

    return 1;
}

//
int delete_user(int id) 
{
    if (valid_user_id(id) != 1) 
    {
        perror("[ERROR] Invalid ID");
        return -1;
    }

    FILE *ids_file = fopen("data/users_id.txt", "r");
    if (ids_file == NULL) 
    {
        perror("[ERROR] Error opening users_id.txt for reading");
        return -1;
    }

    FILE *temp_file = fopen("data/users_id_temp.txt", "w");
    if (temp_file == NULL) 
    {
        perror("[ERROR] Error opening temporary file for writing");
        fclose(ids_file);
        return -1;
    }

    int buffer_id;
    char buffer_username[50];
    int is_found = 0;

    while (fscanf(ids_file, "%d:%s\n", &buffer_id, buffer_username) == 2) 
    {

        if (buffer_id != id) 
        {
            fprintf(temp_file, "%d:%s\n", buffer_id, buffer_username);
        } else 
        {
            is_found = 1;  
        }
    }

    fclose(ids_file);
    fclose(temp_file);

    if (!is_found) 
    {
        perror("[ERROR] ID not found");
        return -1;
    }

    if (remove("data/users_id.txt") != 0 || rename("data/users_id_temp.txt", "data/users_id.txt") != 0) 
    {
        perror("[ERROR] Failed to delete the original users_id.txt");
        return -1;
    }

    char user_dir[256];

    sprintf(user_dir, "data/users/%d", id);
    if (remove_directory(user_dir) != 0) {  
        perror("[ERROR] Failed to remove user's directory");
        return -1;
    }

    return 1;  // Success
}


int update_user(int id, char *key, char *value) 
{

    if (valid_user_id(id) != 1) 
    {
        perror("[ERROR] Invalid ID");
        return 0;
    }

    char info_filename[256];   
    // data/users/id/user_info.txt
    sprintf(info_filename, "%s%d/%s", USERS_DIR, id, USER_INFO_FILE);

    FILE *info_file = fopen(info_filename, "r");

    if (info_file == NULL) 
    {
        perror("Error Opening user_info.txt");
        return 0;
    }

    char temp_filename[50];

    sprintf(temp_filename, "%s%d/temp_file.txt", USERS_DIR, id);

    FILE *temp_file = fopen(temp_filename, "w");

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

    if (remove(info_filename) != 0 || rename(temp_filename, info_filename) != 0) 
    {
        printf("[ERROR] Failed to update %d's %s", id, key);
        return 0;
    }
    return 1;
}


char *read_user(int id, char key[50]) 
{
    if (valid_user_id(id) != 1)
     {
        return NULL;
    }

    char info_filename[256];   
    // data/users/id/user_info.txt
    sprintf(info_filename, "%s%d/%s", USERS_DIR, id, USER_INFO_FILE);

    FILE *info_file = fopen(info_filename, "r");

    if (info_file == NULL) 
    {
        return NULL;
    }

    char buffer_value[50];
    char key_buffer[50];

    while (!feof(info_file)) {
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



char *read_all_user_by_key(const char key[50]) 
{
    FILE *ids_file = fopen("data/users_id.txt", "r");
    if (ids_file == NULL) 
    {
        perror("[ERROR] Unable to open users_id.txt");
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
    while (fscanf(ids_file, "%d:%*s\n", &id) != EOF) 
    {
        char info_filename[256];

        sprintf(info_filename, "%s%d/%s", USERS_DIR, id, USER_INFO_FILE);

        FILE *info_file = fopen(info_filename, "r");
        if (info_file == NULL) 
        {
            perror("[ERROR] Unable to open user_info.txt");
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


char *read_last_login(char key[50]) 
{

    FILE *info_file = fopen("data/last_login.txt", "r");

    if (info_file == NULL) 
    {
        return NULL;
    }

    char buffer_value[50];
    char key_buffer[50];

    while (!feof(info_file)) {
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

int generate_user_id() 
{
    static int is_seeded = 0;

    if (!is_seeded) 
    {
        srand(time(NULL));
        is_seeded = 1;
    }

    int event_id;

    do 
    {
        event_id = rand() % MAX_ID_RANGE;
    } while (valid_user_id(event_id) == 1);

    return event_id;
}

int valid_user_id(int id) 
{
    FILE *event_file = fopen(USERID_FILE, "r");

    if (event_file == NULL) 
    {
        perror("Error opening user id file");
        return 0;
    }

    int buffer_id;

    while (fscanf(event_file, "%d:%*s\n", &buffer_id) == 1) 
    {
        if (buffer_id == id) 
        {
            fclose(event_file);
            return 1;
        }
    }
    fclose(event_file);
    return 0;
}

int get_id_username(char username[50]) 
{
    FILE *event_file = fopen(USERID_FILE, "r");

    if (event_file == NULL) 
    {
        perror("Error opening user id file");
        return 0;
    }

    char buffer_username[50];
    int buffer_id;

    while (fscanf(event_file, "%d:%s\n", &buffer_id, buffer_username) == 2) 
    {
        if (strcmp(username, buffer_username) == 0) 
        {
            fclose(event_file);
            return buffer_id;
        }
    }
    fclose(event_file);
    printf("[ERROR] Username %s not found.\n", username);
    return -1; // Return -1 if the username was not found
}

int username_exist(char username[50])
{
    FILE *event_file = fopen(USERID_FILE, "r");

    if (event_file == NULL) 
    {
        perror("Err");
    }

    char buffer_username[50];

    while (fscanf(event_file, "%*d:%s\n", buffer_username) == 1) 
    {
        if (strcmp(username, buffer_username) == 0) 
        {
            printf("[ERROR] %s already exist, please try again\n", username);
            return 0; 
        }
    }
    fclose(event_file);
    return 1; // Return -1 if the username was not found
}

/* typedef struct {
    int user_id; // asa nimo ganahan ipa da si notfication 
    int asa_gikan;  // id gihapo nsiya
    int notifcation_id;
    char notifcation_description[100]; // unsay ipadala nimo ni user_id
    int mana; // 0 for mana 1 for wala <- if ever si client kay nahuman niya ang specific or natanaw niya ang specific nga task
} Notifcation;
 */

