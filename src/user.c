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

#include <windows.h>
#include <unistd.h>

#include "utils.h"
#include "users.h"
#include "organizer.h"
#include "events.h"

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


#define MAX_INPUT_SIZE 256
#define USER_INFO_FILE "user_info.txt"
#define USERID_FILE "data/users_id.txt"
#define USERS_DIR "data/users/"
#define MAX_ID_RANGE 10000

int login_user(char user_input[50], const char password[50]) 
{
    
    FILE *ids_file = fopen(USERID_FILE, "r");
    if (!ids_file) 
    {
        perror("[ERROR] Unable to open users_id.txt");
        return -1;
    }

    char dir_name[MAX_INPUT_SIZE], info_filename[MAX_INPUT_SIZE], buffer_key[50], buffer_value[256];
    char file_username[50], file_password[50];
    int id, login_success = 0;

    // date/users/id
    sprintf(dir_name, "%s%s", USERS_DIR, user_input);

    // data/users/id/
    sprintf(info_filename, "%s/%s", dir_name, USER_INFO_FILE);

    FILE *info_file = fopen(info_filename, "r");

    if (info_file == NULL) 
    {
        perror("[ERROR] Unable to open user_info.txt");
        fclose(ids_file);
        return -1;
    }

    while (fscanf(ids_file, "%d:%*s\n", &id) != EOF) 
    {

        while (fscanf(info_file, "%49[^:]:%255[^\n]\n", buffer_key, buffer_value) == 2) 
        {
            if (strcmp(buffer_key, "username") == 0) 
            {
                strcpy(file_username, buffer_value);
            } 
            else if (strcmp(buffer_key, "password") == 0) 
            {
                strcpy(file_password, buffer_value);
            }
        }
        fclose(info_file);

        if ((is_numeric(user_input) && atoi(user_input) == id) || strcmp(user_input, file_username) == 0) 
        {
            if (strcmp(password, file_password) == 0) 
            {
                login_success = 1;

                // store in user's directory?
                FILE *last_login = fopen("data/last_login.txt", "w");
                if (last_login) 
                {
                    time_t g_t = time(NULL);
                    char *generated_date = ctime(&g_t);

                    fprintf(last_login, "id:%d\nusername:%s\nsigned_in:1\nstay_logged_in:%s\nrole:%s\ndate_logged_on:%s\n",
                            id, file_username, read_user(id,"stay_logged_in"), read_user(id, "role") , generated_date);
                    fclose(last_login);
                    // TODO:
                    /* char downloads_receipt_file[256];
                    sprintf(downloads_receipt_file, "%s/%06d.txt", "data/users/%d/logs/login/%s.txt", generated_date);
                    
                    FILE *downloads_file = fopen(downloads_receipt_file, "w");
                    if (downloads_file == NULL) {
                        perror("[ERROR] Unable to create receipt in Downloads folder");
                        return;
                    }

                    FILE *read_file = fopen(receipt_file_path, "r");
                    if (read_file != NULL) {
                        char buffer[256];
                        while (fgets(buffer, sizeof(buffer), read_file)) {
                            fputs(buffer, downloads_file);
                        }
                        fclose(read_file);
                    } else {
                        perror("[ERROR] Unable to read receipt file for copying");
                    } */

                    // fclose(downloads_file);
                }
                else 
                {
                    perror("[ERROR] Unable to open last_login.txt");
                }
                break;
            }
            else 
            {
                printf("[WARNING] Invalid Password. Please Try Again.\n");
                break;
            }
        }
    }
    fclose(ids_file);
    return login_success ? 1 : 0;
}

int register_user(User user) 
{
    if (username_exist(user.username)) 
    {
        return 0;
    }

    char dir_name[MAX_INPUT_SIZE], info_filename[MAX_INPUT_SIZE];
    sprintf(dir_name, "%s%d", USERS_DIR, user.id);
    sprintf(info_filename, "%s/%s", dir_name, USER_INFO_FILE);

    if (_mkdir(dir_name) != 0) 
    {
        perror("[ERROR] Failed to create user directory");
        return -1;
    }

    FILE *info_file = fopen(info_filename, "w");
    FILE *ids_file = fopen(USERID_FILE, "a");

    if (!info_file || !ids_file) 
    {
        perror("[ERROR] Failed to open required files for registration");
        if (info_file) fclose(info_file);
        if (ids_file) fclose(ids_file);
        return -1;
    }

    time_t g_t = time(NULL);
    char *generated_date = ctime(&g_t);

    fprintf(info_file, "id:%d\nusername:%s\npassword:%s\nfull_name:%s\nemail:%s\nphone:%s\nrole:%s\nsigned_in:%d\nstay_logged_in:0\ndate_created:%s\n",
            user.id, user.username, user.password, user.full_name, user.email, user.phone, user.role, user.signed_in, generated_date);
    fclose(info_file);

    fprintf(ids_file, "%d:%s\n", user.id, user.username);
    fclose(ids_file);

    char event_dir[MAX_INPUT_SIZE];
    sprintf(event_dir, "%s/events", dir_name);
    _mkdir(event_dir);

    char book_list_path[MAX_INPUT_SIZE];
    sprintf(book_list_path, "%s/book_event_id.txt", dir_name);
    FILE *book_list_file = fopen(book_list_path, "w");
    
    char notification_path[50], reminder_path[50], ticket_path[50], payment_history[50];
    sprintf(notification_path, "%s/notifiations.txt", dir_name);
    sprintf(reminder_path, "%s/reminders.txt", dir_name);
    sprintf(ticket_path, "%s/support_tickets.txt", dir_name);
    sprintf(payment_history, "%s/payment_history.txt", dir_name);


    create_file(notification_path);
    create_file(reminder_path);
    create_file(ticket_path);
    create_file(payment_history);
    
    fclose(book_list_file);

    return 1;
}


// check if username exist already.

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

    while (fscanf(info_file, "%49[^:]:%256[^\n]\n", key_buffer, buffer_value) == 2) 
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

// TODO
int *update_last_login(char key[50], char value[50])
{

    char info_filename[256];   
    // data/users/id/user_info.txt
    sprintf(info_filename, "data/last_login.txt");

    FILE *info_file = fopen(info_filename, "r");

    if (info_file == NULL) 
    {
        perror("Error Opening user_info.txt");
        return 0;
    }

    char temp_filename[50];

    sprintf(temp_filename, "data/last_login_temp.txt");

    FILE *temp_file = fopen(temp_filename, "w");

    if(temp_file == NULL)
    {
        fclose(info_file);
        return 0;
    }

    char buffer_value[50];
    char key_buffer[50];

    while (fscanf(info_file, "%49[^:]:%256[^\n]\n", key_buffer, buffer_value) == 2) 
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
        return 0;
    }
    return 1;
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

void generate_receipt_singular(int client_id, int event_type_id, int pkg_id, int book_event_id, double amount, const char *payment_method) {
    // Generate a random receipt number
    srand(time(NULL));
    int receipt_no = rand() % 1000000;

    // Read required details
    char *client_name = read_user(client_id, "full_name");
    char *client_email = read_user(client_id, "email");
    char *client_phone = read_user(client_id, "phone");

    char *venue = read_event(client_id, book_event_id, "venue");
    char *event_date = read_event(client_id, book_event_id, "event_date");
    char *start_time = read_event(client_id, book_event_id, "start_time");
    char *balance_str = read_event(client_id, book_event_id, "balance");
    char *event_name = read_event(client_id, book_event_id, "name");

    char *package_name = read_pkg(event_type_id, pkg_id, "package_name");

    // Convert numeric values
    double remaining_balance = atof(balance_str);

    // Create the receipts folder for the user
    char user_receipts_folder[256];
    sprintf(user_receipts_folder, "data/users/%d/receipts", client_id);

#ifdef _WIN32
    mkdir(user_receipts_folder);
#else
    mkdir(user_receipts_folder, 0777);
#endif

    // Create the receipt file
    char receipt_file_path[256];
    sprintf(receipt_file_path, "%s/%06d.txt", user_receipts_folder, receipt_no);

    FILE *receipt_file = fopen(receipt_file_path, "w");
    if (receipt_file == NULL) {
        perror("[ERROR] Unable to create receipt file");
        return;
    }

    // Get the current date and time
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    char date_buffer[20], time_buffer[10];
    strftime(date_buffer, sizeof(date_buffer), "%d %b %Y", current_time);
    strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", current_time);

    // Write the receipt content
    fprintf(receipt_file,
        "==================================================\n"
        "                  🎟 Payment Receipt              \n"
        "==================================================\n"
        "                  EventEase Payment Gateway       \n"
        "==================================================\n\n"
        "🕒 Transaction Details\n"
        "--------------------------------------------------\n"
        "   Transaction Date : %s\n"
        "   Transaction Time : %s\n"
        "--------------------------------------------------\n\n"
        "💳 Payment Details\n"
        "--------------------------------------------------\n"
        "   Amount Paid      : PHP %.2f\n"
        "   Payment Method   : %s\n"
        "   Remaining Balance: PHP %.2f\n"
        "--------------------------------------------------\n\n"
        "📅 Event Details\n"
        "--------------------------------------------------\n"
        "   Event Name       : %s\n"
        "   Event ID         : %d\n"
        "   Date             : %s\n"
        "   Time             : %s\n"
        "   Venue            : %s\n"
        "--------------------------------------------------\n\n"
        "👤 Client Details\n"
        "--------------------------------------------------\n"
        "   Client Name      : %s\n"
        "   Booking ID       : %d\n"
        "--------------------------------------------------\n\n"
        "==================================================\n"
        "  Thank you for your payment! We look forward to  \n"
        "           seeing you at your event!             \n"
        "==================================================\n",
        date_buffer, time_buffer, amount, payment_method, remaining_balance,
        event_name, book_event_id, event_date, start_time, venue,
        client_name, book_event_id);

    fclose(receipt_file);

    // Save a copy to the Downloads folder
    char downloads_folder[512];
    const char *home = getenv("HOME");
    if (home != NULL) {
        sprintf(downloads_folder, "%s/Downloads/event-ease/receipts", home);
#ifdef _WIN32
        mkdir(downloads_folder);
#else
        mkdir(downloads_folder, 0777);
#endif

        char downloads_receipt_file[256];
        sprintf(downloads_receipt_file, "%s/%06d.txt", downloads_folder, receipt_no);

        FILE *downloads_file = fopen(downloads_receipt_file, "w");
        if (downloads_file != NULL) {
            FILE *read_file = fopen(receipt_file_path, "r");
            if (read_file != NULL) {
                char buffer[256];
                while (fgets(buffer, sizeof(buffer), read_file)) {
                    fputs(buffer, downloads_file);
                }
                fclose(read_file);
            }
            fclose(downloads_file);
        } else {
            perror("[ERROR] Unable to create receipt in Downloads folder");
        }
    } else {
        perror("[ERROR] Unable to locate home directory");
    }

    // Free allocated memory
    free(client_name);
    free(client_email);
    free(client_phone);
    free(venue);
    free(event_date);
    free(start_time);
    free(balance_str);
    free(event_name);
    free(package_name);
}


void print_receipt(int client_id, int event_type_id, int pkg_id, int book_event_id) {
    // Generate a random receipt number
    srand(time(NULL));
    int receipt_no = rand() % 1000000;

    // Read required details
    char *client_name = read_user(client_id, "full_name");
    char *client_email = read_user(client_id, "email");
    char *client_phone = read_user(client_id, "phone");

    char *venue = read_event(client_id, book_event_id, "venue");
    char *event_date = read_event(client_id, book_event_id, "event_date");
    char *start_time = read_event(client_id, book_event_id, "start_time");
    char *payment_deadline = read_event(client_id, book_event_id, "payment_deadline");
    char *balance_str = read_event(client_id, book_event_id, "balance");
    char *event_name = read_event(client_id, book_event_id, "name");

    char *package_name = read_pkg(event_type_id, pkg_id, "package_name");
    char *price_str = read_pkg(event_type_id, pkg_id, "price");

    // Convert numeric values
    double total_cost = atof(price_str);
    double remaining_balance = atof(balance_str);
    double down_payment = total_cost - remaining_balance;

    // Print the receipt
    printf("==================================================\n");
    printf("                   🧾 EventEase Receipt              \n");
    printf("==================================================\n\n");
    printf("Receipt No.        : %06d\n", receipt_no);
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    char formatted_date_time[100];
    strftime(formatted_date_time, sizeof(formatted_date_time), "%a %b %d %Y %H:%M:%S", current_time);

    printf("Transaction Date and Time: %s\n", formatted_date_time);
    printf("--------------------------------------------------\n");
    printf("🔖 Client Information\n");
    printf("--------------------------------------------------\n");
    printf("Name               : %s\n", client_name);
    printf("Email              : %s\n", client_email);
    printf("Phone              : %s\n\n", client_phone);
    printf("--------------------------------------------------\n");
    printf("🎉 Event Details\n");
    printf("--------------------------------------------------\n");
    printf("Event Name         : %s\n", event_name);
    printf("Event Date         : %s | Time: %s\n", event_date, start_time);
    printf("Venue              : %s\n", venue);
    printf("Package            : %s\n\n", package_name);
    printf("--------------------------------------------------\n");
    printf("💳 Payment Details\n");
    printf("--------------------------------------------------\n");
    printf("Total Package Cost : PHP %.2f\n", total_cost);
    printf("Down Payment       : PHP %.2f\n", down_payment);
    printf("Remaining Balance  : PHP %.2f\n", remaining_balance);
    printf("Payment Deadline   : %s\n\n", payment_deadline);
    printf("--------------------------------------------------\n");
    printf("📜 Notes\n");
    printf("--------------------------------------------------\n");
    printf("- Please settle the remaining balance by the deadline.\n");
    printf("- Contact us for any inquiries or changes to your booking.\n\n");
    printf("==================================================\n");
    printf("✨ Thank you for choosing EventEase! ✨\n");
    printf("\nPlease enter [ANY KEY] to go back.");
}


void print_receipt_singular(int client_id, int event_type_id, int pkg_id, int book_event_id, double amount, const char *payment_method) {
    // Generate a random receipt number
    srand(time(NULL));
    int receipt_no = rand() % 1000000;

    // Read required details
    char *client_name = read_user(client_id, "full_name");
    char *client_email = read_user(client_id, "email");
    char *client_phone = read_user(client_id, "phone");

    char *venue = read_event(client_id, book_event_id, "venue");
    char *event_date = read_event(client_id, book_event_id, "event_date");
    char *start_time = read_event(client_id, book_event_id, "start_time");
    char *balance_str = read_event(client_id, book_event_id, "balance");
    char *event_name = read_event(client_id, book_event_id, "name");

    char *package_name = read_pkg(event_type_id, pkg_id, "package_name");

    // Convert numeric values
    double remaining_balance = atof(balance_str);

    // Get current date and time
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    char date_buffer[20], time_buffer[10];
    strftime(date_buffer, sizeof(date_buffer), "%d %b %Y", current_time);
    strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", current_time);

    // Print the receipt
    printf("==================================================\n");
    printf("                  🎟 Payment Receipt              \n");
    printf("==================================================\n");
    printf("                  EventEase Payment Gateway       \n");
    printf("==================================================\n\n");
    printf("🕒 Transaction Details\n");
    printf("--------------------------------------------------\n");
    printf("   Transaction Date : %s\n", date_buffer);
    printf("   Transaction Time : %s\n", time_buffer);
    printf("--------------------------------------------------\n\n");
    printf("💳 Payment Details\n");
    printf("--------------------------------------------------\n");
    printf("   Amount Paid      : PHP %.2f\n", amount);
    printf("   Payment Method   : %s\n", payment_method);
    printf("   Remaining Balance: PHP %.2f\n", remaining_balance);
    printf("--------------------------------------------------\n\n");
    printf("📅 Event Details\n");
    printf("--------------------------------------------------\n");
    printf("   Event Name       : %s\n", event_name);
    printf("   Event ID         : %d\n", book_event_id);
    printf("   Date             : %s\n", event_date);
    printf("   Time             : %s\n", start_time);
    printf("   Venue            : %s\n", venue);
    printf("--------------------------------------------------\n\n");
    printf("👤 Client Details\n");
    printf("--------------------------------------------------\n");
    printf("   Client Name      : %s\n", client_name);
    printf("   Booking ID       : %d\n", book_event_id);
    printf("--------------------------------------------------\n\n");
    printf("==================================================\n");
    printf("  Thank you for your payment! We look forward to  \n");
    printf("           seeing you at your event!             \n");
    printf("==================================================\n");

    // Free allocated memory
    free(client_name);
    free(client_email);
    free(client_phone);
    free(venue);
    free(event_date);
    free(start_time);
    free(balance_str);
    free(event_name);
    free(package_name);
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
            return 1; 
        }
    }
    fclose(event_file);
    return 0; // Return -1 if the username was not found
}

/* typedef struct {
    int user_id; // asa nimo ganahan ipa da si notfication 
    int asa_gikan;  // id gihapo nsiya
    int notifcation_id;
    char notifcation_description[100]; // unsay ipadala nimo ni user_id
    int mana; // 0 for mana 1 for wala <- if ever si client kay nahuman niya ang specific or natanaw niya ang specific nga task
} Notifcation;
 */


// notifcations.

int add_notification(int user_id, const char *message, const char *status) {
    char path[100];
    snprintf(path, sizeof(path), "data/users/%d/notifications.txt", user_id);

    FILE *file = fopen(path, "a");
    if (!file) {
        perror("Failed to open notifications file");
        return -1;
    }

    // Generate a unique notification ID
    srand(time(NULL));
    int notification_id = rand() % 10000; // Simple random ID generation

    // Get current timestamp
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char date_added[30];
    strftime(date_added, sizeof(date_added), "%Y-%m-%d %H:%M:%S", local_time);

    // Write the notification to the file
    fprintf(file, "%d,\"%s\",\"%s\",\"%s\"\n", notification_id, message, status, date_added);

    fclose(file);
    return notification_id;
}

int mark_all_notifications_read(int user_id) 
{
    char file_path[100], temp_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/notifications.txt", user_id);
    snprintf(temp_path, sizeof(temp_path), "data/users/%d/temp_notifications.txt", user_id);

    FILE *file = fopen(file_path, "r");
    FILE *temp_file = fopen(temp_path, "w");

    if (!file || !temp_file) {
        perror("[ERROR] Unable to process notifications file");
        if (file) fclose(file);
        if (temp_file) fclose(temp_file);
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char message[200], status[10], date_added[30];

        // Parse the notification line
        if (sscanf(line, "%d,\"%199[^\"]\",\"%9[^\"]\",\"%29[^\"]\"", &id, message, status, date_added) == 4) {
            // Change status to "Read"
            fprintf(temp_file, "%d,\"%s\",\"Read\",\"%s\"\n", id, message, date_added);
        }
    }

    fclose(file);
    fclose(temp_file);

    // Replace the original file with the updated one
    remove(file_path);
    rename(temp_path, file_path);

    return 1; // Success
}

char **view_notifications(int user_id) 
{
    char path[100];
    snprintf(path, sizeof(path), "data/users/%d/notifications.txt", user_id);

    FILE *file = fopen(path, "r");
    if (!file) {
        perror("No notifications found for this user.");
        return NULL;
    }

    char **notifications = malloc(101 * sizeof(char *)); // Support up to 100 notifications + NULL
    if (!notifications) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) && count < 100) {
        int id;
        char message[200], status[10], date_added[30];

        // Parse notification details
        if (sscanf(line, "%d,\"%199[^\"]\",\"%9[^\"]\",\"%29[^\"]\"", &id, message, status, date_added) == 4) {
            char *formatted_notification = malloc(300);
            if (!formatted_notification) {
                perror("Memory allocation failed for a notification");
                break;
            }

            snprintf(formatted_notification, 300,
                     "%d. %s [%s] (Added: %s)",
                     id,
                     message,
                     strcmp(status, "Unread") == 0 ? "🔴 Unread" : "🟢 Read",
                     date_added);

            notifications[count++] = formatted_notification;
        }
    }

    notifications[count] = NULL; // Null-terminate the array
    fclose(file);

    // Handle no notifications
    if (count == 0) {
        notifications[0] = strdup("🎉 No notifications at the moment!");
        notifications[1] = NULL;
    }

    return notifications;
}


int clear_notifications(int user_id) {
    char file_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/notifications.txt", user_id);

    FILE *file = fopen(file_path, "w");
    if (!file) {
        perror("[ERROR] Unable to clear notifications");
        return -1;
    }

    fclose(file);
    return 1; // Success
}

void mark_notification_as_read(int user_id, int notification_id) {
    char path[100], temp_path[100];
    snprintf(path, sizeof(path), "data/users/%d/notifications.txt", user_id);
    snprintf(temp_path, sizeof(temp_path), "data/users/%d/temp_notifications.txt", user_id);

    FILE *file = fopen(path, "r");
    FILE *temp = fopen(temp_path, "w");

    if (!file || !temp) {
        perror("Failed to open files");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char message[200], status[10], date_added[30];

        if (sscanf(line, "%d,\"%199[^\"]\",\"%9[^\"]\",\"%29[^\"]\"", &id, message, status, date_added) == 4) {
            if (id == notification_id) {
                fprintf(temp, "%d,\"%s\",\"Read\",\"%s\"\n", id, message, date_added);
            } else {
                fprintf(temp, "%s", line);
            }
        }
    }

    fclose(file);
    fclose(temp);

    remove(path);
    rename(temp_path, path);
}

// reminders

int set_reminder(int user_id, const char *message, const char *due_date) 
{
    char file_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/reminders.txt", user_id);

    FILE *reminder_file = fopen(file_path, "a");
    if (reminder_file == NULL) {
        perror("Failed to open reminders file");
        return -1;
    }

    int reminder_id = rand() % 10000; // Generate a random reminder ID
    fprintf(reminder_file, "%d,\"%s\",\"%s\",\"PENDING\"\n", reminder_id, message, due_date);
    fclose(reminder_file);
    
    return reminder_id;
}

char *view_reminders(int user_id) 
{
    char file_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/reminders.txt", user_id);

    FILE *reminder_file = fopen(file_path, "r");
    if (reminder_file == NULL) 
    {
        perror("No reminders found for this user.");
        return NULL;
    }

    // Allocate memory for formatted output
    char *result = malloc(2048); 
    if (!result) 
    {
        perror("Memory allocation failed");
        fclose(reminder_file);
        return NULL;
    }

    // Initialize the result string with the header
    snprintf(result, 2048,
        "==================================================\n"
        "               ⏰ Your Reminders          \n"
        "==================================================\n\n");

    char line[256];
    int line_num = 1;
    int reminder_found = 0;

    // Read and format each reminder
    while (fgets(line, sizeof(line), reminder_file)) 
    {
        int id;
        char message[100], due_date[30], status[10];

        // Use a more flexible sscanf format with extra checks
        if (sscanf(line, "%d,\"%99[^\"]\",\"%29[^\"]\",\"%9[^\"]\"", &id, message, due_date, status) == 4) 
        {
            char formatted_line[300];
            snprintf(formatted_line, sizeof(formatted_line), 
                    "⏰ Reminder ID:%d\n   📌 Info       : \"%s\" \n   📅 Due        : %s [%s]\n", 
                    id,
                    message, 
                    due_date, 
                    strcmp(status, "PENDING") == 0 ? "🔴 Pending" : "🟢 Done");
            strcat(result, formatted_line);
            reminder_found = 1;
        } 
        else 
        {
            perror("Failed to parse a reminder line. Check the file format.");
        }
    }
    if(reminder_found == 0)
    {
        strcat(result, "🎉 You’re all caught up! No reminders for you today. \n\t\tEnjoy your day!\n");
    }
    strcat(result,"\n==================================================");
    fclose(reminder_file);

    return result; // Returns the formatted reminders
}


void reveal_all_users_except_organizers() 
{
    const char *users_file = "data/users_id.txt";

    FILE *user_ids_file = fopen(users_file, "r");
    if (user_ids_file == NULL) 
    {
        perror("[ERROR] Unable to open users_id.txt");
        return;
    }

    printf("==================================================\n");
    printf("              📋 User Information Reports          \n");
    printf("==================================================\n\n");

    int user_id;
    int user_found = 0;

    while (fscanf(user_ids_file, "%d:%*s\n", &user_id) != EOF) 
    {
        char user_info_file[256];
        sprintf(user_info_file, "data/users/%d/user_info.txt", user_id);

        FILE *info_file = fopen(user_info_file, "r");
        if (info_file == NULL) 
        {
            perror("[ERROR] Unable to open user_info.txt");
            continue;
        }

        char key_buffer[50];
        char value_buffer[256];

        printf("==================================================\n");
        printf("🆔 User ID: %d\n", user_id);
        printf("==================================================\n");

        while (fscanf(info_file, "%49[^:]:%255[^\n]\n", key_buffer, value_buffer) == 2) 
        {
            if (strcmp(key_buffer, "password") == 0 || strcmp(key_buffer, "signed_in") == 0 || 
                strcmp(key_buffer, "stay_logged_in") == 0 || strcmp(key_buffer, "id") == 0) 
            {
                // Skip displaying sensitive or irrelevant information
                continue;
            }

            printf("✨ %-15s: %s\n", key_buffer, value_buffer);
        }

        fclose(info_file);
        user_found = 1;
    }

    fclose(user_ids_file);

    if (!user_found) 
    {
        printf("\n🚫 No users found or accessible.\n");
    }

    printf("==================================================\n");
    printf("\n👉 Please enter the User ID to continue  (-1 to go back): ");
}


/* 
void check_due_reminders(int user_id, int alert_minutes) 
{
    char file_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/reminders.txt", user_id);

    FILE *reminder_file = fopen("data/reminders.txt", "r");
    if (reminder_file == NULL) {
        perror("Failed to open reminders file");
        return;
    }

    time_t now = time(NULL);
    char line[256];

    printf("\n-- Due Reminders --\n");

    while (fgets(line, sizeof(line), reminder_file)) 
    {
        int id, uid;
        char message[100], due_date[20], status[10];

        if (sscanf(line, "%d,%d,\"%99[^\"]\",\"%19[^\"]\",\"%9[^\"]", &id, &uid, message, due_date, status) == 5) {
            if (uid == user_id && strcmp(status, "PENDING") == 0) {
                struct tm due_time = {0};
                strftime(due_date, "%Y-%m-%d %H:%M:%S", &due_time);
                time_t due_time_t = mktime(&due_time);

                double diff_minutes = difftime(due_time_t, now) / 60;

                if (diff_minutes > 0 && diff_minutes <= alert_minutes) 
                {
                    printf("Reminder: %s (Due in %.0f minutes)\n", message, diff_minutes);
                }
            }
        }
    }

    fclose(reminder_file);
} */


int update_reminder_status(int user_id, int reminder_id, const char *new_status) 
{
    char file_path[100], temp_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/reminders.txt", user_id);
    snprintf(temp_path, sizeof(temp_path), "data/users/%d/temp_reminders.txt", user_id);

    FILE *reminder_file = fopen(file_path, "r");
    FILE *temp_file = fopen(temp_path, "w");

    if (reminder_file == NULL || temp_file == NULL) {
        perror("Failed to open files");
        if (reminder_file) fclose(reminder_file);
        if (temp_file) fclose(temp_file);
        return -1;
    }

    char line[256];
    int updated = 0;

    while (fgets(line, sizeof(line), reminder_file)) 
    {
        int r_id;
        char message[100], due_date[20], status[10];

        if (sscanf(line, "%d,\"%99[^\"]\",\"%19[^\"]\",\"%9[^\"]\"", &r_id, message, due_date, status) == 4) 
        {
            if (r_id == reminder_id) 
            {
                fprintf(temp_file, "%d,\"%s\",\"%s\",\"%s\"\n", r_id, message, due_date, new_status);
                updated = 1;
            } 
            else 
            {
                fprintf(temp_file, "%s", line);
            }
        }
    }

    fclose(reminder_file);
    fclose(temp_file);

    remove(file_path);
    rename(temp_path, file_path);

    if (updated) 
    {
        printf("\nReminder status updated successfully!\n");
        return 1;
    } else 
    {
        printf("\nReminder with ID %d not found.\n", reminder_id);
        return 0;
    }
}

int valid_reminder_id(int user_id, int reminder_id)
{
    char file_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/reminders.txt", user_id);

    FILE *reminder_file = fopen(file_path, "r");

    if (reminder_file == NULL) 
    {
        perror("Failed to open files");
        if (reminder_file) fclose(reminder_file);
        return -1;
    }

    char line[256];

    while (fgets(line, sizeof(line), reminder_file)) 
    {
        int r_id;
        if (sscanf(line, "%d,", &r_id) == 1) 
        {
            if (r_id == reminder_id) 
            {
                return 1;        
            } 
        
        }
    }
    return 0;
}

int delete_reminder(int user_id, int reminder_id) 
{
    char file_path[150], temp_path[150];
    snprintf(file_path, sizeof(file_path), "data/users/%d/reminders.txt", user_id);
    snprintf(temp_path, sizeof(temp_path), "data/users/%d/temp_reminders.txt", user_id);

    FILE *reminder_file = fopen(file_path, "r");
    FILE *temp_file = fopen(temp_path, "w");

    if (reminder_file == NULL || temp_file == NULL) 
    {
        perror("Failed to open files");
        if (reminder_file) fclose(reminder_file);
        if (temp_file) fclose(temp_file);
        return -1;
    }

    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), reminder_file)) 
    {
        // Skip empty lines
        if (line[0] == '\n' || line[0] == '\0') 
        {
            fprintf(temp_file, "\n");
            continue;
        }

        int r_id;
        if (sscanf(line, "%d,", &r_id) == 1) 
        {
            if (r_id != reminder_id) 
            {
                fprintf(temp_file, "%s", line);
            } 
            else 
            {
                found = 1;  
            }
        } 
        else 
        {
            fprintf(temp_file, "%s", line);
        }
    }

    fclose(reminder_file);
    fclose(temp_file);


    if (!SetFileAttributes(file_path, FILE_ATTRIBUTE_NORMAL)) {
        printf("Failed to adjust file attributes. Error: %lu\n", GetLastError());
    }

    if (remove(file_path) != 0) 
    {
        perror("Failed to delete the original reminders file");
        return -1;
    }

    
    if (rename(temp_path, file_path) != 0) 
    {
        perror("Failed to rename temporary file to original file");
        return -1;
    }

    if (found) 
    {
        printf("\n✅ Reminder deleted successfully!\n");
    } 
    else 
    {
        printf("\n⚠️ Reminder ID %d not found.\n", reminder_id);
    }

    return found;  // 1 if found and deleted, 0 if not found
}


// ticketing systems



// Function to create a new ticket
int set_ticket(int user_id, const char *message) 
{
    char file_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/support_tickets.txt", user_id);

    FILE *ticket_file = fopen(file_path, "a");
    if (ticket_file == NULL) {
        perror("Failed to open support tickets file");
        return -1;
    }

    int ticket_id = rand() % 10000; // Generate a random ticket ID
    fprintf(ticket_file, "%d,\"%s\",\"PENDING\"\n", ticket_id, message);
    fclose(ticket_file);

    return ticket_id;
}


// Function to view all tickets
char *view_tickets(int user_id) {
    char file_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/support_tickets.txt", user_id);

    FILE *ticket_file = fopen(file_path, "r");
    if (ticket_file == NULL) {
        perror("No support tickets found for this user.");
        return NULL;
    }

    char *result = malloc(2048);
    if (!result) {
        perror("Memory allocation failed");
        fclose(ticket_file);
        return NULL;
    }

    snprintf(result, 2048,
        "==================================================\n"
        "               📩 Your Support Tickets           \n"
        "==================================================\n\n");

    char line[256];
    int line_num = 1;
    int ticket_found = 0;

    while (fgets(line, sizeof(line), ticket_file)) {
        int id;
        char message[100], status[10];

        if (sscanf(line, "%d,\"%99[^\"]\",\"%9[^\"]\"", &id, message, status) == 3) {
            char formatted_line[300];
            snprintf(formatted_line, sizeof(formatted_line), 
                    "%d. ID:%d\n\t📌 Issue: \"%s\" \n\t[%s]\n", 
                    line_num++, id, message, 
                    strcmp(status, "PENDING") == 0 ? "🔴 Pending" : "🟢 Resolved");
            strcat(result, formatted_line);
            ticket_found = 1;
        } else {
            perror("Failed to parse a ticket line. Check the file format.");
        }
    }

    if (!ticket_found) {
        strcat(result, "🎉 No tickets found. All issues resolved or none reported yet.\n");
    }
    strcat(result, "\n==================================================");
    fclose(ticket_file);

    return result;
}



// Function to validate a ticket ID
int valid_ticket_id(int user_id, int ticket_id) 
{
    char file_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/support_tickets.txt", user_id);

    FILE *ticket_file = fopen(file_path, "r");
    if (ticket_file == NULL) {
        perror("Failed to open support tickets file");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), ticket_file)) {
        int id;
        if (sscanf(line, "%d,", &id) == 1 && id == ticket_id) {
            fclose(ticket_file);
            return 1; // Ticket ID is valid
        }
    }

    fclose(ticket_file);
    return 0; // Ticket ID is not found
}

// Function to update a ticket
int update_ticket(int user_id, int ticket_id, const char *new_status) {
    if (!valid_ticket_id(user_id, ticket_id)) {
        printf("\n⚠️ Ticket ID %d does not exist.\n", ticket_id);
        return 0;
    }

    char file_path[100], temp_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/support_tickets.txt", user_id);
    snprintf(temp_path, sizeof(temp_path), "data/users/%d/temp_support_tickets.txt", user_id);

    FILE *ticket_file = fopen(file_path, "r");
    FILE *temp_file = fopen(temp_path, "w");

    if (ticket_file == NULL || temp_file == NULL) {
        perror("Failed to open files for updating tickets");
        if (ticket_file) fclose(ticket_file);
        if (temp_file) fclose(temp_file);
        return -1;
    }

    char line[256];
    int updated = 0;

    while (fgets(line, sizeof(line), ticket_file)) {
        int id;
        char message[100], status[10];

        if (sscanf(line, "%d,\"%99[^\"]\",\"%9[^\"]\"", &id, message, status) == 3) {
            if (id == ticket_id) {
                fprintf(temp_file, "%d,\"%s\",\"%s\"\n", id, message, new_status);
                updated = 1;
            } else {
                fprintf(temp_file, "%s", line);
            }
        } else {
            perror("Failed to parse a ticket line. Check the file format.");
        }
    }

    fclose(ticket_file);
    fclose(temp_file);

    if (remove(file_path) != 0 || rename(temp_path, file_path) != 0) {
        perror("Failed to update the ticket file");
        return -1;
    }

    if (updated) {
        printf("\n✅ Ticket ID %d updated to status: %s\n", ticket_id, new_status);
    }

    return updated;
}

int delete_ticket(int user_id, int ticket_id) 
{
    char file_path[100], temp_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/support_tickets.txt", user_id);
    snprintf(temp_path, sizeof(temp_path), "data/users/%d/temp_support_tickets.txt", user_id);

    FILE *ticket_file = fopen(file_path, "r");
    FILE *temp_file = fopen(temp_path, "w");

    if (ticket_file == NULL || temp_file == NULL) 
    {
        perror("Failed to open files for deleting tickets");
        if (ticket_file) fclose(ticket_file);
        if (temp_file) fclose(temp_file);
        return -1;
    }

    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), ticket_file)) 
    {
        int id;
        if (sscanf(line, "%d,", &id) == 1) 
        {
            if (id == ticket_id) {
                found = 1; // Mark the ticket as found and skip writing it to the temp file
                continue;
            }
        }
        fprintf(temp_file, "%s", line); // Write other tickets to the temp file
    }

    fclose(ticket_file);
    fclose(temp_file);

    if (remove(file_path) != 0 || rename(temp_path, file_path) != 0) 
    {
        perror("Failed to update the ticket file after deletion");
        return -1;
    }

    if (found) 
    {
        printf("\n✅ Ticket ID %d deleted successfully.\n", ticket_id);
        return 1;
    } else {
        printf("\n⚠️ Ticket ID %d not found.\n", ticket_id);
        return 0;
    }
}

#define MAX_COMMENT_LENGTH 500

// Function to create feedback
int create_feedback(int user_id, int star, const char *comments)
{



    FILE *file = fopen("data/feedback.txt", "a");
    if (file == NULL)
    {
        perror("❌ Failed to open feedback file");
        return -1;
    }

    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    fprintf(file, "%d,%d,\"%s\",\"%04d-%02d-%02d %02d:%02d:%02d\"\n",
            user_id, star, comments,
            local_time->tm_year + 1900,
            local_time->tm_mon + 1,
            local_time->tm_mday,
            local_time->tm_hour,
            local_time->tm_min,
            local_time->tm_sec);

    fclose(file);

    printf("\n✅ Feedback submitted successfully! Thank you for your input.\n");
    return 1;
}

// Function to preview feedback
char *preview_feedback()
{
    FILE *file = fopen("data/feedback.txt", "r");
    if (file == NULL)
    {
        perror("❌ No feedback available to preview");
        return NULL;
    }

    char *result = malloc(4096); // Adjust size as needed
    if (result == NULL)
    {
        perror("❌ Memory allocation failed");
        fclose(file);
        return NULL;
    }

    snprintf(result, 4096,
             "==================================================\n"
             "                  📝 Feedback Preview             \n"
             "==================================================\n\n");

    char line[512];
    int feedback_found = 0;

    while (fgets(line, sizeof(line), file))
    {
        int user_id, star;
        char comments[MAX_COMMENT_LENGTH];
        char timestamp[20];

        if (sscanf(line, "%d,%d,\"%[^\"]\",\"%19[^\"]\"", &user_id, &star, comments, timestamp) == 4)
        {
            char formatted_feedback[600];
            snprintf(formatted_feedback, sizeof(formatted_feedback),
                     "User ID: %d\n"
                     "⭐ Rating: %d\n"
                     "📋 Comments: %s\n"
                     "📅 Submitted On: %s\n"
                     "--------------------------------------------------\n\n",
                     user_id, star, comments, timestamp);
            strcat(result, formatted_feedback);
            feedback_found = 1;
        }
    }

    fclose(file);

    if (!feedback_found)
    {
        strcat(result, "🎉 No feedback available at the moment.\n");
    }

    strcat(result, "==================================================");
    return result;
}

int add_payment_history(int user_id, const char *event_name, double amount, const char *payment_method) {
    char file_path[MAX_PATH];
    snprintf(file_path, sizeof(file_path), "data/users/%d/payment_history.txt", user_id);

    FILE *file = fopen(file_path, "a");
    if (file == NULL) {
        perror("[ERROR] Unable to open payment history file");
        return -1;
    }

    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char date_time[30];
    strftime(date_time, sizeof(date_time), "%Y-%m-%d %H:%M:%S", local_time);

    // Generate a random payment ID
    int payment_id = rand() % 100000;

    // Write the payment entry to the file
    fprintf(file, "%d,\"%s\",%.2f,\"%s\",\"%s\"\n", payment_id, event_name, amount, payment_method, date_time);
    fclose(file);

    printf("\n✅ Payment history updated successfully with Payment ID: %d\n", payment_id);
    return payment_id;
}

// Function to delete all payment history
int clear_payment_history(int user_id) 
{
    char file_path[MAX_PATH];
    snprintf(file_path, sizeof(file_path), "data/users/%d/payment_history.txt", user_id);

    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        perror("[ERROR] Unable to clear payment history file");
        return -1;
    }

    fclose(file);
    printf("\n🧹 Payment history cleared successfully!\n");
    return 1;
}

// Function to preview payment history
char *preview_payment_history(int user_id) 
{
    char file_path[MAX_PATH];
    snprintf(file_path, sizeof(file_path), "data/users/%d/payment_history.txt", user_id);

    FILE *file = fopen(file_path, "r");
    if (file == NULL) 
    {
        perror("[ERROR] Unable to open payment history file");
        return strdup("🎉 No payment history found. Time to book your first event!");
    }

    char *result = malloc(2048); // Allocate memory for the output string
    if (!result) {
        perror("[ERROR] Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Initialize the result string with a header
    snprintf(result, 2048,
        "\n==================================================\n"
        "               💵 Payment History                \n"
        "==================================================\n\n");

    char line[MAX_LINE];
    int line_num = 1;

    // Read and format each payment entry
    while (fgets(line, sizeof(line), file)) {
        int payment_id;
        char event_name[100], payment_method[100], date_time[30];
        double amount;

        // Parse the line
        if (sscanf(line, "%d,\"%99[^\"]\",%lf,\"%29[^\"]\",\"%29[^\"]\"", &payment_id, event_name, &amount, payment_method, date_time) == 5) {
            char formatted_line[MAX_LINE];
            snprintf(formatted_line, sizeof(formatted_line),
                     "%d. Payment ID: %d\n"
                     "   🏷️ Event: %s\n"
                     "   💰 Amount: PHP %.2f\n"
                     "   💳 Method: %s\n"
                     "   🗓️ Date: %s\n"
                     "--------------------------------------------------\n",
                     line_num++, payment_id, event_name, amount, payment_method, date_time);
            strcat(result, formatted_line);
        }
    }

    if (line_num == 1) {
        strcat(result, "🎉 No payment history found. Time to book your first event!\n");
    }

    strcat(result, "\n==================================================");
    fclose(file);

    return result; // Returns the formatted payment history
}