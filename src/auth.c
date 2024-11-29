#include <stdio.h>
#include <string.h>
#include "data_handler.h"

#define MAX_PASS 12
#define MAX_USER 12
// 1 for success 0 for failed

int register_user(char username[12], char password[12])
{
    char filename[30];
    FILE *file = check_file_existence("auth.txt", "r");

    if (file == NULL)
    {
        sprintf(filename, "%sauth.txt", DATA_DIR);
    }

    int username_size = strlen(username);
    int password_size = strlen(password);

    if ((username_size > MAX_USER) && (password_size > MAX_PASS))
    {
        return 0; 
    }

    int has_special_char = 0;
    while(has_special_char)
    {
        for(int i = 0; i < username;i++)
        {
            username = username[i];
            password = password[i];

            
            // : >
            // 65 97
            // if both are null then break.
        }
    }

    if(has_special_char == 1)
    {
        return 0;
    }
    // check if the userame doesnt have any number or spaces
    // check if the password does not have any special characters
    // check if the password is too short or long
    // check if the username is too short or long

}

int login_user(char username, int password)
{
    // check if the username exist
    // check if the username matches the exact username
    // check if the password matches the exact password
}