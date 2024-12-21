
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "menu.h"


/*
 login
  login-as
  
 Sign-Up
 Quit

*/
int auth_menu()
{
    int hero_choice;
    char *hero_options[] = {"Login", "Register", "Quit"};
    int hero_size = sizeof(hero_options) / sizeof(hero_options[0]);

    char password[50];

    char user_input[50];

    int data_filled = 0;

    char user_members[6][50] = {"Username", "Password", "Full Name", "Phone Number", "Email", "Role"};
    char user_login_members[2][20] = {"Username or ID", "Password"};
    
   char *roles_users = read_all_user_by_key("role");
   char temp_role[1000]; // TODO: dynamically allocate memory size for temp_role 

   int login_filled = 0;

   if(roles_users != NULL)
   {
        strcpy(temp_role, roles_users);
   }

    char *token = strtok(temp_role,"\n");

    free(roles_users);
    do 
    {
        int valid_username = 0;
        int valid_password = 0;
        int valid_role = 0;
        int valid_email = 0;
        int valid_full_name = 0;
        int valid_phone = 0;

        int valid_login_username = 0;
        int vaid_login_password = 0;

        char *info_login = NULL; 

        char *register_info = NULL;

        int edit_mode = 0;
        system("cls");
        for(int i = 0; i < hero_size; i++)
        {
            printf("%d. %s\n", i + 1, hero_options[i]);
        }
        printf("\nEnter your choice: ");
        scanf("%d", &hero_choice);                

        User user = {
                .username = "", 
                .password = "", 
                .full_name = "", 
                .phone = "", 
                .email = "", 
                .role = "", };

        switch(hero_choice)
        {
            // Login
            case 1:
                while (1) 
                {
                    system("cls");
                    printf("-- Login Menu --\n");
                    printf("Please select a data field to input (%d/2):\n%s\n\n", login_filled, info_login);


                    // Data fields with their validation status
                    for (int i = 0; i < 2; i++) 
                    {
                        char *field = (i == 0) ? user.username : user.password;
                        if (strlen(field) > 0) 
                        {
                            printf("%d. %s %s(/)%s\n", i + 1, user_login_members[i], GREEN, RESET);
                        } else 
                        {
                            printf("%d. %s %s(x)%s\n", i + 1, user_login_members[i], RED, RESET);
                        }
                    }

                    printf("3. Login\n\n");
                    printf("4. Forget Password\n");
                    printf("5. Back\n");
                    printf("Enter your choice: ");
                    int login_choice;

                    if (scanf("%d", &login_choice) != 1) {
                        printf("Invalid input. Please enter a valid choice.\n");
                        while (getchar() != '\n'); // Clear the input buffer
                        continue;
                    }
                    getchar(); // Clear the newline left by `scanf`

                    int valid_username = 0;
                    int valid_password = 0;
                    int convert_id = 0;

                    switch (login_choice) {
                        case 1: // Username Input
                            while (!valid_username) 
                            {
                                printf("\nPress %s'ESC'%s to cancel.\n", BOLD_GREEN, RESET);
                                printf("Enter Username or ID: ");
                                char user_input[50];
                                fgets(user_input, sizeof(user_input), stdin);
                                user_input[strcspn(user_input, "\n")] = 0;
                                
                                // press ESC TO go back? <- Implement this one.
                                // i have to somehow stop scanning first before reading getch()

                                if (is_numeric(user_input)) { // Check if input is numeric
                                    convert_id = atoi(user_input);
                                    if (valid_user_id(convert_id)) 
                                    {
                                        valid_username = 1;
                                        strcpy(user.username, user_input); // Store user ID as username
                                        login_filled++;
                                    } else {
                                        printf("Invalid ID. Please try again.\n");
                                    }
                                } else {
                                    if (strlen(user_input) < 4 || strlen(user_input) > 12) 
                                    {
                                        printf("Username must be 4-12 characters long.\n");
                                        continue;
                                    }

                                    if (username_exist(user_input) == 0) 
                                    {
                                        valid_username--;
                                        valid_username = 1;
                                        strcpy(user.username, user_input); // Store the username
                                        login_filled++;
                                    } else 
                                    {
                                        printf("[INFO]: Username not found.\n");
                                    }
                                }
                            }
                            break;

                        case 2: // Password Input
                            while (!valid_password) 
                            {
                                char temp[50];
                                printf("Enter Password: ");
                                fgets(temp, sizeof(temp), stdin);
                                temp[strcspn(temp, "\n")] = 0;

                                if (strlen(temp) < 6) 
                                {
                                    printf("Password must be at least 6 characters long.\n");
                                    continue;
                                }

                                printf("Confirm Password: ");
                                fgets(user.password, sizeof(user.password), stdin);
                                user.password[strcspn(user.password, "\n")] = 0;

                                if (strcmp(temp, user.password) == 0) 
                                {
                                    valid_password = 1;
                                    login_filled++;
                                    printf("Password successfully set.\n");
                                } else 
                                {
                                    printf("Passwords do not match. Please try again.\n");
                                }
                            }
                            break;

                        case 3: // Login
                            if (login_filled != 2) 
                            {
                                info_login = "Please ensure both fields are filled before logging in.\n";
                            }

                            if (login_user(user.username, user.password)) 
                            {
                                char stay_logged_in[2];
                                while (1) 
                                {
                                    printf("\nDo you want to stay logged in? (Y/N): ");
                                    fgets(stay_logged_in, sizeof(stay_logged_in), stdin);
                                    
                                    if(strcasecmp(stay_logged_in, "y") == 0)
                                    {
                                        int convert_id = 0;
                                        
                                        if(strlen(user.username) > 3)
                                        {
                                            convert_id = get_id_username(user.username);
                                        }

                                        convert_id = atoi(user.username);

                                        update_user(convert_id, "signed_in", strcasecmp(stay_logged_in, "y") == 0 ? "1" : "0");
                                        
                                        if (strcmp(read_user(convert_id, "role"), "client") == 0) 
                                        {
                                            client_menu(convert_id);
                                            break;
                                        } else 
                                        {
                                            printf("Organizer Dashboard\n");
                                        }
                                        break;
                                    }
                                }
                            } else 
                            {
                                valid_password = 0;
                                valid_username = 0;


                                info_login = "Please make sure your credentials are correct.";
                            }
                            break;
                        case 4:
                            break;
                        case 5:
                            auth_menu();
                            break;
                        default:
                            info_login = "Invalid choice. Please try again.\n";
                    }
                }

                
            // register
            case 2:
                // but if it's equal to 6 then continue?
                while(1) // hmmm if it's equal to 6
                {
                    system("cls");
                    
                    printf("-- Register Menu --\n");
                    printf("Please enter the remaining information (%d of 6 fields completed).\n%s\n\n", data_filled, register_info);    

                    

                    for (int i = 0; i < 6; i++)  
                    {
                        char *field = NULL;
                        switch (i) 
                        {
                            case 0: field = user.username; break;
                            case 1: field = user.password; break;
                            case 2: field = user.full_name; break;
                            case 3: field = user.phone; break;
                            case 4: field = user.email; break;
                            case 5: field = user.role; break;
                        }

                        if (field && strlen(field) > 0) 
                        {
                            printf("%d. %s %s(/)%s\n", i + 1, user_members[i], GREEN, RESET);
                        } else 
                        {
                            printf("%d. %s %s(x)%s\n", i + 1, user_members[i], RED, RESET);
                        }
                    }

                    printf("7. Cancel Registration\n");
                    if(edit_mode == 1)
                    {
                        printf("8. Finish Editing\n");
                    }
                    int register_choice;
                    printf("\nEnter Option: ");
                    scanf("%d", &register_choice);
                    getchar();


                    int count_organizer = 0;
                    char cancel_confirm[4];
                    
                    printf("%d\n", valid_password);
                    
                    if(edit_mode == 1 || data_filled <= 6)
                    {
                        switch (register_choice) 
                        {
                            case 1: 
                                // Username
                                while (!valid_username || edit_mode == 1) 
                                {
                                    printf("\nEnter Username: ");
                                    fgets(user.username, sizeof(user.username), stdin);
                                    user.username[strcspn(user.username, "\n")] = 0;  // Remove trailing newline

                                    size_t len = strlen(user.username);
                                    if (len < 4 || len > 12) 
                                    {
                                        printf("Invalid length! Username must be 4-12 characters.\n");
                                        continue;
                                    }

                                    int has_space = 0, has_special_char = 0;
                                    for (size_t i = 0; i < len; i++) 
                                    {
                                        if (isspace(user.username[i])) 
                                        {
                                            has_space = 1;
                                        }
                                        if (!isalnum(user.username[i])) 
                                        {
                                            has_special_char = 1;
                                        }
                                    }

                                    if (has_space) 
                                    {
                                        printf("Username cannot contain spaces.\n");
                                    } else if (has_special_char) 
                                    {
                                        printf("Username cannot contain special characters.\n");
                                    } else if (username_exist(user.username) == 0)
                                    {
                                    }
                                    else 
                                    {
                                        if(valid_username == 1)
                                        {
                                            register_info = "\nUsername Succesffully Updated.";
                                            edit_mode = 0;
                                        } else {
                                            valid_username = 1;
                                            data_filled += 1;
                                            register_info = "\nUsername Succesffully set.";
                                        }
                                    }
                                }
                                break;                        
                            case 2: 
                                // Password
                                // Implement Confirmation
                                while (!valid_password || edit_mode == 1) 
                                {
                                    char temp[50];

                                    printf("Enter Password: ");
                                    fgets(temp, sizeof(temp), stdin);
                                    temp[strcspn(temp, "\n")] = 0;  // Remove trailing newline

                                    if (strlen(temp) < 6) 
                                    {
                                        printf("\nPassword must be at least 6 characters long.\n");
                                        continue;
                                    }

                                    // printf("Temp: %s\n", temp);

                                    printf("Confirm Password: ");
                                    fgets(user.password, sizeof(user.password), stdin);
                                    user.password[strcspn(user.password, "\n")] = 0;  // Remove trailing newline

                                    // printf("New: %s\n", user.password);

                                    // printf("%d\n", strcmp(temp, user.username));
                                    if (strcmp(temp, user.password) == 0) 
                                    { // Compare the passwords

                                         // Mark the password as valid
                                        if(valid_password == 1)
                                        {
                                            register_info = "\nPassword Succesffully Updated.";
                                            edit_mode = 0;
                                        } else {
                                            valid_password = 1;
                                            data_filled += 1;
                                            register_info = "\nPassword Succesffully set.";
                                        }
                                                                    
                                    } else 
                                    {
                                        printf("\nPasswords do not match. Please try again.\n");
                                        continue;
                                    }                           
                                }
                                break;
                            
                            case 3: 
                                // Full Name

                                while (!valid_full_name || edit_mode == 1) 
                                {
                                    printf("Enter Full Name (First Middle Last): ");
                                    fgets(user.full_name, sizeof(user.full_name), stdin);
                                    user.full_name[strcspn(user.full_name, "\n")] = 0;  // Remove trailing newline

                                    int word_count = 0;
                                    // Left value of the delimiter " "
                                    char *token = strtok(user.full_name, " ");
                                    while (token) 
                                    {
                                        word_count++;
                                        // right value of the delimiter " "
                                        token = strtok(NULL, " ");
                                    }

                                    if (word_count < 3) 
                                    {
                                        printf("\nFull name must include first, middle, and last names.\n");
                                    } else {
                                        if(valid_full_name == 1)
                                        {
                                            register_info = "\nFull Name Succesffully Updated.";
                                            edit_mode = 0;
                                        } else {
                                            valid_full_name = 1;
                                            data_filled += 1;
                                            register_info = "\nFull Name Succesffully set.";
                                        }
                                    }
                                }
                                break;
                            
                            case 4: 
                                // Phone Number
                                
                                while (!valid_phone || edit_mode == 1) 
                                {
                                    printf("Enter Phone Number (e.g., 09123456789): ");
                                    fgets(user.phone, sizeof(user.phone), stdin);
                                    user.phone[strcspn(user.phone, "\n")] = 0;  

                                    if (strlen(user.phone) == 11 && user.phone[0] == '0' && user.phone[1] == '9') 
                                    {
                                        if(valid_phone == 1)
                                        {
                                            register_info = "\nPhone Succesffully Updated.";
                                            edit_mode = 0;
                                        } else 
                                        {
                                            valid_phone = 1;
                                            data_filled += 1;
                                            register_info = "\nPhone Name Succesffully set.";
                                        }
                                    } else 
                                    {
                                        printf("\nInvalid phone number. Must be 11 digits starting with '09'.\n");
                                    }
                                }
                                break;
                            
                            case 5: 
                                // Email
                                
                                while (!valid_email || edit_mode == 1) 
                                {
                                    printf("Enter Email: ");
                                    fgets(user.email, sizeof(user.email), stdin);
                                    user.email[strcspn(user.email, "\n")] = 0;  

                                    if(strcmp(user.email, "@") != 0)
                                    {
                                        if (strchr(user.email, '@') != NULL) 
                                        {
                                            if(valid_email == 1)
                                            {
                                                register_info = "\nEmail Succesffully Updated.";
                                                edit_mode = 0;
                                            } else 
                                            {
                                                valid_email = 1;
                                                data_filled += 1;
                                                register_info = "\nEmail Name Succesffully set.";
                                            }
                                        } else 
                                        {
                                            printf("\nInvalid email! Must contain '@'.\n");
                                        }
                                    }
                                }
                                break;
                            
                            case 6:
                                // role 
                                while(token)
                                {
                                    if(strcmp(token, "organizer") == 0 && count_organizer != 1)
                                    {
                                        count_organizer += 1;
                                    }
                                    token = strtok(NULL ,"\n");
                                    break;
                                }

                                while (!valid_role || edit_mode == 1) 
                                {
                                    printf("Enter Role (client/organizer): ");
                                    fgets(user.role, sizeof(user.role), stdin);
                                    user.role[strcspn(user.role, "\n")] = 0;  

                                    // check for count of organ

                                    // check if the count is == 1.
                                    if (strcmp(user.role, "client") == 0 || strcmp(user.role, "organizer") == 0) 
                                    {
                                        if(strcmp(user.role, "organizer") == 0 && count_organizer == 1)
                                        {
                                            // usa ra pwede organizer.
                                            printf("\nYou can't create more user with this role: Organizer\n");
                                            continue;
                                        }
                                        if(valid_role == 1)
                                        {
                                            register_info = "\nRole Succesffully Updated.";
                                            edit_mode = 0;
                                        } else 
                                        {
                                            valid_role = 1;
                                            data_filled += 1;
                                            register_info = "\nRole Name Succesffully set.";
                                        }
                                    } else 
                                    {
                                        printf("\nInvalid role! Please enter 'client' or 'organizer'.\n");
                                    }
                                }
                                break;
                            case 7:
                                printf("\nDo you wish to cancel your registration? Y / N: ");
                                scanf("%s", cancel_confirm);

                                if(strcasecmp(cancel_confirm, "y") == 0)
                                {
                                    if(sizeof(user) > 0)
                                    {
                                        memset(&user, 0, sizeof(user));
                                        auth_menu();
                                        break;
                                    }
                                } else if (strcasecmp(cancel_confirm, "n") == 0)
                                {
                                    continue;
                                } 
                                return -1;
                            case 8:
                                if(edit_mode == 1)
                                {
                                    edit_mode = 0;
                                } else 
                                {
                                    printf("\nInvalid choice. Please select a valid option.\n");
                                }
                                break;
                            default:
                                printf("\nInvalid choice. Please select a valid option.\n");
                                break;
                        }
                    }
                                    
                    if(data_filled == 6 && edit_mode == 0)
                    {
                        system("cls");
                        int registration_confirmation;
                        
                        printf("-- Review Registration Information --\n\n");
                        printf("Username: %s\n", user.username);
                        printf("Full Name: %s\n", user.full_name);
                        printf("Phone Number: %s\n", user.phone);
                        printf("Role: %s\n\n", user.role);

                        printf("1. Confirm Registration\n");
                        printf("2. Edit Information \n");
                        scanf("%d", &registration_confirmation);

                        switch(registration_confirmation)
                        {
                            case 1:
                                user.id = generate_user_id();

                                char registration_confirm[4];

                                printf("\nDo you wish to confirm your registration? %sY%s / %sN%s?: ", UNDERLINE, RESET, UNDERLINE, RESET);
                                scanf("%s", registration_confirm);

                                if(strcasecmp(registration_confirm, "y") == 0 || strcasecmp(registration_confirm, "n") == 0)
                                {
                                    if(strcasecmp(registration_confirm, "y") == 0)
                                    {
                                        register_user(user);

                                        char stay_logged_in[4];
                                        printf("Do you want to stay Logged In? (Y / N:) ");
                                        scanf("%s", stay_logged_in);
                                        
                                        char *stay_log_in = "";

                                        stay_log_in = strcasecmp(stay_logged_in, "y")  == 0 ? "1" : "0";

                                        
                                        if(strcasecmp(stay_logged_in, "y") == 0)
                                        {
                                            
                                            update_user(user.id, "signed_in", stay_log_in);
                                            
                                            // do not forget to empty user struct kay if ever mo balik si user para ma register.
                                            data_filled = 0;
                                            if(strcmp(user.role, "client") == 0)
                                            {
                                                // client_menu(int client_id);
                                                client_menu(user.id);
                                                memset(&user, 0, sizeof(user));
                                                return 0;
                                                // proceed client dashboard
                                            } else if(strcmp(user.role, "organizer") == 0)
                                            {
                                                
                                                printf("ORGANIZER");
                                                memset(&user, 0, sizeof(user));

                                                break;
                                                // proceed organizer dashboard
                                            }

                                            break;
                                        }
                                        else 
                                        {
                                            printf("\nDo you want to stay Logged In? \n(Y / N:)\n ");
                                            continue;
                                        }
                                        
                                    } else 
                                    {
                                        continue;
                                    }
                                } else 
                                {
                                    printf("Do you wish to confirm your registration? Y / N?:\n ");
                                    continue;
                                }
                                break;
                            case 2:
                                edit_mode += 1;
                                break;
                            default:
                                continue;
                        }
                    }
                }
                break;
              
            case 3:
                return -1;    
        }


    } while (1);
}
int client_menu(int client_id)
{
    int client_choice;

    char client_options[7][50] = {"Profile", "Book Event", "My Events", "View Packages", "Feedback & Support", "Notifications", "Logout"};

    while(1)
    {
        system("cls");
        printf("WELCOME TO EVENT EASE\n");
        printf("Welcome, %s ! Here's a quick overview of your events and options.\n\n", read_user(client_id, "username"));

        // up coming events

        // notifcation

        // options

        // -- Options --

        for(int i = 0 ; i < 7; i ++)
        {
            printf("%d. %s\n", i + 1, client_options[i]);
        }

        printf("Enter the number corresponding to your choice: ");
        scanf("%d", &client_choice);

        switch (client_choice) 
        {
            case 1:
                // Profile
                while(1)
                {
                    int choice_profile;
                    printf("-- Profile Menu --");
                    printf("1. View Profile \n");
                    printf("2. Update Profile \n");
                    printf("3. Back to Main Menu\n");
                    scanf("%d", &choice_profile);
                    
                    // View Profile
                    if(choice_profile == 1)
                    {
                        UserField fields[] = 
                        {
                            {"id", "ID"},
                            {"full_name", "Full Name"},
                            {"email", "Email"},
                            {"phone", "Phone"},
                            {"role", "Role"},
                            {"date_created", "Date Created"}
                        };

                        printf("-- User Profile --");
                        size_t field_count = sizeof(fields) / sizeof(fields[0]);

                        for (size_t i = 0; i < field_count; i++) 
                        {
                            const char *value = read_user(client_id, fields[i].key);

                            if (value) 
                            {
                                printf("%-15s: %s\n", fields[i].display_name, value);
                            } else 
                            {
                                printf("%-15s: %s\n", fields[i].display_name, "N/A");
                            }
                        }

                    // Update Profile
                    } else if (choice_profile == 2)
                    {
                        const char *updatable_keys[] = {"username", "full_name", "password", "email", "phone"};
                        const char *updatable_labels[] = {"Username", "Full Name", "Password", "Email", "Phone"};
                        size_t key_count = sizeof(updatable_keys) / sizeof(updatable_keys[0]);

                        // Display updatable options
                        printf("\n-- Update Profile --\n");
                        for (size_t i = 0; i < key_count; i++) 
                        {
                            printf("%zu. %s\n", i + 1, updatable_labels[i]);
                        }

                        int field_choice;
                        printf("Enter the number of the field you'd like to update: ");
                        scanf("%d", &field_choice);
                        getchar(); 
                        if (field_choice < 1 || field_choice > (int)key_count) 
                        {
                            printf("Invalid choice. Returning to profile menu.\n");
                            return;
                        }

                        const char *selected_key = updatable_keys[field_choice - 1];
                        const char *selected_label = updatable_labels[field_choice - 1];

                        char new_value[100];
                        char confirm_value[100];

                        printf("Enter new %s: ", selected_label);
                        fgets(new_value, sizeof(new_value), stdin);
                        new_value[strcspn(new_value, "\n")] = 0; 

                        if (strcmp(selected_key, "password") == 0) 
                        {

                            printf("Confirm new Password: ");
                            fgets(confirm_value, sizeof( confirm_value), stdin);
                            confirm_value[strcspn(confirm_value, "\n")] = 0;

                            if (strcmp(new_value, confirm_value) != 0) 
                            {
                                printf("Passwords do not match. Update canceled.\n");
                            }
                        }

                        if (update_user(client_id, selected_key, new_value) == 1) 
                        {
                            printf("%s updated successfully!\n", selected_label);
                        } else 
                        {
                            printf("Failed to update %s. Please try again.\n", selected_label);
                        }
                    } else if(choice_profile == 3)
                    {
                        client_menu(client_id);
                    } else 
                    {
                        printf("Please select a valid option:\n");
                        continue;
                    }
                }
                break;
            case 2:
                // Book Event
                break;
            case 3:
                // My Events
                break;
            case 4:
                // View Packages
                break;
            case 5:
                // Feedback & Support
                break;
            case 6:
                // Notifications
                break;
            case 7:
                // Logout
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 8.\n");
                break;
        }
    }
}

/* int organizer_menu()
{
    
} */