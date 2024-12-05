#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auth.h"
#include "data_handler.h"

// #include "include/data_handler.h"

void display_menu();
void display_auth_menu();
int is_valid_choice(int n);

int event_ids[10];

int main()
{
    display_auth_menu();
}

// Displays the main menu of the program.
void display_auth_menu()
{
    char username[MAX_USER];
    char password[MAX_PASS];
    int choice;

    char filename[30];
    sprintf(filename, "%sauth.txt", DATA_DIR);

    FILE *auth_file = fopen(filename, "r");

    do {
        // 1.login
        // 2. register
        // 3. exit
        char *choices[] = {"Login","Register", "Exit"};

        for(int i = 0; i < 3; i++)
        {
            printf("%d. %s", i + 1, choices[i]);
        }

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                if(is_auth_file_empty(auth_file))
                {
                    fclose(auth_file);
                    printf("Please register first.");
                    continue;
                }
                printf("\n-- %s --\n", choices[1]);
                printf("Enter username : ");
                scanf("%s", username);

                printf("Enter pasword: ");
                scanf("%s", password);

                int login = login_user(username, password);
                if(login == 1)
                {
                    display_menu();
                } else if (login == 0)
                {
                    printf("Please make sure your credentials are correct.");
                }

            case 2:
                if(!is_auth_file_empty(auth_file))
                {
                    // We can also include; and ask the user if he/she is having 
                    // a problem regarding to their account's credentials?
                    system("cls");
                    printf("Please proceed to login\n");

                    fclose(auth_file);
                    continue;
                }

                printf("\n-- %s --\n", choices[1]);
                printf("Enter username: ");

                if(strlen(username) < 3 && strlen(username) > MAX_USER)
                {
                    scanf("%s", username);
                } else 
                {
                    printf("Username must not be less than 3 and greater than %d\nTry Again: ", MAX_USER);    
                    scanf("%s", username);
                }

                printf("Enter pasword: ");
                scanf("%s", password);

                int register_status = register_user(username, password);
                if(register_status == 0)
                {
                    printf("Failed Registration Please Try Again");
                } else if(register_status == 1) {
                    printf("Registration Sucessful;\n Please proceeed to login");
                    continue;
                }
            case 3:

                printf("Goodbye!");
                printf("Exiting now...");
                system("cls");
                break;
        }

    } while(1);
}

void display_menu()
{
    int user_choice;
    const char *menu_options[] = {"Dashboard", "Create Event", "My Events","Reports","Reviews", "Exit"};
    
    do {
        printf("Select an Option\n");

        for(int i = 0; i < 6; i++)
        {
            printf("%d. %s\n", i + 1, menu_options[i]);
        }

        printf("Enter your option");
        scanf("%d", &user_choice);

        if(is_valid_choice(user_choice) != 1)
        {
            printf("Invalid Option");
            continue;
        }

        switch(user_choice)
        {
            // TODO: Ari initialize sa tanan :>
            // TODO: Initialize events according to their assigned value.
            // dashboard - rusell
            case 1:
                continue;
            // create event - tyrone
            case 2:
                continue;
            // my events - junsay
            case 3:
                continue;
            // reviews - chelesea
            case 4:
                
                continue;
            // reports - lo
            case 5:
                continue;
            // exit
            case 6: 
                break;
        }
    } while(1);
}

int is_valid_choice(int n)
{
    if (n >= 1 && n <= 6)
    {
        return 1;
    }
    return 0;
}