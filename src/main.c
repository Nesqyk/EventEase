#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auth.h"
#include "data_handler.h"
#include "events.h"
#include "utils.h"

int display_menu();
int display_auth_menu();

int main()
{
    display_auth_menu();
}

// Displays the main menu of the program.
// TODO: kung naa auth system the need to implement profile stuffs is necessary?
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
        system("cls");
        char *choices[] = {"Login","Register", "Exit"};
        if(!is_auth_file_empty(auth_file)) {
            printf("Welcome Back! %s\n", read_auth("Username"));
        } 

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
                printf("\n-- %s --\n", choices[0]);
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
                    printf("\nPlease proceed to login\n\n");
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
                } else if(register_status == 1) 
                {
                    printf("Registration Sucessful;\nPlease proceeed to login\n");
                    continue;
                }
                continue;
            case 3:

                fclose(auth_file);
                char *name = read_auth("Username");
                system("cls");
                printf("Goodbye %s\n!", name);
                printf("Exiting now...\n");
                return 0;
        }

    } while(1);
}

int display_menu()
{
    int user_choice;
    char *menu_options[] = {"Dashboard", "Create Event", "My Events","Reports","Reviews", "Back"};
    int menu_size = sizeof(menu_options) / sizeof(menu_options[0]);
    
    do {
        char *n = read_auth("Username");
        printf("\nWelcome to EventEase %s\n", n);
        printf("Manage your events easily!\n\n");
        printf("Select an Option\n");

        for(int i = 0; i < sizeof(menu_options) / sizeof(menu_options[0]); i++)
        {
            printf("%d. %s\n", i + 1, menu_options[i]);
        }

        printf("Enter your option: ");
        scanf("%d", &user_choice);

        if(user_choice < 1 || user_choice > menu_size)
        {
            printf("Invalid Option Please Try Again\n");
            continue;
        }

        switch(user_choice)
        {
            // TODO: Implement; everytime the client picks a choice it'll clear the previous printed stuffs.
            // dashboard - rusell
            case 1:
                preview_events();
                continue;
            case 2:
                // Implement 'back' smth 'or 'cancel'
                if (count_events() >= atoi(read_config("max_events"))) {
                    printf("You cannot create more events; reached the max limit\n");
                    continue;
                }

                printf("\nPlease select what type of event it is:\n");

                char type_events[5][100] = {"Wedding", "Birthday", "Graduation", "Party", "Back"};
                int list_size = sizeof(type_events) / sizeof(type_events[0]);

                event_types:
                    for (int i = 0; i < list_size; i++) {
                        printf("%d. %s\n", i + 1, type_events[i]);
                    }

                int event_type_key;
                printf("\nEnter Option: ");
                scanf("%d", &event_type_key);

                if (event_type_key < 1 || event_type_key > list_size) 
                {
                    printf("Invalid option. Please choose a number between 1 and %d.\n", list_size);
                    goto event_types;
                }
                
                if (event_type_key == list_size) {
                    display_menu(); 
                }

                getchar();
                char client_name[30];
                printf("Input the Following:\n");
                printf("Client Name: ");
                fgets(client_name, sizeof(client_name), stdin);
                client_name[strcspn(client_name, "\n")] = '\0';  

                while (strlen(client_name) < 3 || strlen(client_name) > 20) {
                    printf("Client Name must not be less than 3 or greater than 20. Please enter again: ");
                    fgets(client_name, sizeof(client_name), stdin);
                }

                float cost, balance;
                int no_attendee;
                char venue[100], completion_date[100];

                // implement; check if numbers are numbers?
                printf("Enter the approx. cost: ");
                // repetitive  
                while(scanf("%f", &cost) != 1)
                {
                    printf("Please input a valid number!\n");
                    printf("Enter the approx. cost: ");
                    while(getchar() != '\n');
                }
                             
                printf("Enter %s's: Balance:  ", client_name);  
                while(scanf("%f", &balance) != 1)
                {
                    printf("Please input a valid number.\n");
                    printf("Enter %s's: Balance:  ", client_name);
                    while(getchar() != '\n');
                }

                printf("Enter No. of Attendees: ");
                while(scanf("%d", &no_attendee) != 1)
                {
                    printf("Please input a valid number.\n");
                    printf("Enter No. of Attendees: ");
                    while(getchar() != '\n');
                }

                getchar();
                printf("Enter Event's Venue: ");
                fgets(venue, sizeof(venue), stdin);
                venue[strcspn(venue, "\n")] = '\0';  

                printf("Enter Event's Completion Date: ");
                fgets(completion_date, sizeof(completion_date), stdin);
                
                // Generate a unique event ID
                int id = generate_unique_id();

                int event = create_event(id, event_type_key - 1, client_name, cost, balance, no_attendee, venue, completion_date);
                
                if(event != 1)
                {
                    printf("Error creating event. Please try again");
                    delete_event(id);
                }

                preview_event(id);

                char confirmation;

                printf("\n\nPlease confirm the details; Y/N\n");
                printf("(Y) Finish Creating Event\n(N) Edit the value: ");
                scanf("%c", &confirmation); 

                switch (confirmation) 
                {
                    case 'Y':
                        printf("Created Event %d Sucessfully.\n", id);
                        display_menu();
                    case 'N':
                        printf("Which details would you like to update?\n");
                        printf("Select from the following:\n");

                        // editable details
                        char *valid_string_keys[] = {"client_name", "cost", "balance", "no_attendee", "venue", "completion_date"};
                        valid_key:
                            for (int i = 0; i < sizeof(valid_string_keys) / sizeof(valid_string_keys[0]); i++) 
                            {
                                printf("%s, ", valid_string_keys[i]);
                            }

                        printf("\nValue to Change: ");

                        char temp[20];
                        scanf("%s", temp);
                        
                        for (int i = 0; i < sizeof(valid_string_keys) / sizeof(valid_string_keys[0]); i++) 
                        {
                            if (strcmp(temp, valid_string_keys[i]) != 0) 
                            {
                                printf("Invalid input. Please try again.\n");
                                goto valid_key;
                            }
                        }

                        printf("Enter new value: ");
                        char temp_value[50];
                        scanf("%s", temp_value);

                        update_event(id, temp, temp_value);
                        printf("Successfully updated: %s : %s\n", temp, temp_value);
                        continue; 
                    default:
                        printf("Invalid choice. Please confirm again.\n");
                }
                continue;
            // my events - junsay
            case 3: 
                init_myevent();     
                continue;
            // reviews - chelesea
            case 4:
                continue;
            // reports - lo
            case 5:
                continue;
            // exit
            case 6: 
                printf("Go Back!\n");
                printf("Exiting now...\n");
                return -1;
        }
    } while(1);
}