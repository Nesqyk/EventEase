#ifndef USERS_H
#define USERS_H

#define MAX_LINE 256

#define USERID_FILE "data/users_id.txt"
#define USERS_DIR "data/users/"
#define USER_INFO_FILE "user_info.txt"

typedef struct {
    int id;
    char username[50];
    char password[20];
    char full_name[50];
    char email[100];
    char phone[20];
    char role[20];
    int signed_in;  // 1 if signed in, 0 if signed out
} User;

/*
    char username[50];
    char password[20];
    char full_name[50];
    char email[100];
    char phone[20];
    char role[20];
*/

char *view_notification(int user_id);

int delete_user(int id);

int force_login(int id, const char username[50]);

int generate_user_id();

int get_id_username(char username[50]);

int login_user(char user_input[50], const char password[50]);

char *read_all_user_by_key(const char key[50]);

char *read_last_login(char key[50]);

int *update_last_login(char key[50], char value[50]);

char *read_user(int id, char key[50]);

int register_user(User user);

int remove_directory(const char *path);

int update_user(int id, char *key, char *value);

int valid_user_id(int id);

int username_exist(char username[50]);

void reveal_all_users_except_organizers();

void print_receipt_singular(int client_id, int event_type_id, int pkg_id, int book_event_id, double amount, const char *payment_method);

void generate_receipt_singular(int client_id, int event_type_id, int pkg_id, int book_event_id, double amount, const char *payment_method);

// notifcations

void mark_notification_as_read(int user_id, int notification_id);

char **view_notifications(int user_id);

int reveal_organizer_id();

int count_unread_notifications(int user_id);


int add_notification(int user_id, const char *message, const char *status);

int mark_all_notifications_read(int user_id);

void generate_receipt(int client_id, int event_type_id, int pkg_id, int book_event_id);

void print_receipt(int client_id, int event_type_id, int pkg_id, int book_event_id);

// reminders
int delete_reminder(int user_id, int reminder_id);

int update_reminder_status(int user_id, int reminder_id, const char *new_status); 

char *view_reminders(int user_id);

int set_reminder(int user_id, const char *message, const char *due_date);

int valid_reminder_id(int user_id, int reminder_id);


// ticket

int update_ticket(int user_id, int ticket_id, const char *new_status);

// Function to validate a ticket ID
int valid_ticket_id(int user_id, int ticket_id);

// Function to view all tickets
char *view_tickets(int user_id);

// Function to create a new ticket
int set_ticket(int user_id, const char *message);

int delete_ticket(int user_id, int ticket_id);

// feedback
int create_feedback(int user_id, int star, const char *comments);

char *preview_feedback();

// payment history

char *preview_payment_history(int user_id);

// Function to delete all payment history
int clear_payment_history(int user_id);

int add_payment_history(int user_id, const char *event_name, double amount, const char *payment_method);

#endif //USERS_H