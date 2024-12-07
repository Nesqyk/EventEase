#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auth.h"
#include "data_handler.h"
#include "events.h"

int display_menu();
int display_auth_menu();
int is_valid_choice(int n);

int main()
{
    display_auth_menu();
}

// Displays the main menu of the program.
int display_auth_menu()
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
            printf("%d. %s\n", i + 1, choices[i]);
        }

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                if(is_auth_file_empty(auth_file))
                {
                    printf("\nPlease register first.\n\n");
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
                    printf("Please make sure your credentials are correct.\n");
                }
                continue; 

            case 2:
                if(!is_auth_file_empty(auth_file))
                {
                    // We can also include; and ask the user if he/she is having 
                    // a problem regarding to their account's credentials?
                    printf("Please proceed to login\n");
                    fclose(auth_file);
                    continue;
                }

                printf("\n-- %s --\n", choices[1]);
                printf("Enter username: ");
                scanf("%s", username);

                if(strlen(username) < 3 && strlen(username) > MAX_USER)
                {
                    printf("Invalid username");
                    continue;  
                }

                printf("Enter pasword: ");
                scanf("%s", password);

                int register_status = register_user(username, password);
                if(register_status == 0)
                {
                    printf("Failed Registration Please Try Again\n");
                } else if(register_status == 1) {
                    printf("Registration Sucessful;\nPlease proceeed to login\n");
                    continue;
                }
                continue;
            case 3:

                fclose(auth_file);
                char *name = read_auth_key("Username");
                printf("Goodbye %s\n!", name);
                printf("Exiting now...\n");
                return 0;
        }

    } while(1);
}

int display_menu()
{
    int user_choice;
    const char *menu_options[] = {"Dashboard", "Create Event", "My Events","Reports","Reviews", "Exit"};
    
    do {
        char *n = read_auth_key("Username");
        printf("Welcome to the EventEase %s\n", n);
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
                // list the type first
                int event_type_key;
                printf("Please select what type of event it is;\n");
                
                char *list_types = read_type_of_events();
                int list_size = sizeof(list_types) / sizeof(list_types[0]);
                char *event_type_name;
                for(int i = 0; i < list_size; i++)
                {
                    printf("%d. %s", i + 1, list_types[i]);
                }
                printf("%d. Back", list_size);
                scanf("%d", &event_type_key);

                if(event_type_key == list_size)
                {
                    display_menu();
                }

                event_type_name = list_types[event_type_key];

                char client_name[30];
                float cost;


                printf("Input the Following:\n");
                printf("Client Name: ");
                scanf("%s", client_name);

                if(strlen(client_name) <= 3 && strlen(client_name))
                {
                    printf("Client Name must not be less than 3 or greater than 20\n");
                    scanf("%s", client_name);
                }

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
                printf("See you again!\n");
                printf("Exiting now...\n");
                return 0;
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