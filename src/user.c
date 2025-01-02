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

    while (fscanf(ids_file, "%d:%*s\n", &id) != EOF) 
    {
        sprintf(dir_name, "%s%d", USERS_DIR, id);
        sprintf(info_filename, "%s/%s", dir_name, USER_INFO_FILE);

        FILE *info_file = fopen(info_filename, "r");
        if (!info_file) 
        {
            perror("[ERROR] Unable to open user_info.txt");
            fclose(ids_file);
            return -1;
        }

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

                FILE *last_login = fopen("data/last_login.txt", "w");
                if (last_login) 
                {
                    time_t g_t = time(NULL);
                    char *generated_date = ctime(&g_t);

                    fprintf(last_login, "id:%d\nusername:%s\nsigned_in:1\nstay_logged_in:%s\nrole:%s\ndate_logged_on:%s\n",
                            id, file_username, read_user(id,"stay_logged_in"), read_user(id, "role") , generated_date);
                    fclose(last_login);
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
    
    char notification_path[50], reminder_path[50];
    sprintf(notification_path, "%s/notifcations.txt", dir_name);
    sprintf(reminder_path, "%s/reminders.txt", dir_name);

    create_file(notification_path);
    create_file(reminder_path);
    
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

void generate_receipt(int client_id, int event_type_id, int pkg_id, int book_event_id) {
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

    // Create the receipts folder for the user
    char user_receipts_folder[256];
    sprintf(user_receipts_folder, "data/users/%d/receipts", client_id);

    // Create the directory if it doesn't exist
#ifdef _WIN32
    mkdir(user_receipts_folder);
#else
    mkdir(user_receipts_folder, 0777);
#endif

    // Create the receipt file with the receipt number as its name
    char receipt_file_path[256];
    sprintf(receipt_file_path, "%s/%06d.txt", user_receipts_folder, receipt_no);

    FILE *receipt_file = fopen(receipt_file_path, "w");
    if (receipt_file == NULL) {
        perror("[ERROR] Unable to create receipt file");
        return;
    }

    fprintf(receipt_file,
        "==================================================\n"
        "                   🧾 EventEase Receipt              \n"
        "==================================================\n\n"
        "Receipt No.        : %06d\n"
        "Transaction Date   : %s\n\n"
        "--------------------------------------------------\n"
        "🔖 Client Information\n"
        "--------------------------------------------------\n"
        "Name               : %s\n"
        "Email              : %s\n"
        "Phone              : %s\n\n"
        "--------------------------------------------------\n"
        "🎉 Event Details\n"
        "--------------------------------------------------\n"
        "Event Name         : %s\n"
        "Event Date         : %s | Time: %s\n"
        "Venue              : %s\n"
        "Package            : %s\n\n"
        "--------------------------------------------------\n"
        "💳 Payment Details\n"
        "--------------------------------------------------\n"
        "Total Package Cost : PHP %.2f\n"
        "Down Payment       : PHP %.2f\n"
        "Remaining Balance  : PHP %.2f\n"
        "Payment Deadline   : %s\n\n"
        "--------------------------------------------------\n"
        "📜 Notes\n"
        "--------------------------------------------------\n"
        "- Please settle the remaining balance by the deadline.\n"
        "- Contact us for any inquiries or changes to your booking.\n\n"
        "==================================================\n"
        "✨ Thank you for choosing EventEase! ✨\n",
        receipt_no, __DATE__,
        client_name, client_email, client_phone,
        event_name, event_date, start_time, venue, package_name,
        total_cost, down_payment, remaining_balance,
        payment_deadline
    );
    fclose(receipt_file);

    // Locate the Downloads folder
    char downloads_folder[512];

    const char *home = getenv("HOME");
    if (home != NULL) {
        sprintf(downloads_folder, "%s/Downloads/event-ease/receipts", home);
        mkdir(downloads_folder);
    } else {
        perror("[ERROR] Unable to locate home directory");
        return;
    }

    // Copy receipt to Downloads folder
    char downloads_receipt_file[256];
    sprintf(downloads_receipt_file, "%s/%06d.txt", downloads_folder, receipt_no);

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
    }

    fclose(downloads_file);

    // Free allocated memory
    free(client_name);
    free(client_email);
    free(client_phone);
    free(venue);
    free(event_date);
    free(start_time);
    free(payment_deadline);
    free(balance_str);
    free(event_name);
    free(package_name);
    free(price_str);
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
    printf("Transaction Date   : %s\n\n", read_event(client_id, book_event_id, "booking_date"));
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

int add_notification(int user_id, const char *message) {
    char file_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/notifcations.txt", user_id);

    FILE *file = fopen(file_path, "a");
    if (file == NULL) {
        perror("Error opening notification file");
        return 0;
    }

    time_t now = time(NULL);
    fprintf(file, "UNREAD:%s:%s\n", message, ctime(&now)); // Store as UNREAD:Message:Timestamp
    fclose(file);

    return 1;
}


char *read_notifications(int user_id) 
{
    char file_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/notifications.txt", user_id);

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("No notifications found for this user.");
        return NULL;
    }

    // Allocate memory for notifications
    char *result = malloc(2048); // Increased size for formatted output
    if (!result) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Initialize the result string with the header
    snprintf(result, 2048,
        "=========================================\n"
        "           📬 Notifications 📬           \n"
        "=========================================\n\n");

    char buffer[256];
    int line_num = 1;
    int notifcation_found = 0;
    // Read and format each notification
    while (fgets(buffer, sizeof(buffer), file)) {
        char line[300];
        char *status = (strncmp(buffer, "UNREAD", 6) == 0) ? "\U0001F534 Unread" : "\U0001F7E2 Read"; // Emojis for statuses
        snprintf(line, sizeof(line), "%d. [%s] %s", line_num++, status, buffer + 7); // Skip "UNREAD:" or "READ:"
        strcat(result, line);
    }

    // Footer
    strcat(result, "\n=========================================\n");
    fclose(file);

    return result; // Returns the formatted notifications
}



int delete_notification(int user_id, int notif_id) 
{
    char file_path[100], temp_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/notifcations.txt", user_id);
    snprintf(file_path, sizeof(file_path), "data/users/%d/notifcations_temp.txt", user_id);

    FILE *file = fopen(file_path, "r");
    FILE *temp = fopen(temp_path, "w");

    if (!file || !temp) {
        perror("Error opening files");
        return 0;
    }

    char buffer[256];
    int current_line = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (current_line != notif_id) {
            fputs(buffer, temp);
        }
        current_line++;
    }

    fclose(file);
    fclose(temp);

    remove(file_path); // Delete original file
    rename(temp_path, file_path); // Rename temp file

    return 1;
}


int mark_notification_read(int user_id, int notif_id) 
{
    char file_path[100], temp_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/notifcations.txt", user_id);
    snprintf(file_path, sizeof(file_path), "data/users/%d/notifcations_temp.txt", user_id);

    FILE *file = fopen(file_path, "r");
    FILE *temp = fopen(temp_path, "w");

    if (!file || !temp) {
        perror("Error opening files");
        return 0;
    }

    char buffer[256];
    int current_line = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (current_line == notif_id && strncmp(buffer, "UNREAD:", 7) == 0) {
            fprintf(temp, "READ:%s", buffer + 7); // Change UNREAD to READ
        } else {
            fputs(buffer, temp);
        }
        current_line++;
    }

    fclose(file);
    fclose(temp);

    remove(file_path);
    rename(temp_path, file_path);

    return 1;
}

int clear_notifications(int user_id) 
{
    char file_path[100];
    snprintf(file_path, sizeof(file_path), "data/users/%d/notifcations.txt", user_id);
    return remove(file_path) == 0;
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
        "               📅 Your Reminders          \n"
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
                    "%d. 📌 \"%s\" \n\t(Due: %s) [%s]\n", 
                    line_num++, 
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
        strcat(result, "🎉 You’re all caught up! No reminders for you today. Enjoy your day!\n");
    }
    strcat(result,"\n==================================================");
    fclose(reminder_file);

    return result; // Returns the formatted reminders
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

int delete_reminder(int user_id, int reminder_id) 
{
    char file_path[100], temp_path[100];
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
    }

    fclose(reminder_file);
    fclose(temp_file);

    remove(file_path);
    rename(temp_path, file_path);

    if (found) 
    {
        printf("\nReminder deleted successfully!\n");
    } else 
    {
        printf("\nReminder ID %d not found.\n", reminder_id);
    }

    return found;
}

