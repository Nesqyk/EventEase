
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <direct.h>
#include <windows.h>
#include "menu.h"
#include "organizer.h"
#include <time.h>
#define MAX_INPUT_SIZE 50
#define PAGE_SIZE 20

/*
 * Function: clear_input_buffer
 * Clears the input buffer to avoid unwanted characters from interfering
 * with subsequent input operations.
 */
void clear_input_buffer() 
{
    while (getchar() != '\n');
}

/*
 * Function: display_options
 * Displays a header and a list of options for the user to choose from.
 * The function dynamically prints the options provided.
 */
void display_options(const char *header, const char *paragraph[], const char *options[], const char choice[100]) 
{
    system("cls");
    if (header != NULL) 
    {
        printf("-- %s --\n\n", header);
    }

    if (paragraph != NULL) 
    {
        for (size_t i = 0; paragraph[i] != NULL; i++) 
        {
            printf("%s\n", paragraph[i]);
        }
        printf("\n");
    }

    if (options != NULL) 
    {
        for (size_t i = 0; options[i] != NULL; i++) 
        {
            printf("%zu. %s\n", i + 1, options[i]);
        }
    }

    printf("\n==================================================\n");


    if (choice == NULL)
    {
        printf("\n👉 Enter your choice: ");
    } 
    else 
    {
        printf("\n👉 %s ", choice);
    }
}


/*
 * Function: get_password
 * Reads a password input from the user and masks all characters
 * except the last one, which is displayed for visual feedback.
 */
void get_password(char *password, size_t max_length) 
{
    size_t index = 0;
    char ch;

    while (index < max_length - 1) {
        ch = getch(); // Read a single character without echoing
        if (ch == '\r') { // Enter key pressed
            break;
        } else if (ch == '\b' && index > 0) { // Backspace pressed
            printf("\b \b");
            index--;
        } else if (isprint(ch)) { // Add visible characters to password
            password[index++] = ch;

            // Mask all characters except the last
            if (index == 1) {
                printf("%c", ch);
            } else {
                printf("\b*%c", ch);
            }
        }
    }
    password[index] = '\0';
    printf("\n");
}


/*
 * Function: auth_menu
 * Displays the main authentication menu with options to login, register,
 * or quit the program.
 */
int auth_menu() 
{
    const char *menu_options[] = {"Login", "Register", "Quit", NULL};
    const char *paragraph[] = {
        "==================================================",
        "            \U00002728 Welcome to EventEase! \U00002728",
        "==================================================",
        "Your one-stop solution for seamless event management.",
        "",
        "\U0001F44B First time here? Create an account in seconds!",
        "\U0001F511 Already have an account? Log in to access your dashboard.",
        "",
        "Features include:",
        "\U00002705 Effortless event booking",
        "\U00002705 Real-time notifications",
        "\U00002705 Personalized dashboard",
        "",
        "What would you like to do today?",
        "==================================================",
        NULL
    };

    int choice;

    // Check last login status
    char *stay_logged_in = read_last_login("stay_logged_in");
    char *last_user_role = read_last_login("role");
    char *last_user_id = read_last_login("id");

    if (stay_logged_in != NULL && strcmp(stay_logged_in, "1") == 0) 
    {
        printf("Welcome back! Redirecting to your dashboard...\n");
        Sleep(2000);

        if (last_user_role != NULL && strcmp(last_user_role, "client") == 0) 
        {
            client_menu(atoi(last_user_id));
        } 
        else if (last_user_role != NULL && strcmp(last_user_role, "organizer") == 0) 
        {
            organizer_menu(atoi(last_user_id));
        }

        free(stay_logged_in);
        free(last_user_role);
        free(last_user_id);
        return 0; // Exit authentication menu
    }

    // Normal authentication flow
    while (1) 
    {

        display_options(NULL, paragraph, menu_options, NULL);
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) 
        {
            printf("Invalid input. Please enter a number between 1 to 3.\n");
            Sleep(2000);
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) 
        {
            case 1:
                login_menu();
                break;
            case 2:
                register_menu();
                break;
            case 3:
                printf("Exiting program. Goodbye!\n");
                return 0;
        }
    }
}

/*
 * Function: login_menu
 * Handles user login by prompting for username and password,
 * and validating credentials.
 */
int login_menu() 
{
    char username[MAX_INPUT_SIZE] = "", password[MAX_INPUT_SIZE] = "";

    while (1) 
    {
        char username_option[256], password_option[256];
        sprintf(username_option, "Enter Username or ID %s[%s]%s", strlen(username) > 0 ? GREEN : RED, strlen(username) > 0 ? "Filled" : "\U0000274C Empty", RESET);
        sprintf(password_option, "Enter Password %s[%s]%s", strlen(password) > 0 ? GREEN : RED, strlen(username) > 0 ? "Filled" : "\U0000274C Empty", RESET);

        char *login_options[] = {username_option, password_option, "Login\n", "Forgot Password", "Back", NULL};

        system("cls");
        const char *paragraph[] = {
            "==================================================",
            "                     \U0001F512 Login Menu",
            "==================================================",
            "",
            "\U0001F31F Login Guide:",
            "- Enter your **Username or ID** to identify your account.",
            "- Enter your **Password** for authentication.",
            "- Select **Login** to access your account after filling in the fields.",
            "- Forgot your password? Choose **Forgot Password** to reset it.",
            "- To return to the previous menu, select **Back**.",
            "",
            "==================================================",
            "",
            "\U0001F6E0\U0000FE0F Login Menu Options:",
            NULL
        };
        display_options(NULL, paragraph, login_options, NULL);

        int choice;
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) 
        {
            printf("Invalid input. Please enter a valid option (1-5).\n");
            Sleep(2000);
            clear_input_buffer();
            continue;
        }

        

        clear_input_buffer();

        switch (choice) 
        {
            case 1:
                while (1) 
                {
                    printf("\nEnter Username or ID (Press ESC to cancel): ");

                    fgets(username, MAX_INPUT_SIZE, stdin);
                    username[strcspn(username, "\n")] = 0;

                    if (strlen(username) == 0) 
                    {
                        printf("\nInput cannot be empty. Please try again.\n");
                        continue;
                    }

                    if (is_numeric(username)) 
                    {
                        if (!valid_user_id(atoi(username))) 
                        {
                            printf("\nInvalid ID. Please enter a valid user ID.\n");
                            continue;
                        }
                        break; // Valid ID
                    } 
                    else 
                    {
                        if (!username_exist(username)) 
                        {
                            printf("\nUsername does not exist. Please try again.\n");
                            continue;
                        }
                        break; // Valid username
                    }
                }
                break;

            case 2:
                printf("\nEnter Password: ");
                get_password(password, MAX_INPUT_SIZE);
                break;

            case 3:
                if (strlen(username) == 0 || strlen(password) == 0) 
                {
                    printf("\nBoth username and password are required. Please try again.\n");
                    Sleep(2000);
                    continue;
                }

                if (login_user(username, password)) 
                {
                    printf("\nLogin successful!\n");
                    Sleep(1000);

                    int user_id = is_numeric(username) ? atoi(username) : get_id_username(username);
                    if (user_id <= 0) 
                    {
                        printf("\nError: Invalid user ID. Please contact support.\n");
                        return;
                    }

                    char *stay_logged_in = read_user(user_id, "stay_logged_in");
                    if (stay_logged_in) 
                    {
                        if (strcmp(stay_logged_in, "0") == 0) 
                        {
                            while (1) 
                            {
                                char confirmation[4];
                                printf("\nDo you want to stay logged in? (Y/N): ");
                                fgets(confirmation, sizeof(confirmation), stdin);
                                confirmation[strcspn(confirmation, "\n")] = 0;

                                if (strcasecmp(confirmation, "Y") == 0) 
                                {
                                    update_user(user_id, "stay_logged_in", "1");
                                    printf("\nYou will now stay logged in.\n");
                                    Sleep(1000);
                                    break;
                                } 
                                else if (strcasecmp(confirmation, "N") == 0) 
                                {
                                    update_user(user_id, "stay_logged_in", "0");
                                    printf("\nYou chose not to stay logged in.\n");
                                    Sleep(1000);
                                    break;
                                } 
                                else 
                                {
                                    printf("\nInvalid option. Please enter 'Y' or 'N'.\n");
                                    Sleep(2000);
                                }
                            }
                        }
                        free(stay_logged_in);
                    } 
                    else 
                    {
                        printf("\nError: Unable to determine stay logged-in preference.\n");
                    }

                    const char *role = read_user(user_id, "role");
                    if (role) 
                    {
                        if (strcmp(role, "client") == 0) 
                        {
                            client_menu(user_id);
                        } 
                        else if (strcmp(role, "organizer") == 0) 
                        {
                            organizer_menu(user_id);
                        } 
                        else 
                        {
                            printf("\nInvalid role detected. Please contact support.\n");
                        }
                    } 
                    else 
                    {
                        printf("\nError: Unable to determine user role. Please contact support.\n");
                    }
                    return;
                } 
                else 
                {
                    printf("\nInvalid credentials. Please try again.\n");
                    Sleep(2000);
                }
                
                break;

            case 4:
                if (strlen(username) == 0) 
                {
                    printf("\nlease enter your username or ID first.\n");
                    Sleep(2000);
                } 
                else 
                {
                    reset_password(username);
                }
                break;

            case 5:
                printf("\nReturning to the previous menu...\n");
                if(strlen(password) > 0 || strlen(username) > 0)
                {
                    memset(&password, 0, sizeof(password));
                    memset(&username, 0, sizeof(username));

                }
                Sleep(1000);
                return;
        }
    }
}


/*
 * Function: reset_password
 * Allows users to reset their password after providing a username.
 * The password is updated only if the new and confirmation passwords match.
 */
void reset_password(const char *username) 
{
    if (strlen(username) == 0) 
    {
        printf("No username provided. Please enter your username first in the login menu.\n");
        Sleep(2000);
        return;
    }

    char new_password[MAX_INPUT_SIZE], confirm_password[MAX_INPUT_SIZE];
    printf("-- Reset Password --\n\n");
    printf("Enter new password: ");
    get_password(new_password, MAX_INPUT_SIZE);

    printf("Confirm new password: ");
    get_password(confirm_password, MAX_INPUT_SIZE);

    if (strcmp(new_password, confirm_password) == 0) 
    {
        int user_id = is_numeric(username) ? atoi(username) : get_id_username(username);
        if (update_user(user_id, "password", new_password)) 
        {
            printf("Password successfully updated.\n");
        } else 
        {
            printf("Failed to update password. Please try again later.\n");
        }
    } else 
    {
        printf("Passwords do not match. Please try again.\n");
    }
    Sleep(2000);
}

/*
 * Function: register_menu
 * Handles user registration by collecting user details such as username,
 * password, and role. Ensures only one organizer account is allowed.
 */
void register_menu() 
{
    const char *fields[] = {"Username", "Password", "Full Name", "Phone Number", "Email", "Select Role"};
    char input[6][MAX_INPUT_SIZE] = {""}; // Array to store user inputs
    User new_user = {0};

    while (1) 
    {
        system("cls");

        int filled_fields = 0;
        for (int i = 0; i < 6; i++) 
        {
            if (strlen(input[i]) > 0) 
                filled_fields++;
        }

        char progress[50];

        sprintf(progress, "Registration Progress: [%.*s%.*s] (%d / 100%%)", filled_fields, "#######", 7 - filled_fields, "        ", (filled_fields * 100) / 6);

        const char *paragraph[] = {
            "==================================================",
            "              \U0001F4DD Registration Menu",
            "==================================================",
            "",
            "\U0001F31F Join EventEase - Simplify Your Events!",
            "- Create an account to start managing events effortlessly.",
            "- Fill out all required fields to complete your registration.",
            "",
            "==================================================",
            "",
            progress,
            "",
            "\U0001F6E0 Registration Fields:",
            NULL 
        };

        // Progress Indicator
        

        char *options[10];
        char masked_password[50]; // meaning this is unedfined

        if (strlen(input[1]) > 0) 
        {
            for (int i = 0; i < strlen(input[1]); i++) 
            {
                masked_password[i] = '*';
            }
            masked_password[strlen(input[1])] = '\0'; // Null-terminate
        }
        
        // Display Form
        for (int i = 0; i < 6; i++) 
        {
            options[i] = malloc(100);
            if(options[i] != NULL)
                sprintf(options[i],"%s\t[%s%s%s] : %s", fields[i], strlen(input[i]) > 0 ? GREEN : RED, strlen(input[i]) > 0 ? "Filled" : "Empty", RESET, i == 1 ? masked_password: input[i]);
            if(i == 5)
            {
                strcat(options[i] , "\n");
            }
        }
        options[6] = "Submit";
        options[7] = "Cancel";
        options[8] = NULL;
        
        display_options(NULL, paragraph, options,NULL);

        int choice;
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 8) 
        {
            printf("\nInvalid input. Please enter a valid option (1-8).\n");
            Sleep(2000);
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 8) // Cancel
        {
            memset(&new_user, 0, sizeof(new_user));
            memset(input, 0, sizeof(input));
            printf("\nRegistration canceled. All fields have been reset.\n");
            Sleep(2000);
            return;
        }

        if (choice == 1) // Username
        {
            printf("\nEnter Username: ");
            fgets(input[0], MAX_INPUT_SIZE, stdin);
            input[0][strcspn(input[0], "\n")] = 0; // Remove trailing newline

            if (strlen(input[0]) == 0) 
            {
                printf("\nUsername cannot be empty. Please try again.\n");
                Sleep(2000);
                memset(input[0], 0, MAX_INPUT_SIZE);
                continue;
            } 
            else if (username_exist(input[0])) 
            {
                printf("\nUsername already exists. Please try a different one.\n");
                Sleep(2000);
                memset(input[0], 0, MAX_INPUT_SIZE);
                continue;
            }
        }

        else if (choice == 2) // Password
        {
            while (1)
            {
                printf("\nEnter Password: ");
                get_password(input[1], MAX_INPUT_SIZE);

                // Password strength checks
                int has_upper = 0, has_digit = 0, has_special = 0;
                for (size_t i = 0; i < strlen(input[1]); i++) 
                {
                    if (isupper(input[1][i]))
                        has_upper = 1;
                    else if (isdigit(input[1][i]))
                        has_digit = 1;
                    else if (ispunct(input[1][i]))
                        has_special = 1;
                }

                if (strlen(input[1]) < 6 || !has_upper || !has_digit || !has_special) 
                {
                    printf("\nError: Password must be at least 6 characters long, contain an uppercase letter, a digit, and a special character.\n");
                    Sleep(2000);
                    memset(input[1], 0, MAX_INPUT_SIZE);
                    continue;
                }

                // Confirm password
                char confirm_password[MAX_INPUT_SIZE];
                printf("Confirm Password: ");
                get_password(confirm_password, MAX_INPUT_SIZE);

                if (strcmp(input[1], confirm_password) != 0) 
                {
                    printf("\nError: Passwords do not match. Please try again.\n");
                    Sleep(2000);
                    memset(input[1], 0, MAX_INPUT_SIZE);
                } 
                else 
                {
                    printf("\nPassword set successfully.\n");
                    break;
                }
            }
        }

        else if (choice == 3) // Full Name
        {
            while (1)
            {
                printf("\nEnter Full Name: ");
                fgets(input[2], MAX_INPUT_SIZE, stdin);
                input[2][strcspn(input[2], "\n")] = 0;

                if (strlen(input[2]) == 0) 
                {
                    printf("\nError: Full Name cannot be empty.\n");
                    Sleep(2000);
                    memset(input[2], 0, MAX_INPUT_SIZE);
                } 
                else 
                {
                    break;
                }
            }
        }

        else if (choice == 4) // Phone Number
        {
            while (1)
            {
                printf("\nEnter Phone Number: ");
                fgets(input[3], MAX_INPUT_SIZE, stdin);
                input[3][strcspn(input[3], "\n")] = 0;

                if (strlen(input[3]) != 11 || strncmp(input[3], "09", 2) != 0 || !isdigit(input[3][2])) 
                {
                    printf("\nError: Invalid phone number. Must start with '09' and be 11 digits long.\n");
                    Sleep(2000);
                    memset(input[3], 0, MAX_INPUT_SIZE);
                } 
                else 
                {
                    break;
                }
            }
        }

        else if (choice == 5) // Email
        {
            while (1)
            {
                printf("\nEnter Email: ");
                fgets(input[4], MAX_INPUT_SIZE, stdin);
                input[4][strcspn(input[4], "\n")] = 0; // Remove trailing newline

                // Ensure email contains '@' and '.'
                char *at_symbol = strchr(input[4], '@');
                char *dot_symbol = strrchr(input[4], '.');

                if (!at_symbol || !dot_symbol || at_symbol == input[4] || strlen(dot_symbol + 1) < 2 || dot_symbol < at_symbol + 2) 
                {
                    printf("\nError: Invalid email format. Please ensure it contains '@' and a valid domain.\n");
                    Sleep(2000);
                    memset(input[4], 0, MAX_INPUT_SIZE);
                } 
                else 
                {
                    break;
                }
            }
        }

        else if (choice == 6) // Role
        {
            while (1)
            {
                printf("\nEnter Role (client/organizer): ");
                fgets(input[5], MAX_INPUT_SIZE, stdin);
                input[5][strcspn(input[5], "\n")] = 0;

                if (strcmp(input[5], "client") != 0 && strcmp(input[5], "organizer") != 0) 
                {
                    printf("\nError: Invalid role. Please choose 'client' or 'organizer'.\n");
                    Sleep(2000);
                    memset(input[5], 0, MAX_INPUT_SIZE);
                    continue;
                } 
                else if (strcmp(input[5], "organizer") == 0) 
                {
                    char *roles_users = read_all_user_by_key("role");
                    if (roles_users && strstr(roles_users, "organizer")) 
                    {
                        printf("\nError: An organizer account already exists. Please choose a different role.\n");
                        Sleep(2000);
                        memset(input[5], 0, MAX_INPUT_SIZE);
                        free(roles_users);
                        continue;
                    }
                    free(roles_users);
                }
                break;
            }
        }
        else if (choice == 7) // Submit
        {
            if (strlen(input[0]) == 0 || strlen(input[1]) == 0 || strlen(input[2]) == 0) 
            {
                printf("\nError: Please complete all required fields before submitting.\n");
                Sleep(2000);
                continue;
            }

            new_user.id = generate_user_id();
            strcpy(new_user.username, input[0]);
            strcpy(new_user.password, input[1]);
            strcpy(new_user.full_name, input[2]);
            strcpy(new_user.phone, input[3]);
            strcpy(new_user.email, input[4]);
            strcpy(new_user.role, input[5]);

            if (register_user(new_user)) 
            {
                printf("\nRegistration successful! Welcome to EventEase.\n");
                Sleep(2000);

                while (1) 
                {
                    char confirmation[4];
                    printf("\nDo you want to stay logged in? (Y/N): ");
                    fgets(confirmation, sizeof(confirmation), stdin);
                    confirmation[strcspn(confirmation, "\n")] = 0;

                    if (strcasecmp(confirmation, "Y") == 0) 
                    {
                        update_user(new_user.id, "stay_logged_in", "1");
                        printf("\nYou will now stay logged in.\n");
                        Sleep(1000);
                    } 
                    else if (strcasecmp(confirmation, "N") == 0) 
                    {
                        update_user(new_user.id, "stay_logged_in", "0");
                        printf("\nYou chose not to stay logged in.\n");
                        Sleep(1000);
                    } 
                    else 
                    {
                        printf("\nInvalid option. Please enter 'Y' or 'N'.\n");
                        Sleep(2000);
                    }

                    printf("\nRedirecting to Dashboard...\n");
                    Sleep(1000);
                    

                    const char *role = read_user(new_user.id, "role");
                    if (strcmp(role, "client") == 0) 
                    {
                        client_menu(new_user.id);
                    } 
                    else if (strcmp(role, "organizer") == 0) 
                    {
                        organizer_menu(new_user.id);
                    } 
                    else 
                    {
                        printf("\nInvalid role detected. Please contact support.\n");
                    }
                }
            } 
            else 
            {
                printf("\nError: Registration failed. Please try again later.\n");
                Sleep(2000);
            }
        }
    }
}


/*
 * Function: review_registration
 * Displays the user's entered details and allows them to edit fields,
 * submit their registration, or go back to the previous menu.
 * 
 */
/* void review_registration(char input[6][MAX_INPUT_SIZE], const char *fields[]) {
    while (1) {
        system("cls");
        printf("-- Review Registration Details --\n\n");
        for (int i = 0; i < 6; i++) {
            printf("%d. %s: %s\n", i + 1, fields[i], strlen(input[i]) > 0 ? input[i] : "[Empty]");
        }
        printf("7. Edit\n8. Submit\n9. Back\n\nEnter your choice: ");

        int choice;
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 9) {
            printf("Invalid input. Please enter a valid option.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice >= 1 && choice <= 6) {
            printf("Enter new value for %s: ", fields[choice - 1]);
            fgets(input[choice - 1], MAX_INPUT_SIZE, stdin);
            input[choice - 1][strcspn(input[choice - 1], "\n")] = 0;
        } else if (choice == 8) {
            printf("Registration successful!\n");
            return;
        } else if (choice == 9) {
            return;
        }
    }
} */


int client_menu(int client_id)
{
    int client_choice;
    // initiate log here

    char *client_options[] = {
        "Profile            👤 - Manage your account details.",
        "Book Event         📅 - Plan and schedule new events.",
        "My Events          📚 - View and edit your booked events.",
        "View Packages      📦 - Explore our event packages.",
        "Feedback & Support 💬 - Get assistance or leave feedback.",
        "Notifications      🔔 - Check recent updates and alerts.",
        "Payment History    🧾 - Review all your past and pending payments.",
        "Logout             🔑 - Exit the system securely.",
        NULL // Terminator for iteration
    };


    BookEvent bk_event = {
        .name = "",
        .venue = "",
        .event_date = "",
        .start_time = "",
        .event_type_id = -1,
        .package_id = -1
    };

    char book_input[5][MAX_INPUT_SIZE] = {""}; // Array to store user inputs

    while(1)
    {

        int book_event_filled = 0;
        char *calendar = display_current_month_with_highlight();
        char *prev_reminders = view_reminders(client_id);

        const char *dashboard_paragraph[] = {
            "==================================================",
            "              🌟 EventEase Dashboard 🌟",
            "==================================================",
            "",
            "👋 Welcome back, Hello2024!", // Welcome only after register.
            "Here's your personalized dashboard to manage events effortlessly.",
            "",
            calendar,
            prev_reminders,
            "                  📋 Main Menu                   ",
            "==================================================", NULL
        };

        display_options(NULL, dashboard_paragraph, client_options, NULL);
        
        if(scanf("%d", &client_choice) != 1 || client_choice < 1 || client_choice > 12)
        {
            printf("\nInvalid choice. Please enter a number between 1 and 12.\n");
            Sleep(2000);
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        // Log user choice (Info)
        char choice_message[100];
        sprintf(choice_message, "User selected option %d from the main menu.", client_choice);
        log_event(client_id, 0, choice_message);

        switch (client_choice) 
        {
            case 1:
                // Profile
                log_event(client_id, 0, "User selected 'Profile' option.");

                while(1)
                {
                    system("cls");

                    char *profile_menu[] = {
                        "==================================================",
                        "                🔍 Profile Menu                  ",
                        "==================================================",
                        "📍 Home > Dashboard > Profile",
                        "",
                        "==================================================",
                        NULL // Null terminator for the array
                    };

                    char *profile_choices[] = {
                        "View Profile         \U0001F464 - See your personal details.",
                        "Update Profile       \U0001F4DD - Modify your account information.",
                        "Back to Main Menu    \U0001F519 - Return to the dashboard.",
                        "Delete Account       \U0001F5D1 - Permanently remove your account and data.",
                        NULL
                    };
                    int choice_profile;
                    display_options(NULL,profile_menu, profile_choices, NULL);

                    if(scanf("%d", &choice_profile) != 1 || choice_profile < 1 || choice_profile > 4)
                    {
                        printf("\nInvalid choice. Please enter a number between 1 to 4.\n");
                        Sleep(2000);
                        clear_input_buffer();
                        continue;
                    }
                    clear_input_buffer();
                                
                    // Log profile menu choice (Info)
                    char profile_choice_message[100];
                    sprintf(profile_choice_message, "User selected option %d from the profile menu.", choice_profile);
                    log_event(client_id, 0, profile_choice_message);
                    
                    if(choice_profile == 1)
                    {
                        // View Profile
                        log_event(client_id, 0, "User viewed their profile.");
                        UserField fields[] = 
                        {
                            {"id", "ID"},
                            {"full_name", "Full Name"},
                            {"email", "Email"},
                            {"phone", "Phone"},
                            {"role", "Role"},
                            {"date_created", "Date Created"}
                        };

                        char *view_profile[] = {
                            "==================================================",
                            "                👤 User Profile                   ",
                            "==================================================",
                            "📍 Home > Dashboard > Profile > View",
                            "",
                            "==================================================",
                            NULL
                        };


                        size_t field_count = sizeof(fields) / sizeof(fields[0]);
                        char *buffer_view[field_count + 1];
                        for (size_t i = 0; i < 6; i++) 
                        {
                            buffer_view[i] = malloc(100);
                            const char *value = read_user(client_id, fields[i].key);
                            if (value) 
                            {
                                sprintf(buffer_view[i], "✨ %-15s: \t%s", fields[i].display_name, value);
                            } else 
                            {
                                sprintf(buffer_view[i], "✨ %-15s: \t%s", fields[i].display_name, "N/A");
                            }
                        }
                        buffer_view[field_count] = NULL;
                        display_options(NULL, view_profile, buffer_view,"Press [Any KEY] to stop viewing.");
                        
                        if(getch() == '\r')
                        {
                            continue;
                        }
                    } 
                    else if(choice_profile == 2)
                    {
                        // Update Profile
                        log_event(client_id, 0, "User selected to update their profile.");
                        const char *updatable_keys[] = {"username", "full_name", "password", "email", "phone"};
                        const char *updatable_labels[] = {"Username", "Full Name", "Password", "Email", "Phone"};
                        size_t key_count = sizeof(updatable_keys) / sizeof(updatable_keys[0]);
                        char *update_profile[] = {
                            "==================================================",
                            "               📝 Update Profile                  ",
                            "==================================================",
                            "📍 Home > Dashboard > Profile > Update",
                            "",
                            "==================================================",
                            NULL
                        };

                        char *update_field[] = {
                            "Username          \U0001F4DD - Change your username.",
                            "Full Name         \U0001F4DD - Update your full name.",
                            "Password          \U0001F511 - Set a new password.",
                            "Email             \U0001F4E7 - Update your email address.",
                            "Phone             \U0001F4F1 - Change your contact number.",
                            "Cancel            \U0001F519 - Return to Profile Menu.",NULL
                        };

                        display_options(NULL, update_profile, update_field, "Enter the number of the field you'd like to update:");

                        int field_choice;
                        if(scanf("%d", &field_choice) != 1 || field_choice < 1 || field_choice > 6)
                        {
                            printf("\nInvalid choice. Please enter a number between 1 to 6.\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        if (field_choice < 1 || field_choice > (int)key_count + 1) 
                        {
                            log_event(client_id, 1, "Invalid profile update choice. Returning to profile menu.");
                            printf("Invalid choice. Returning to profile menu.\n");
                            Sleep(500);

                            continue;
                        }

                        if (field_choice == (int)key_count + 1) 
                        {
                            log_event(client_id, 1, "Profile update canceled. Returning to profile menu.");
                            printf("Update canceled. Returning to profile menu.\n");
                            Sleep(500);
                            continue;
                        }

                        const char *selected_key = updatable_keys[field_choice - 1];
                        const char *selected_label = updatable_labels[field_choice - 1];
                        char new_value[100];
                        char confirm_value[100];

                        printf("Enter new %s (type 'cancel' to cancel): ", selected_label);
                        fgets(new_value, sizeof(new_value), stdin);
                        new_value[strcspn(new_value, "\n")] = 0;

                        if (strcmp(new_value, "cancel") == 0) 
                        {
                            log_event(client_id, 1, "Profile update canceled by the user.");
                            printf("Update canceled. Returning to profile menu.\n");
                            continue;
                        }

                        if (strcmp(selected_key, "password") == 0) 
                        {
                            printf("Confirm new Password (type 'cancel' to cancel): ");
                            fgets(confirm_value, sizeof(confirm_value), stdin);
                            confirm_value[strcspn(confirm_value, "\n")] = 0;

                            if (strcmp(confirm_value, "cancel") == 0) 
                            {
                                log_event(client_id, 1, "Profile password update canceled by the user.");
                                printf("Update canceled. Returning to profile menu.\n");
                                return;
                            }

                            if (strcmp(new_value, confirm_value) != 0) 
                            {
                                log_event(client_id, 1, "Password update failed: passwords do not match.");
                                printf("Passwords do not match. Update canceled.\n");
                                return;
                            }
                        }

                        if (update_user(client_id, selected_key, new_value) == 1) 
                        {
                            log_event(client_id, 0, "Profile updated successfully.");
                            printf("%s updated successfully!\n", selected_label);
                        } 
                        else 
                        {
                            log_event(client_id, 2, "Failed to update profile.");
                            printf("Failed to update %s. Please try again.\n", selected_label);
                        }
                    }
                    else if(choice_profile == 3)
                    {
                        log_event(client_id, 0, "User returned to main menu from profile.");
                        client_menu(client_id);
                    }
                    else if(choice_profile == 4)
                    {
                        while(1)
                        {
                            char confirmation[4];
                            system("cls");
                            printf("==================================================\n");
                            printf("               ❌ Delete Account                  \n");
                            printf("==================================================\n\n");
                            printf("⚠️ WARNING: Deleting your account is permanent.\n");
                            printf("   All your data, including events and history,\n");
                            printf("   will be permanently removed and cannot be\n");
                            printf("   recovered.\n");
                            printf("\n");
                            printf("==================================================\n\n");
                            printf("Type 'Y' to confirm or 'N' to cancel: ");
                            scanf("%s", confirmation);

                            if(strcasecmp(confirmation, "y") == 0)
                            {
                                delete_user(client_id);
                                auth_menu();
                            } else if(strcasecmp(confirmation, "n") == 0)
                            {

                            } else 
                            {
                                printf("\nPlease enter a valid option\n");
                                continue;
                            }
                        // print here
                        }
                    }
                    else 
                    {
                        log_event(client_id, 1, "Invalid profile menu choice.");
                        printf("\nPlease select a valid option:\n");
                        Sleep(500);
                        continue;
                    }
                }
                break;
            case 2:
                log_event(client_id, 0, "User selected 'Book Event' option.");

                /*
                Book Event
                    Select Event Type.
                    View Available Packages <- make sure first event type is not empty
                    Confirm. <- make sure that event_type is not empty as well as packages. 
                    Cancel.
                */
                while(1)
                {
                    char progress[50];

                    int steps_completed = 0;

                    for (int i = 0; i < 5; i++) {
                        if (strlen(book_input[i]) > 0) 
                        {
                            steps_completed++;
                        }
                    }

                    if (steps_completed < 5) {
                        sprintf(progress, "Booking Process: [%.*s%.*s] %d%%",
                            (steps_completed * 20) / 5, "####################",
                            20 - (steps_completed * 20) / 5, "--------------------",
                            (steps_completed * 100) / 5);
                    } else {
                        sprintf(progress, "Booking Process: [%s####################%s] 100%%", GREEN, RESET);
                    }
                                        
                    const char *book_event_menu[] = {
                        "==================================================",
                        "               📆 Book Event Submenu              ",
                        "==================================================",
                        "",
                        "🔧 Please fill in the required fields to book your event.",
                        "",
                        "==================================================",
                        "",
                        progress,
                        "",
                        "==================================================",
                        NULL
                    };

                    char book_event_options[5][30] = {"Event Name", "Select Event", "Package", "Select Venue", "Set Date"};

                    size_t input_size = sizeof(book_event_options) / sizeof(book_event_options[0]); // Get the size of event options

                    char *book_options[input_size + 3]; // +2 for "Book Event" and "Cancel", +1 for NULL termination

                    for (int i = 0; i < 5; i++) 
                    {
                        // Allocate sufficient memory for each option
                        book_options[i] = malloc(100); 

                        if (book_options[i] != NULL) 
                        {
                            snprintf(book_options[i], 100, "%s\t\t[%s%s%s]", 
                                    book_event_options[i], 
                                    strlen(book_input[i]) > 0 ? GREEN : RED, 
                                    strlen(book_input[i]) > 0 ? "Filled" : "Empty", 
                                    RESET);
                        } 
                        else 
                        {
                            perror("Memory allocation failed");
                            exit(EXIT_FAILURE);
                        }

                        if(i == 4)
                        {
                            strcat(book_options[i], "\n");
                        }
                    }

                    // let's not forget to free book_options to avoid memory leakage.
                    book_options[input_size] = strdup("Book Event");
                    book_options[input_size + 1] = strdup("Cancel");
                    book_options[input_size + 2] = NULL;

                    int book_event_choice;

                    display_options(NULL, book_event_menu, book_options, "Enter the corresponding number of the field: ");

                    if(scanf("%d", &book_event_choice) != 1 || book_event_choice < 1 || book_event_choice > 7 )
                    {
                        printf("\nPlease select a valid choice from 1 to 7\n");
                        Sleep(2000);
                        clear_input_buffer();
                        continue;
                    }
                    clear_input_buffer();
                    // shud show preview here. 
                
                    if(book_event_choice == 1) // event name
                    {
                        while(1)
                        {
                            printf("\nPlease enter event name (or type '-1' to cancel): ");
                            fgets(book_input[0], sizeof(book_input[0]), stdin);
                            book_input[0][strcspn(book_input[0], "\n")] = 0;


                            if(strcmp(book_input[0], "-1") == 0)
                            {
                                memset(book_input[0], 0, sizeof(book_input[0]));
                                break;
                            }

                            size_t len = strlen(book_input[0]);

                            if(len < 4 || len > 40)
                            {
                                printf("Invalid length! Event Name must be 4-40 characters.\n");
                                continue;
                            } else 
                            {
                                if(book_input[0] == NULL)
                                {
                                    book_event_filled++;
                                }
                                break;
                            }
                        }
                    }
                    else if(book_event_choice == 2) // event type
                    {
                        while(1)
                        {
                            char *preview_event = preview_event_type();
                            const char *event_selection_menu[] = 
                            {
                                "==================================================",
                                "           🛠️ EventEase Dashboard > Book Event     ",
                                "==================================================",
                                "📍 Home > Dashboard > Book Event > Select Event",
                                "",
                                "==================================================",
                                "        📝 Select the Kind of Event to Book       ",
                                "==================================================",
                                preview_event,
                                NULL
                            };

                            if(preview_event != NULL)
                            {
                                display_options(NULL, event_selection_menu, NULL, "Enter the Event ID of your choice");

                                if(scanf("%s", book_input[1]) != 1 || valid_typeevent_id(atoi(book_input[1])) != 1)
                                {
                                    printf("\nPlease enter a valid event id\n");
                                    Sleep(2000);
                                    clear_input_buffer();
                                    continue;
                                }
                                
                                clear_input_buffer();
                                book_event_filled++;
                                break;
                            } else 
                            {
                                printf("\n-- No Events are available for booking. --\n");
                                Sleep(2000);
                                break;
                            }  
                        }               
                        // review all choices

                        // read_eventtype_all will read all event_name. i.e Birthday\nWedding\n and so on.

                    } else if (book_event_choice == 3) // set package
                    {
                        while (1) 
                        {
                            if (book_input[1] == NULL || strlen(book_input[1]) == 0) {
                                printf("\nPlease select a type of event first (choose Option 2 from the menu).\n");
                                Sleep(1000);
                                break;
                            }

                            char *packages = preview_pkgs(atoi(book_input[1]));
                            if (packages == NULL) {
                                printf("\n-- No Available Packages for this Event. --\n");
                                Sleep(2000);
                                break;
                            }

                            char *package_blocks[100]; // Assuming a maximum of 100 packages
                            int total_packages = 0;

                            char *packages_copy = strdup(packages); // Create a modifiable copy of the input
                            if (!packages_copy) {
                                perror("Failed to allocate memory for packages copy.");
                                free(packages); // Free original memory
                                return;
                            }

                            char *block = strtok(packages_copy, "\n\n"); // Each package block is separated by double newlines
                            while (block) {
                                if (total_packages < 100) { // Prevent out-of-bounds write
                                    package_blocks[total_packages++] = block;
                                }
                                block = strtok(NULL, "\n\n");
                            }

                            int current_page = 0;
                            int total_pages = (total_packages + PAGE_SIZE - 1) / PAGE_SIZE;

                            while (1) 
                            {
                                system("cls");
                                printf("=========================================\n");
                                printf("           📦 Event Packages\n");
                                printf("=========================================\n");
                                printf("📍 Home > Dashboard > Book Event > Select Package\n\n");

                                // Calculate start and end indices for the current page
                                int start_index = current_page * PAGE_SIZE;
                                int end_index = start_index + PAGE_SIZE;
                                if (end_index > total_packages) {
                                    end_index = total_packages;
                                }

                                // Display packages for the current page
                                for (int i = start_index; i < end_index; i++) {
                                    printf("%s\n", package_blocks[i]);
                                }
                                printf("\n=========================================\n");
                                printf("Page %d of %d\n", current_page + 1, total_pages);
                                printf("=========================================\n");
                                printf("[N] Next Page | [P] Previous Page | [E] Exit\n");
                                printf("=========================================\n");
                                printf("Enter Package ID to Select: ");

                                char choice[50];
                                scanf(" %49s", choice);

                                if (strcasecmp(choice, "n") == 0) {
                                    if (current_page < total_pages - 1) {
                                        current_page++;
                                    } else {
                                        printf("\nYou are on the last page.\n");
                                        Sleep(2000);
                                    }
                                } else if (strcasecmp(choice, "p") == 0) {
                                    if (current_page > 0) {
                                        current_page--;
                                    } else {
                                        printf("\nYou are on the first page.\n");
                                        Sleep(2000);
                                    }
                                } else if (strcasecmp(choice, "e") == 0) {
                                    
                                    free(packages_copy); // Free packages_copy memory
                                    free(packages);   
                                    printf("\nExiting package selection...\n");
                                    Sleep(2000);
                                    memset(book_input[2], 0, sizeof(book_input[2]));
                                    break;
                                } else if (isdigit(choice[0])) {
                                    int pkg_id = atoi(choice);
                                    if (valid_pkg_id(atoi(book_input[1]), pkg_id) == 1) {
                                        snprintf(book_input[2], sizeof(book_input[2]), "%d", pkg_id);
                                        printf("\nPackage ID %d selected successfully!\n", pkg_id);
                                        Sleep(2000);
                                        book_event_filled++;
                                        break;
                                    } else {
                                        printf("\nInvalid Package ID. Please try again.\n");
                                        Sleep(2000);
                                        memset(book_input[2], 0, sizeof(book_input[2]));
                                    }
                                } else {
                                    printf("\nInvalid choice. Please try again.\n");
                                    Sleep(2000);
                                }
                            }
                               // Free original packages memory
                            break;               // Exit the outer loop
                        }

                        
                    } else if (book_event_choice == 4) // venue
                    {
                        while (1) 
                        {
                            if (strlen(book_input[1]) == 0) 
                            {
                                printf("\nPlease select a type of event first (choose Option 2 from the menu).\n");
                                Sleep(2000);
                                break;
                            }

                            char *venues = read_typevent(atoi(book_input[1]), "venues");
                            if (venues == NULL) 
                            {
                                printf("\n-- No venues available for this event type. --\n");
                                Sleep(2000);
                                continue;
                            }

                            char *temp_venues = strdup(venues);
                            if (temp_venues == NULL) 
                            {
                                perror("[ERROR] Memory allocation failed.");
                                free(venues);
                                continue;
                            }

                            int venue_count = 0;
                            char *temp_token = temp_venues;
                            while ((temp_token = strtok(temp_token, ",")) != NULL) 
                            {
                                venue_count++;
                                temp_token = NULL; // Continue parsing
                            }

                            char **venue_options = malloc((venue_count + 1) * sizeof(char *)); // +1 for NULL termination
                            if (venue_options == NULL) // hoping this would == NULL; because havent really experienced faulty mem allocation. :<
                            {
                                perror("[ERROR] Memory allocation failed for venue options.");
                                free(temp_venues);
                                free(venues);
                                continue;
                            }

                            strcpy(temp_venues, venues); 
                            int index = 0;
                            char *token = strtok(temp_venues, ",");
                            while (token) 
                            {
                                venue_options[index] = malloc(strlen(token) + 1);
                                if (venue_options[index] == NULL) 
                                {
                                    perror("[ERROR] Memory allocation failed.");
                                    for (int i = 0; i < index; i++) 
                                        free(venue_options[i]);
                                    free(venue_options);
                                    free(temp_venues);
                                    free(venues);
                                    return;
                                }
                                strcpy(venue_options[index], token);
                                index++;
                                token = strtok(NULL, ",");
                            }

                            // NULL terminate the array
                            venue_options[index] = NULL;

                            // Free temporary string copy
                            free(temp_venues);

                            const char *available_venues_menu[] = {
                                "==================================================",
                                "                 🌟 Available Venues 🌟            ",
                                "==================================================",
                                "",
                                "📍 Please select a venue for your event from the list below:",
                                NULL
                            };

                            display_options(NULL, available_venues_menu, venue_options, "👉 Enter your choice (-1 to cancel):");

                            int venue_choice;
                            printf("\nEnter your choice: ");
                            if (scanf("%d", &venue_choice) != 1 || (venue_choice < 1 || venue_choice > venue_count)) 
                            {   
                                printf("\nInvalid choice. Please try again.\n");
                                Sleep(2000);
                                for (int i = 0; i < venue_count; i++)
                                    free(venue_options[i]);
                                free(venue_options);
                                free(venues);
                                clear_input_buffer();
                                continue;
                            }
                            clear_input_buffer();

                            if (venue_choice == -1) 
                            {
                                printf("\nVenue selection canceled.\n");
                                for (int i = 0; i < venue_count; i++) 
                                    free(venue_options[i]);
                                free(venue_options);
                                free(venues);
                                break;
                            }

                            // Find and store the selected venue
                            token = strtok(venues, ",");
                            index = 1;
                            while (token) 
                            {
                                if (index == venue_choice) 
                                {
                                    strncpy(book_input[3], token, sizeof(book_input[3]) - 1);
                                    book_input[3][sizeof(book_input[3]) - 1] = '\0'; // Ensure null termination
                                    book_event_filled++;
                                    break;
                                }
                                index++;
                                token = strtok(NULL, ",");
                            }

                            printf("\nYou have selected the venue: %s\n", book_input[3]);
                            book_event_filled++;
                            Sleep(2000);

                            // dont forget to free the one you allocated.
                            for (int i = 0; i < venue_count; i++) 
                                free(venue_options[i]);
                            free(venue_options);
                            free(venues);

                            break; // rahhh
                        }

                        
                    } else if (book_event_choice == 5) // set-date
                    {
                        while (1) 
                        {
                            char date_str[50];
                            char formatted_date[20];
                            char formatted_time[10];
                            system("cls");
                            printf("\n==================================================\n");
                            printf("               \U0001F4C5 Set Event Date\n");
                            printf("==================================================\n");
                            printf("\n📝 Instructions:\n");
                            printf("- Please enter the date and time in the format 'DD Mon YYYY HH:MM:SS'.\n");
                            printf("- Example: 29 Dec 2024 21:42:36\n");
                            printf("- To cancel, type '-1'.\n");
                            printf("\n==================================================\n");
                            printf("\U0001F4C6 Enter the date and time: ");

                            fgets(date_str, sizeof(date_str), stdin);
                            date_str[strcspn(date_str, "\n")] = 0; // Remove trailing newline

                            if (strcmp(date_str, "-1") == 0) {
                                printf("\n\U0000274C Date entry canceled.\n");
                                break;
                            }

                            if(parse_date_manual(date_str, formatted_date, formatted_time) == 1)
                            {
                                strcpy(book_input[4], formatted_date);
                                strcpy(bk_event.start_time, formatted_time);
                                book_event_filled++;
                            } else 
                            {
                                Sleep(2000);
                                continue;
                            }
                            break;
                        }
                    } else if(book_event_choice == 6) // confirm
                    {
                        if(book_event_filled >= 5 && book_input[0] != NULL && book_input[1] != NULL && book_input[2] != NULL && book_input[3] != NULL && book_input[4] != NULL)
                        {
                            while(1)
                            {
                                strcpy(bk_event.name, book_input[0]);
                                bk_event.event_type_id = atoi(book_input[1]);
                                bk_event.package_id = atoi(book_input[2]);
                                strcpy(bk_event.venue, book_input[3]);
                                strcpy(bk_event.event_date, book_input[4]);

                                char event_type[50], venue_choice[50], date_time[50], selected_package[50], display_cost[50], display_percentage[50], amount_due[50];

                                sprintf(event_type, "📅 Event Type         : %s", read_typevent(bk_event.event_type_id, "event_name"));
                                sprintf(venue_choice, "📍 Venue of Choice    : %s", bk_event.venue);
                                sprintf(date_time,"🕒 Date & Time        : %s %s", bk_event.event_date, bk_event.start_time);
                                sprintf(selected_package, "🎁 Selected Package   : %s", read_pkg(bk_event.event_type_id, bk_event.package_id, "package_name"));

                                int total_cost = atoi(read_pkg(bk_event.event_type_id, bk_event.package_id, "price"));
                                int percentage = atoi(read_typevent(bk_event.event_type_id, "dp_percentage"));
                                sprintf(display_cost, "💰 Total Cost         : %s", format_number(total_cost));

                                float percentage_due = (float)percentage / 100.0;
                                int amount_due_now = (int)(total_cost * percentage_due);

                                sprintf(amount_due, "💳 Down Payment       : %d%% of Total Cost (%s)\n    ➡️  Amount Due Now : %s\n", 
                                    percentage, 
                                    format_number(total_cost), 
                                    format_number(amount_due_now));

                                const char *booking_confirmation_menu[] = {
                                    "==================================================",
                                    "         ✅ Booking Confirmation Submenu          ",
                                    "==================================================",
                                    "",
                                    "📋 Please review the details of your booking.",
                                    "",
                                    "==================================================",
                                    "               📄 Booking Details                 ",
                                    "==================================================",
                                    "",
                                    event_type,
                                    venue_choice,
                                    date_time,
                                    selected_package,
                                    "",
                                    display_cost,
                                    amount_due,
                                    NULL
                                };

                                display_options(NULL, booking_confirmation_menu, NULL, "For Confirmation Enter (Y) and (N) for cancellation:");
                                
                                char agree_balance[5];

                                if(scanf("%s", agree_balance) != 1 || strcasecmp(agree_balance, "y") == 0)
                                {
                                    bk_event.balance = total_cost - amount_due_now;

                                    char file_path[200];

                                    // payment deadline.
                                    time_t current_time = time(NULL);  // Get the current time
                                    if (current_time == -1) 
                                    {
                                        perror("Failed to get the current time");
                                        return 1;
                                    }

                                    // Add 3 days in seconds

                                    // deadlline.
                                    time_t future_time = current_time + (atoi(read_typevent(bk_event.event_type_id, "payment_deadline_days")) * 24 * 60 * 60);

                                    // Convert to a human-readable format
                                    struct tm *future_date = localtime(&future_time);

                                    char formatted_deadline[50];
                                    sprintf(formatted_deadline, "%02d %s %04d %02d:%02d:%02d",
                                            future_date->tm_mday,
                                            (char *[]){"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}[future_date->tm_mon],
                                            future_date->tm_year + 1900,
                                            future_date->tm_hour,
                                            future_date->tm_min,
                                            future_date->tm_sec);

                                    strcpy(bk_event.payment_deadline, future_date);

                                    //
                                    if (future_date == NULL) {
                                        perror("Failed to convert time please contact support.");
                                        break;
                                    }
                                    
                                    bk_event.id = generate_event_id();

                                    char reminder_message[50];
                                    sprintf(reminder_message, "Payment for Event ID: %d", bk_event.id);
                                    set_reminder(client_id, reminder_message, formatted_deadline);
                                    bk_event.status = 3; // Incomplete Payment.
                                    book_event(client_id, bk_event);

                                    system("cls");
                                    printf("\n==================================================\n");
                                    printf("      \U0001F4E6 Down Payment Submission Required\n");
                                    printf("==================================================\n");
                                    printf("📋 Please send proof of payment for the down payment to the organizer.\n");
                                    printf("🕒 Confirmation will be processed within 1-3 business days.\n");
                                    printf("✅ Once confirmed, you will receive a notification to proceed further.\n");
                                    printf("\nThank you for booking with us! We look forward to making your event special.\n");
                                    printf("\n==================================================\n");
                                    
                                    printf("\nPlease press [ANY KEY] to Return to Menu\n");
                                    if(getch() == '\r')
                                    {
                                        client_menu(client_id); // 
                                    }
                                    
                                    /* snprintf(file_path, sizeof(file_path), "%s%s%s.txt", path, read_user(client_id,"full_name"), bk_event.id);

                                    FILE *file_receipt = fopen(path, "w"); */


                                } else if(strcasecmp(agree_balance, "n") == 0)
                                {

                                    printf("\nCancelling Book Creation Session\n");
                                    Sleep(2000);
                                    break;
                                } else 
                                {
                                    printf("\nPlease pick a valid input.\n");
                                    Sleep(2000);
                                    continue;
                                }
                                
                            }

                            // confirmation
                            // then show agreement.
                            // inform that a cvs file is upload to this folder...
                            

                            // ask for the if they're willing to pay 50%
                            // 50% payment.
                            // then balance.
                        } else 
                        {
                            printf("\nPlease make sure you've filled-up the data fields.\n");
                            Sleep(2000);
                        }
                    } else if(book_event_choice == 7) // cancel
                    {
                        
                        printf("Cancelling Booking Session...");
                        Sleep(2000);
                    
                        memset(&bk_event, 0, sizeof(bk_event));
                        memset(&book_input, 0, sizeof(book_input));
                        book_event_filled = 0;
                        break;
                    }
                }
                break;
            case 3: // my events
                while(1)
                {
                    // TODO THIS ONE 2:48 A.M
                    int book_event_id;
                    int *book_id = NULL;
                    int event_count = 0;

                    char **events = preview_events(client_id, &event_count, &book_id);
                    if(events == NULL || event_count == 0)
                    {
                        system("cls");
                        printf("==================================================\n");
                        printf("                  📅 Booked Events               \n");
                        printf("==================================================\n\n");
                        printf("It looks like you haven't booked any events yet! 🎉\n");
                        printf("Why not plan something special today?\n");
                        printf("✨ Browse our event packages and create unforgettable memories.\n");
                        printf("Start by selecting 'Book Event' from the menu! 🛠️\n");
                        printf("We can't wait to make your event extraordinary! 🌟\n");
                        printf("\n==================================================\n\n");
                        printf("Press [ANY KEY] to return to Dashboard");

                        if(getch() == '\r')
                        {
                            client_menu(client_id);
                        }
                    }

                    char *booked_events_menu[] = {
                        "==================================================",
                        "                  📅 Booked Events",
                        "==================================================",
                        "",
                        "📍 Home > Dashboard > My Events > View Booked Events",
                        "",
                        "🔧 Select an event to manage its details:",
                        "",
                        "==================================================",
                        NULL // NULL-terminated to mark the end of the array
                    };
                    
                    display_options(NULL, booked_events_menu, events, "Enter event id to manage (-1 to cancel):");
                    scanf("%d", &book_event_id);

                    if(book_event_id == -1)
                    {
                        printf("Exiting Managing Booked Events...");
                        Sleep(2000);
                        break;
                    }
                    if(valid_event_id(client_id, book_event_id) != 1)
                    {
                        printf("\nPlease enter a valid choice/id\n");
                        Sleep(2000);
                        clear_input_buffer();
                        continue;
                    }

                    clear_input_buffer();

                    while(1)
                    {
                        int submenu_choice;

                        char *event_name = read_event(client_id, book_event_id, "name");



                        char manage_event[50], crumbs_event[50];
                        sprintf(manage_event, "     📝 Manage Event: %s", event_name);
                        sprintf(crumbs_event, "📍 Home > Dashboard > My Events > Manage Event > %s ", event_name);

                        char *event_submenu[] = {
                            "==================================================",
                            manage_event,
                            "==================================================",
                            crumbs_event,
                            "",
                            "==================================================",
                            NULL
                        };

                        char *options_submenu[] = { "View Full Details         - 🧐 Display the event’s complete information.",
                            "Generate Receipt             - 🧾 Download a printable receipt for this event.",
                            "Generate Itinerary           - 📋 Download a printable a detailed event itinerary.",
                            "Update Details            - ✏️ Modify specific details of the event.",
                            "Cancel Booking            - ❌ Request to cancel this booking.",
                            "Back to Booked Events      - 🔙 Return to the list of booked events.",
                            NULL
                        };

                        display_options(NULL, event_submenu, options_submenu, "Please enter a valid option:");

                        if(scanf("%d", &submenu_choice) != 1 || submenu_choice < 1 || submenu_choice > 6)
                        {
                            printf("\nPlease enter a valid option.\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        char *venue = read_event(client_id, book_event_id, "venue");
                        char *event_date = read_event(client_id, book_event_id, "event_date");
                        char *start_time = read_event(client_id, book_event_id, "start_time");
                        char *payment_deadline = read_event(client_id, book_event_id, "payment_deadline");
                        char *balance = read_event(client_id, book_event_id, "balance");
                        
                        if(submenu_choice == 1) // view full details
                        {
                            while(1)
                            {
                                

                                system("cls");

                                printf("==================================================\n");
                                printf("               📄 Event Details\n");
                                printf("==================================================\n\n");

                                if (event_name) {
                                    printf("📍 Event: %s\n", event_name);
                                } else {
                                    printf("📍 Event: [Unable to fetch event name]\n");
                                }

                                if (event_date) {
                                    printf("📅 Date: %s\n", event_date);
                                } else {
                                    printf("📅 Date: [Unable to fetch event date]\n");
                                }

                                if (start_time) {
                                    printf("⏰ Time: %s\n", start_time);
                                } else {
                                    printf("⏰ Time: [Unable to fetch start time]\n");
                                }

                                if (venue) {
                                    printf("🏨 Venue: %s\n", venue);
                                } else {
                                    printf("🏨 Venue: [Unable to fetch venue]\n");
                                }

                                char *event_type_str = read_event(client_id, book_event_id, "event_type_id");
                                int type_event_id = event_type_str ? atoi(event_type_str) : 0;
                                if (event_type_str) free(event_type_str);

                                char *pkg_id_str = read_event(client_id, book_event_id, "package_id");
                                int pkg_event_id = pkg_id_str ? atoi(pkg_id_str) : 0;
                                if (pkg_id_str) free(pkg_id_str);

                                char *package_name = read_pkg(type_event_id, pkg_event_id, "package_name");
                                if (package_name) 
                                {
                                    printf("🎁 Package: %s\n", package_name);
                                    free(package_name);
                                } else 
                                {
                                    printf("🎁 Package: [Unable to fetch package name]\n");
                                }

                                char *price_str = read_pkg(type_event_id, pkg_event_id, "price");
                                if (price_str) 
                                {
                                    printf("💰 Total Cost: PHP %s\n", format_number(atoi(price_str)));
                                    free(price_str);
                                } else
                                {
                                    printf("💰 Total Cost: [Unable to fetch price]\n");
                                }

                                if (balance) 
                                {
                                    printf("⚠️ Balance Due: PHP %s\n", balance);
                                } else {
                                    printf("⚠️ Balance Due: [Unable to fetch balance]\n");
                                }

                                if (payment_deadline) 
                                {
                                    printf("🔔 Payment Deadline: %s\n\n", payment_deadline);
                                } else 
                                {
                                    printf("🔔 Payment Deadline: [Unable to fetch payment deadline]\n\n");
                                }

                                printf("==================================================\n");
                                printf("Press Enter to return to the submenu.\n");

                                if(getch() == '\r')
                                {
                                    break;
                                }

                            }
                        } else if(submenu_choice == 2) // print receipt
                        {
                            // generate_receipe
                            int event_type_id = atoi(read_event(client_id, book_event_id, "event_type_id"));
                            generate_receipt(client_id, event_type_id, atoi(read_event(client_id, book_event_id, "package_id")), book_event_id);


                            while(1)
                            {
                                system("cls");
                                print_receipt(client_id, event_type_id, atoi(read_event(client_id, book_event_id, "package_id")), book_event_id);
                                if(getchar() != '\r')
                                {
                                    break;
                                }
                            }

                        } else if(submenu_choice == 3) // print itinerary
                        {

                        }
                        else if (submenu_choice == 4) // update details
                        {
                            while(1)
                            {
                                log_event(client_id, 0, "User selected to update an event.");
                                const char *updatable_keys[] = {"name", "event_date", "venue"};
                                const char *updatable_labels[] = {"Event Name", "Event Date", "Venue"};
                                size_t key_count = sizeof(updatable_keys) / sizeof(updatable_keys[0]);

                                char *update_event_menu[] = {
                                    "==================================================",
                                    "               📝 Update Booked Event             ",
                                    "==================================================",
                                    "📍 Home > Dashboard > My Events > Update Event",
                                    "",
                                    "==================================================",
                                    NULL
                                };

                                char *update_field[] = {
                                    "Event Name       \U0001F4C3 - Update the name of your event.",
                                    "Event Date       \U0001F4C5 - Change the date and time of your event.",
                                    "Venue            \U0001F3E2 - Update the venue of the event.",
                                    "Cancel           \U0001F519 - Return to My Events Menu.", NULL
                                };

                                display_options(NULL, update_event_menu, update_field, "Enter the number of the field you'd like to update:");

                                int field_choice;
                                if (scanf("%d", &field_choice) != 1 || field_choice < 1 || field_choice > 4) {
                                    printf("\nInvalid choice. Please enter a number between 1 to 5.\n");
                                    Sleep(2000);
                                    clear_input_buffer();
                                    continue;
                                }
                                clear_input_buffer();

                                if (field_choice < 1 || field_choice > (int)key_count + 1) {
                                    log_event(client_id, 1, "Invalid event update choice. Returning to events menu.");
                                    printf("Invalid choice. Returning to events menu.\n");
                                    Sleep(500);
                                    continue;
                                }

                                if (field_choice == (int)key_count + 1) {
                                    log_event(client_id, 1, "Event update canceled. Returning to events menu.");
                                    printf("Update canceled. Returning to events menu.\n");
                                    Sleep(500);
                                    break;
                                }

                                const char *selected_key = updatable_keys[field_choice - 1];
                                const char *selected_label = updatable_labels[field_choice - 1];
                                char new_value[100];

                                if(field_choice == 2)
                                {
                                    printf("\n==================================================\n");
                                    printf("               \U0001F4C5 Set Event Date\n");
                                    printf("==================================================\n");
                                    printf("\n📝 Instructions:\n");
                                    printf("- Please enter the date and time in the format 'DD Mon YYYY HH:MM:SS'.\n");
                                    printf("- Example: 29 Dec 2024 21:42:36\n");
                                    printf("- To cancel, type '-1'.\n");
                                    printf("\n==================================================\n");
                                }

                                printf("\nEnter new %s (type 'cancel' to cancel): ", selected_label);
                                fgets(new_value, sizeof(new_value), stdin);
                                new_value[strcspn(new_value, "\n")] = 0;

                                if (strcmp(new_value, "cancel") == 0) {
                                    log_event(client_id, 1, "Event update canceled by the user.");
                                    printf("Update canceled. Returning to events menu.\n");
                                    memset(new_value, 0, sizeof(new_value));
                                    continue;
                                }

                                // Validate input for specific fields
                                if (strcmp(selected_key, "event_date") == 0) 
                                {

                                    char formatted_date[20];
                                    char formatted_time[10];

                                    if(parse_date_manual(new_value, formatted_date, formatted_time) == 1)
                                    {
                                        if(update_event(client_id, book_event_id, selected_key, new_value) == 1)
                                        {
                                            printf("\n %s updated successfully! \n", selected_label);
                                            Sleep(3000);
                                            break;
                                        }
                                    } else 
                                    {
                                        printf("\n❌ Invalid date format. Please try again.\n");
                                        Sleep(2000);
                                        continue;
                                    }
                                }

                                if (update_event(client_id, book_event_id, selected_key, new_value) == 1) 
                                {
                                    log_event(client_id, 0, "Event updated successfully.");
                                    printf("%s updated successfully!\n", selected_label);
                                } else {
                                    log_event(client_id, 2, "Failed to update event.");
                                    printf("Failed to update %s. Please try again.\n", selected_label);
                                }

                            }                        
                            
                        } else if(submenu_choice == 5) // cancel booking
                        {
                            while(1)
                            {
                                system("cls");
                                printf("==================================================\n");
                                printf("             ❌ Cancel Booking                   \n");
                                printf("==================================================\n\n");
                                printf("⚠️ Are you sure you want to cancel \"%s\" (%s)?\n", event_name, event_date);
                                printf("This action may incur cancellation charges.\n\n");
                                printf("==================================================\n\n");
                                printf("Type 'Y' to confirm or 'N' to cancel: ");

                                char confirmation[4];

                                if(scanf("%s", confirmation) != 1 || (strcasecmp(confirmation, "y") != 0 && strcasecmp(confirmation, "n") != 0))
                                {
                                    printf("\n Please select a valid Input.\n");
                                    Sleep(2000);
                                    clear_input_buffer();
                                    continue;
                                }
                                
                                if(strcasecmp(confirmation, "y") == 0)
                                {
                                    system("cls");
                                    printf("\n==================================================\n");
                                    printf("             ✅ Cancellation Requested           \n");
                                    printf("==================================================\n\n");
                                    printf("Your cancellation request for \"%s\" has been submitted.\n", event_name);
                                    printf("The organizer will process this within 1-3 business days.\n\n");
                                    printf("==================================================\n");
                                    printf("Press Enter to return to the submenu.\n");

                                    cancel_event(client_id, book_event_id);
                                    if(getch() == '\r')
                                    {
                                        client_menu(client_id);
                                    }

                                } else if(strcasecmp(confirmation, "n") == 0)
                                {
                                    break;
                                }

                            }
                            // ask for confirmation
                            
                        } else if(submenu_choice == 6) // back.
                        {
                            printf("Going back now...");
                            Sleep(2000);
                            break;
                        } else 
                        {
                            printf("\nPlease enter a valid choice.\n");
                            Sleep(2000);
                            continue;
                        }
                    }
                }
                // view booked events
                // update event details
                // delete event
                // 
                break;
            case 4: // view packages
                while (1) 
                {
                    char preview[20];
                    int choice_preview;
                    char *preview_event = preview_event_type();

                    if (preview_event != NULL) {
                        const char *paragraph[] = {
                            "==================================================",
                            "           🎯 Event Type Selection Menu           ",
                            "==================================================",
                            "",
                            "📍 Navigation: Home > Dashboard > Preview Packages",
                            "",
                            "🔍 Please select the event type to view its available packages.",
                            "   Use the Event ID to make your selection.",
                            "",
                            "==================================================",
                            "🌟 Available Event Types:",
                            preview_event,
                            "==================================================",
                            "",
                            "📌 Instructions:",
                            "- Enter the Event ID to proceed.",
                            "- Type '-1' to go back or cancel.",
                            NULL
                        };

                        display_options(NULL, paragraph, NULL, NULL);
                        scanf("%s", preview);

                        if(strcmp(preview, "-1") == 0)
                        {
                            client_menu(client_id);
                        }

                        if (valid_typeevent_id(atoi(preview)) != 1 ) 
                        {
                            printf("\n❌ Please enter a valid Event ID.\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        } 
                        clear_input_buffer();
                        choice_preview = atoi(preview);

                        char *preview_pkg = preview_pkgs(choice_preview);

                        if (preview_pkg == NULL) 
                        {
                            printf("==================================================\n");
                            printf("               ⚠️ No Packages Available\n");
                            printf("==================================================\n");
                            printf("\n   Sorry, there are currently no packages available\n");
                            printf("         for this event. Please check back later.\n\n");
                            printf("==================================================\n");
                            Sleep(3000);
                            break;
                        } else {
                            while (1) 
                            {
                                char *package_blocks[100];
                                int total_packages = 0;

                                char *packages_copy = strdup(preview_pkg);
                                if (packages_copy == NULL) {
                                    perror("[ERROR] Memory allocation failed");
                                    free(preview_pkg);
                                    free(preview_event);
                                    return;
                                }

                                char *block = strtok(packages_copy, "\n\n");
                                while (block) {
                                    package_blocks[total_packages++] = block;
                                    block = strtok(NULL, "\n\n");
                                }

                                int current_page = 0;
                                int total_pages = (total_packages + PAGE_SIZE - 1) / PAGE_SIZE;

                                while (1) {
                                    system("cls");
                                    printf("=========================================\n");
                                    printf("           📦 Event Packages\n");
                                    printf("=========================================\n");
                                    printf("📍 Home > Dashboard > Book Event > Select Package\n\n");

                                    int start_index = current_page * PAGE_SIZE;
                                    int end_index = start_index + PAGE_SIZE;
                                    if (end_index > total_packages) {
                                        end_index = total_packages;
                                    }

                                    for (int i = start_index; i < end_index; i++) {
                                        printf("%s\n", package_blocks[i]);
                                    }

                                    printf("\n=========================================\n");
                                    printf("Page %d of %d\n", current_page + 1, total_pages);
                                    printf("=========================================\n");
                                    printf("[N] Next Page | [P] Previous Page | [E] Exit\n");
                                    printf("=========================================\n");
                                    printf("Enter Package ID to Select: ");

                                    char choice[50];
                                    scanf(" %49s", choice);

                                    if (strcasecmp(choice, "n") == 0) {
                                        if (current_page < total_pages - 1) {
                                            current_page++;
                                        } else {
                                            printf("\n❗ You are on the last page.\n");
                                            Sleep(2000);
                                        }
                                    } else if (strcasecmp(choice, "p") == 0) {
                                        if (current_page > 0) {
                                            current_page--;
                                        } else {
                                            printf("\n❗ You are on the first page.\n");
                                            Sleep(2000);
                                        }
                                    } else if (strcasecmp(choice, "e") == 0) {
                                        printf("\nExiting package selection...\n");
                                        Sleep(2000);
                                        break;
                                    } else if (valid_pkg_id(choice_preview, atoi(choice)) == 1) {
                                        printf("\n✅ Package ID %s selected successfully!\n", choice);
                                        Sleep(2000);
                                        break;
                                    } else {
                                        printf("\n❌ Invalid choice. Please try again.\n");
                                        Sleep(2000);
                                    }
                                }

                                free(packages_copy);
                                break;
                            }
                            free(preview_pkg);
                        }
                    } else {
                        printf("==================================================\n");
                        printf("               ⚠️ No Events Available\n");
                        printf("==================================================\n");
                        printf("\n   Sorry, there are currently no events available\n");
                        printf("         for booking. Please check back later.\n\n");
                        printf("==================================================\n");
                        Sleep(2000);
                        break;
                    }
                }
                break;
            case 5: // feedback & support
                // Feedback & Support
                break;
            case 6: // notifcations
                // Notifications
                break;
            case 7: // payment history
                // Logout
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            case 11:
                break;
            case 12:
                break;
        }
    }
}

int organizer_menu(int organizer_id)
{
    int organizer_choice;

    while(1)
    {
        system("cls");
        int event_choice;
        printf("Enter your choice: ");
        scanf("%d", &organizer_choice);

        TypeEvent type_event =
        {
            .name = "",
            .description = "", // \0
            .venues = "" // delimiter kay 'comma'
        };

        int valid_event_name;

        int data_filled;

        switch(organizer_choice)
        {
             case 1:
                // Event Management
                // prompt user for event_choice
                // create new eventtype

                while(1)
                {
                    system("cls");
                    printf("-- Create New Event Type Menu --\n");
                    printf("Please enter the remaining information\n\n\n");

                    char type_member[4][100] = {"Name", "Description", "Venues", "Type of Event"};

                    for (int i = 0; i < 4; i++)  
                    {
                        char *field = NULL;
                        switch (i) 
                        {
                            case 0: field = type_event.name; break;
                            case 1: field = type_event.description; break;
                            case 2: field = type_event.venues; break;
                            case 3: field = type_event.type; break;
                        }

                        
                        if (field && strlen(field) > 0) 
                        {
                            printf("%d. %s %s(/)%s\n", i + 1, type_member[i], GREEN, RESET);
                        } else 
                        {
                            printf("%d. %s %s(x)%s\n", i + 1, type_member[i], RED, RESET);
                        }
                    }
                    printf("5. Confirm Creation\n");
                    printf("6. Cancel Creation.\n");
                    printf("\nEnter Choice: ");
                    scanf("%d", &event_choice);
                    getchar();

                    if(event_choice == 1)
                    {
                        
                            // event name
                        if(event_choice == 1)
                        {
                            // validate kung unsay kalyangan
                            // i.e; dapat ang gi input kay 4-12 chars.
                            while(!valid_event_name)
                            {
                                printf("\nEnter Event Type Name: ");
                                fgets(type_event.name, sizeof(type_event.name), stdin);
                                type_event.name[strcspn(type_event.name, "\n")] = 0; // para mawala ang nextline


                                // check check kung si name blahb lah blah
                                size_t name_size = strlen(type_event.name);

                                if(name_size < 4 || name_size > 12)
                                {
                                    printf("Invalid length! Event Name Must be 4-12 Characters!\n");
                                } else
                                {
                                    data_filled++; // + 1
                                    valid_event_name = 1;
                                }
                            }
                        }
                        
                    }  else if(event_choice == 5) // confirm.
                    {
                        if(data_filled == 4)
                        {

                        } else
                        {
                            printf("Please fill in the fields first.");
                            Sleep(1000);
                            continue;
                        }
                    } else if(event_choice == 6) // cancel nga part.
                    {
                        memset(&type_event, 0, sizeof(type_event)); 
                    }
                }
                break;
            case 2:
                
                break;
            case 3:
                
                break;
            case 4:
                
                break;
            case 5:
                
                break;
            case 6:

                break;
            case 7:
            
                break;
            case 8:

                break;
            
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 8.\n");
                Sleep(1000);
                break;
        }
    }
} 