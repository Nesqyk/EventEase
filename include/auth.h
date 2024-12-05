#ifndef AUTH_H 
#define AUTH_H

#define MAX_PASS 12
#define MAX_USER 12
#define DATA_DIR "data/"  // Directory path for files

// Logins user; 0 Failed, 1 Sucess, 2 Login
int register_user(char username[MAX_USER], char password[MAX_PASS]);

// Logins user; 0 Failed, 1 Sucess
int login_user(const char username[MAX_USER], const char password[MAX_PASS]);

// Checks if auth.txt is empty
int is_auth_file_empty(FILE *file);

// Checks the username of the user
char *check_username();


#endif // COLLECTIONS_AUTH_H