#include <stdio.h>
#include <string.h>
#include "data_handler.h"

// auth seems fine now

#define MAX_PASS 12
#define MAX_USER 12
#define DATA_DIR "data/"

int is_auth_file_empty(FILE *file) {
    char stored_username[MAX_USER], stored_password[MAX_PASS];
    rewind(file);
    if (fscanf(file, "Username:%s\nPassword:%s", stored_username, stored_password) != 2) {
        return 1;
    }
    return 0;
}

char *read_auth_key(const char key[20]) {
    char filename[30];
    sprintf(filename, "%sauth.txt", DATA_DIR);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    static char stored_username[MAX_USER], stored_password[MAX_PASS];
    while (fscanf(file, "Username:%s\nPassword:%s", stored_username, stored_password) == 2) {
        if (strcmp(key, "Username") == 0) 
        {
            fclose(file);
            return stored_username;
        } else if (strcmp(key, "Password") == 0) {
            fclose(file);
            return stored_password;
        }
    }
    fclose(file);
    return NULL;
}

// 1 sucess 0 failed
int register_user(char username[MAX_USER], char password[MAX_PASS]) {
    char filename[30];
    sprintf(filename, "%sauth.txt", DATA_DIR);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        file = fopen(filename, "w");
        if (!file) {
            // perror("Error creating auth file");
            return 0;
        }
        fprintf(file, "Username:\nPassword:");
        fclose(file);
        file = fopen(filename, "r");
    }

    fclose(file);

    int username_size = strlen(username);
    int password_size = strlen(password);

    if ((username_size < 3 && username_size > MAX_USER) || (password_size < 6 && password_size > MAX_PASS)) {
        return 0;
    }

    for (int i = 0; i < username_size; i++) {
        if (username[i] == ' ') {
            return 0;
        }
    }

    file = fopen(filename, "w");
    if (!file) {
        // perror("Error opening auth file for writing");
        return 0;
    }
    fprintf(file, "Username:%s\nPassword:%s", username, password);
    fclose(file);
    return 1;
}

int login_user(const char username[MAX_USER], const char password[MAX_PASS]) {
    char filename[30];
    sprintf(filename, "%sauth.txt", DATA_DIR);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    char stored_username[MAX_USER], stored_password[MAX_PASS];
    while (fscanf(file, "Username:%s\nPassword:%s", stored_username, stored_password) == 2) {
        if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

/*
char *read_username()
{
    char filename[30];

    sprintf(filename, "%sauth.txt",DATA_DIR);

    FILE *auth_file = fopen(filename, "r");

    if(auth_file == NULL)
    {
        return NULL; 
    }

    static char stored_username[12];
    while(fscanf(auth_file, "Username:%11s", stored_username) == 1)
    {
        fclose(auth_file);
        return stored_username;
    } 

    fclose(auth_file);
    return NULL;
}
*/