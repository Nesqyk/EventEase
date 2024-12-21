#ifndef USERS_H
#define USERS_H

#define MAX_LINE 256
#define MAX_ID_RANGE 100

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
int delete_user(int id);

int force_login(int id, const char username[50]);

int generate_user_id();

int get_id_username(char username[50]);

int login_user(char user_input[50], const char password[50]);

char *read_all_user_by_key(const char key[50]);

char *read_last_login(char key[50]);

char *read_user(int id, char key[50]);

int register_user(User user);

int remove_directory(const char *path);

int update_user(int id, char *key, char *value);

int valid_user_id(int id);

int username_exist(char username[50]);

#endif //USERS_H