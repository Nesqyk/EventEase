#include <stdio.h>
#include <string.h>
#include "data_handler.h"

#define MAX_PASS 12
#define MAX_USER 12
#define DATA_DIR "data/"  // Directory path for files

// Registers a user; 1 suceess, 0 failed.
int register_user(char username[MAX_USER], char password[MAX_PASS])
{
    char filename[30];
    sprintf(filename, "%sauth.txt", DATA_DIR);
    FILE *file = check_file_existence(filename, "r");

    // If file doesn't exist, create it
    if (file == NULL)
    {
        file = fopen(filename, "w");
        if (!file) {
            perror("Error opening auth file");
            return 0;  // File opening failed
        }
        fclose(file);
    }

    // Check if username and password are within allowed length
    int username_size = strlen(username);
    int password_size = strlen(password);

    if (username_size < 3 || username_size > MAX_USER || password_size < 6 || password_size > MAX_PASS)
    {
        return 0;  // Invalid length
    }

    for (int i = 0; i < username_size; i++)
    {
        if (username[i] == ' ') {
            return 0;  // Invalid username contains spaces
        }
    }

    // If all checks passed, save username and password to file
    file = fopen(filename, "a");
    if (!file)
    {
        perror("Error opening auth file for writing");
        return 0;
    }

    fprintf(file, "Username:%s\nPassword:%s", username, password);  // Store username and password
    fclose(file);
    return 1;  // Success
}

// Login function
int login_user(const char username[MAX_USER], const char password[MAX_PASS])
{
    FILE *file = check_file_existence("data/auth.txt", "r");
    if (file == NULL)
    {
        return 0;  // File doesn't exist
    }

    char stored_username[MAX_USER], stored_password[MAX_PASS];
    while (fscanf(file, "Username:%s\nPassword:%s", stored_username, stored_password) != EOF)
    {
        // Check if username matches
        if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0)
        {
            fclose(file);
            return 1;  // Successful login
        }
    }

    fclose(file);
    return 0;  // Failed login
}

