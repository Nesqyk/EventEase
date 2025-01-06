
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
        system("cls");
        printf("==================================================\n");
        printf("               🌟 Welcome Back! 🌟               \n");
        printf("==================================================\n");
        printf("   We're getting everything ready for you.       \n");
        printf("         Redirecting to your dashboard...        \n");
        printf("==================================================\n");

        // Sleep(4000);

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
                    Sleep(2000);

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
                    while (1)
                    {
                        printf("\n==================================================\n");
                        printf("                  🔒 Set Password                 \n");
                        printf("==================================================\n");

                        char password[50] = {0};  // Updated variable name
                        printf("👉 Enter your new password: ");
                        get_password(password, sizeof(password));

                        // Password strength checks
                        int has_upper = 0, has_digit = 0, has_special = 0;
                        for (size_t i = 0; i < strlen(password); i++) 
                        {
                            if (isupper(password[i]))
                                has_upper = 1;
                            else if (isdigit(password[i]))
                                has_digit = 1;
                            else if (ispunct(password[i]))
                                has_special = 1;
                        }

                        if (strlen(password) < 6 || !has_upper || !has_digit || !has_special) 
                        {
                            printf("\n==================================================\n");
                            printf("            🚫 Password Strength Error            \n");
                            printf("==================================================\n");
                            printf("Password must:\n");
                            printf("- Be at least 6 characters long\n");
                            printf("- Contain at least one uppercase letter\n");
                            printf("- Contain at least one digit\n");
                            printf("- Contain at least one special character\n");
                            printf("==================================================\n");
                            Sleep(3000);
                            memset(password, 0, sizeof(password));  // Clear the password variable
                            continue;
                        }

                        // Confirm password
                        char confirm_password[50] = {0};
                        printf("👉 Confirm your new password: ");
                        get_password(confirm_password, sizeof(confirm_password));

                        if (strcmp(password, confirm_password) != 0) 
                        {
                            printf("\n==================================================\n");
                            printf("            🚫 Passwords Do Not Match             \n");
                            printf("==================================================\n");
                            printf("Please ensure both passwords are the same.\n");
                            printf("==================================================\n");
                            Sleep(3000);
                            memset(password, 0, sizeof(password));
                        } 
                        else 
                        {
                            printf("\n==================================================\n");
                            printf("             ✅ Password Set Successfully         \n");
                            printf("==================================================\n");

                            int user_id = is_numeric(username) ? atoi(username) : get_id_username(username);

                            update_user(user_id, "password", password);
                            Sleep(2000);
                            break;
                        }
                    }
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
                register_user(new_user);
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

// highlight personalized dashboard
// ticketing system
// efficient booking system
// account management
// history for payments
// organized schedule for booked events.
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


    while(1)
    {
        char book_input[5][MAX_INPUT_SIZE] = {""}; // Array to store user inputs

        int book_event_filled = 0;
        char *calendar = display_current_month_with_highlight();
        char *prev_reminders = view_reminders(client_id);

        int *book_id = NULL;
        int event_count = 0;

        char *preview_event = prev_events(client_id);

        char *name = read_user(client_id, "full_name");
        char p_name[30];

        sprintf(p_name, "           👋 Welcome, %s", name);

        const char *dashboard_paragraph[] = {
            "==================================================",
            "              🌟 EventEase Dashboard 🌟",
            "==================================================",
            "",
            p_name, // Welcome only after register.
            "Here's your personalized dashboard to manage events \n                 effortlessly.",
            "",
            get_current_date_time(),
            "",
            preview_event,
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
                        "",
                        "📍 Home > Dashboard > Profile",
                        "",
                        "==================================================",
                        NULL // Null terminator for the array
                    };

                    char *profile_choices[] = {
                        "View Profile         \U0001F464 - See your personal details.",
                        "Update Profile       \U0001F4DD - Modify your account information.",
                        "Payment History      \U0001F4B8 - Review your past and pending payments.",
                        "Delete Account       \U0001F5D1 - Permanently remove your account and data.",
                        "Back to Main Menu    \U0001F519 - Return to the dashboard.",
                        NULL
                    };

                    int choice_profile;
                    display_options(NULL,profile_menu, profile_choices, NULL);

                    if(scanf("%d", &choice_profile) != 1 || choice_profile < 1 || choice_profile > 5)
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
                    else if(choice_profile == 3) // payment history
                    {
                        char *payment_history = preview_payment_history(client_id);

                        if(payment_history == NULL)
                        {
                            printf("%s\n", payment_history);
                            Sleep(2000);
                        }

                        while(1)
                        {
                           printf("%s\n", payment_history);
                           printf("\nPlease press [ENTER] to exit");
                           if(getch() == '\r')
                           {
                            break;
                           }
                        }
                    }
                    else if(choice_profile == 5)
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
                                break;
                            } else 
                            {
                                printf("\nPlease enter a valid option\n");
                                Sleep(3000);
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


                                    //
                                    if (future_date == NULL) {
                                        perror("Failed to convert time please contact support.");
                                        break;
                                    }
                                    
                                    bk_event.id = generate_event_id();

                                    char reminder_message[50];
                                    sprintf(reminder_message, "Payment for Event ID: %d", bk_event.id);
                                    set_reminder(client_id, reminder_message, formatted_deadline);
                                    strcpy(bk_event.payment_deadline, formatted_deadline);
                                    add_payment_history(client_id, bk_event.name, (double) bk_event.balance, "Down Payment");
                                    bk_event.status = 3; // Incomplete Payment.
                                    book_event(client_id, bk_event);

                                    system("cls");
                                    printf("\n==================================================\n");
                                    printf("      \U0001F4E6 Down Payment Submission Required\n\n");
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
                        
                        printf("\nCancelling Booking Session...\n");
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


                    char *events = prev_events(client_id);
                    if(events == NULL)
                    {
                        while(1)
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
                        events,
                        NULL // NULL-terminated to mark the end of the array
                    };
                    
                    display_options(NULL, booked_events_menu, NULL, "Enter event id to manage (-1 to cancel):");
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

                        char manage_event[100], crumbs_event[100];
                        sprintf(manage_event, "         📝 Manage Event: %s", event_name);
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
                            // "Generate Receipt          - 🧾 Download a printable receipt for this event.",
                            "Pay Balance               - 💳 Pay remaining balance",
                            "Update Details            - ✏️ Modify specific details of the event.",
                            "Cancel Booking            - ❌ Request to cancel this booking.",
                            "Back to Booked Events     - 🔙 Return to the list of booked events.",
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
                        // } else if(submenu_choice == 2) // print receipt // should probably remove this.
                        // {
                        //     // generate_receipe
                        //     int event_type_id = atoi(read_event(client_id, book_event_id, "event_type_id"));
                        //     generate_receipt(client_id, event_type_id, atoi(read_event(client_id, book_event_id, "package_id")), book_event_id);


                        //     while(1)
                        //     {
                        //         system("cls");
                        //         print_receipt(client_id, event_type_id, atoi(read_event(client_id, book_event_id, "package_id")), book_event_id);
                        //         if(getchar() != '\r')
                        //         {
                        //             break;
                        //         }
                        //     }

                        } else if(submenu_choice == 2) // pay balanace.
                        {
                            // check if balance is 0;

                            char *balance = read_event(client_id,book_event_id, "balance");

                            if(atoi(balance) == 0)
                            {
                                system("cls");
                                printf("==================================================\n");
                                printf("             ✅ No Outstanding Balance          \n");
                                printf("==================================================\n");
                                printf("   Your balance has been fully paid.             \n");
                                printf("   Everything is set—we’ll see you at your       \n");
                                printf("                amazing event!                   \n");
                                printf("==================================================\n");
                                printf("\nPlease press [ANY KEY] to go back.\n");
                                if(getch() == '\r' )
                                {
                                    break;
                                }
                            }

                            while(1)
                            {
                                const char *pay_balance_header[] = {
                                    "==================================================",
                                    "                 💳 Pay Balance                   ",
                                    "==================================================",
                                    "",
                                    "Manage your outstanding balances effortlessly.",
                                    "",
                                    "==================================================",
                                    "               Select Payment Method              ",
                                    "==================================================",
                                    NULL
                                };

                                const char *payment_methods[] = {
                                    "💳 Credit/Debit Card",
                                    "🏦 Bank Transfer",
                                    "🖥️ E-Wallet (e.g., PayPal, GCash)",
                                    "🔙 Back to My Events",
                                    NULL
                                };
                                
                                int choice;
                                display_options(NULL, pay_balance_header, payment_methods, NULL);
                                if(scanf("%d", &choice) != 1 || choice < 1 || choice > 4 )
                                {
                                    printf("\nPlease enter a valid payment choice.\n");
                                    Sleep(3000);
                                    clear_input_buffer();
                                    continue;
                                }
                                clear_input_buffer();

                                if(choice == 4)
                                {
                                    break;
                                }

                                while(1)
                                {
                                    char confirmation[4];
                                    int amount;

                                    system("cls");
                                    printf("==================================================\n");
                                    printf("                     Payment Summary              \n");
                                    printf("==================================================\n\n");
                                    printf("Event Name        : %s\n", event_name);
                                    printf("Outstanding Balance: %s\n", format_number(atoi(balance)));
                                    printf("Payment Method     : %s\n\n", payment_methods[choice - 1]);
                                    printf("--------------------------------------------------\n");

                                    printf("\nPlease enter the amount you want to pay (-1 to cancel): ");

                                    if(scanf("%d", &amount) != 1 || amount > atoi(balance) || amount < 1 && amount != -1)
                                    {
                                        printf("\nPlease enter a valid amount\n");
                                        clear_input_buffer();
                                        Sleep(3000);
                                        continue;
                                    }
                                    clear_input_buffer();
                                    if(amount == -1)
                                    {
                                        break;
                                    }

                                    printf("\nPlease confirm your payment (Y / N): ");
                                    scanf("%s", &confirmation);
                                    clear_input_buffer();

                                    if((strcasecmp(confirmation, "y") != 0 && strcasecmp(confirmation, "n") != 0))
                                    {
                                        printf("\n Please select a valid Input.\n");
                                        Sleep(2000);
                                        clear_input_buffer();
                                        continue;
                                    }
                                    int new_balance = atoi(balance) - amount;
                                    char b_char[30];
                                    sprintf(b_char, "%d", new_balance);

                                    if (strcasecmp(confirmation, "y") == 0) 
                                    {
                                        add_payment_history(client_id, bk_event.name, (double) bk_event.balance, payment_methods[choice - 1]);

                                        if (update_event(client_id, book_event_id, "balance", b_char) != 1) 
                                        {
                                            printf("[ERROR] Failed to update event balance.\n");
                                            break;
                                        }

                                        while (1) 
                                        {
                                            char *event_type_str = read_event(client_id, book_event_id, "event_type_id");
                                            if (event_type_str == NULL) 
                                            {
                                                printf("[ERROR] Failed to fetch event_type_id.\n");
                                                return;
                                            }

                                            int event_type_id = atoi(event_type_str);

                                            char *package_id_str = read_event(client_id, book_event_id, "package_id");
                                            if (package_id_str == NULL) {
                                                printf("[ERROR] Failed to fetch package_id.\n");
                                                return;
                                            }

                                            int package_id = atoi(package_id_str);

                                            system("cls");
                                            generate_receipt_singular(client_id, event_type_id, package_id, book_event_id, (double) amount, payment_methods[choice - 1]);
                                            print_receipt_singular(client_id, event_type_id, package_id, book_event_id, (double) amount, payment_methods[choice - 1]);

                                            if (getch() == '\r') {
                                                client_menu(client_id);
                                                break;
                                            }
                                        }
                                    } else if (strcasecmp(confirmation, "n") == 0) {
                                        continue;
                                    }

                                }
                            }
                    }
                    else if (submenu_choice == 3) // update details
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
                        
                    } else if(submenu_choice == 4) // cancel booking
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
                        
                    } else if(submenu_choice == 5) // back.
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
                            "",
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
                while(1)
                {
                    int feedback_choice;
                    const char *feedback_support_paragraph[] = {
                        "==================================================",
                        "              💬 Feedback & Support Menu          ",
                        "==================================================",
                        "",
                        "📍 Navigation: Home > Dashboard > Feedback & Support",
                        "",
                        "🔍 Select an option to proceed:",
                        "",
                        NULL
                    };

                    const char *feedback_support_options[] = {
                        "🎟️ Preview Tickets    - View all submitted support tickets and their statuses.",
                        "📨 Submit Feedback    - Share your thoughts or suggestions.",
                        "🆘 Request Support    - Get assistance with any issues or queries.",
                        "🔙 Back to Dashboard  - Return to the main dashboard.",
                        NULL
                    };

                    display_options(NULL, feedback_support_paragraph, feedback_support_options, NULL);

                    if(scanf("%d", &feedback_choice) != 1 || feedback_choice < 1 || feedback_choice > 4)
                    {
                        printf("\nPlease enter a valid choice\n");
                        clear_input_buffer();
                        Sleep(3000);
                        continue;
                    }
                    clear_input_buffer();

                    if(feedback_choice == 1)
                    {
                        while(1)
                        {
                            system("cls");
                            printf("%s\n",view_tickets(client_id));
                            printf("\nPlease enter [ANY KEY] to stop viewing.\n");

                            if(getch() == '\r')
                            {
                                break;
                            }
                        }
                    }
                    else if(feedback_choice == 2)
                    {
                        while (1)
                            {
                                int star;
                                printf("\nHow would you rate your experience? (1-5 ⭐): ");
                                if (scanf("%d", &star) != 1 || star < 1 || star > 5)
                                {
                                    printf("\n❌ Invalid star rating! Please provide a rating between 1 and 5.\n");
                                    clear_input_buffer();
                                    Sleep(2000);
                                    continue;
                                }
                                clear_input_buffer();

                                char feedback[500];
                                printf("\nPlease share any additional comments or suggestions: ");
                                fgets(feedback, sizeof(feedback), stdin);
                                feedback[strcspn(feedback, "\n")] = 0; // Remove trailing newline character

                                if (strlen(feedback) == 0)
                                {
                                    printf("\n⚠️ Comments cannot be empty. Please provide your feedback.\n");
                                    Sleep(2000);
                                    continue;
                                }

                                // Confirm feedback submission
                                printf("\n==================================================\n");
                                printf("          📋 Review Your Feedback Submission      \n");
                                printf("==================================================\n");
                                printf("⭐ Rating       : %d\n", star);
                                printf("💬 Comments    : %s\n", feedback);
                                printf("==================================================\n");
                                printf("\n❓ Confirm submission? (Y/N): ");

                                char confirmation;
                                scanf(" %c", &confirmation);
                                clear_input_buffer();

                                if (confirmation == 'Y' || confirmation == 'y')
                                {
                                    if (create_feedback(client_id, star, feedback) == 1)
                                    {
                                        printf("\n✅ Thank you for your feedback! It has been submitted successfully.\n");
                                        Sleep(2000);
                                        break;
                                    }
                                    else
                                    {
                                        printf("\n❌ An error occurred while submitting your feedback. Please try again.\n");
                                        Sleep(2000);
                                    }
                                }
                                else if (confirmation == 'N' || confirmation == 'n')
                                {
                                    printf("\n🚫 Feedback submission canceled.\n");
                                    Sleep(2000);
                                    break;
                                }
                                else
                                {
                                    printf("\n❌ Invalid option. Please choose 'Y' or 'N'.\n");
                                    Sleep(2000);
                                }
                            }
                    } else if(feedback_choice == 3)
                    {
                        while (1)
                        {
                            const char *feedback_support_menu[] = {
                                "==================================================",
                                "              💬 Request & Support               ",
                                "==================================================",
                                "📍 Home > Dashboard > Feedback & Support",
                                "",
                                "🔧 Need assistance or have feedback to share?",
                                "   Select an option below to proceed:",
                                "",
                                "==================================================",
                                NULL
                            };

                            char *feedback_support_options[] = {
                                "🔄 Modify an existing booking  - Request changes to your current booking.",
                                "💵 Payment-related queries     - Get help with payment issues or balances.",
                                "⚠️ Report an issue with an event - Notify us of any problems with your event.",
                                "📋 Other                       - For all other queries and feedback.",
                                "🔙 Back to Dashboard           - Return to the main menu.",
                                NULL
                            };

                            display_options(NULL, feedback_support_menu, feedback_support_options, "Enter your choice:");

                            int choice;
                            if (scanf("%d", &choice) != 1 || choice < 1 || choice > 5)
                            {
                                printf("\n❌ Invalid input. Please select a valid option (1-5).\n");
                                Sleep(2000);
                                clear_input_buffer();
                                continue;
                            }
                            clear_input_buffer();

                            if (choice == 5) // Back to Dashboard
                            {
                                printf("\n🔙 Returning to the dashboard...\n");
                                Sleep(2000);
                                break;
                            }

                            char ticket_message[256];
                            if (choice == 4) // Custom feedback
                            {
                                char custom_reason[100];
                                printf("\n📋 You selected 'Other'. Please specify the nature of your request: ");
                                fgets(custom_reason, sizeof(custom_reason), stdin);
                                custom_reason[strcspn(custom_reason, "\n")] = 0; // Remove trailing newline

                                if (strlen(custom_reason) == 0)
                                {
                                    printf("\n❌ Custom reason cannot be empty. Returning to the menu.\n");
                                    Sleep(2000);
                                    continue;
                                }

                                snprintf(ticket_message, sizeof(ticket_message), "Custom Request: %s", custom_reason);
                            }
                            else
                            {
                                printf("\n📝 Enter the details of your request (or type '-1' to cancel): ");
                                fgets(ticket_message, sizeof(ticket_message), stdin);
                                ticket_message[strcspn(ticket_message, "\n")] = 0; // Remove trailing newline
                            }

                            if (strcmp(ticket_message, "-1") == 0)
                            {
                                printf("\n🚫 Request canceled. Returning to the previous menu.\n");
                                Sleep(2000);
                                continue;
                            }

                            int ticket_id = -1;
                            switch (choice)
                            {
                                case 1: // Modify an existing booking
                                    printf("\n🔄 Request to modify a booking noted.\n");
                                    ticket_id = set_ticket(client_id, ticket_message);
                                    break;

                                case 2: // Payment-related queries
                                    printf("\n💵 Payment-related query noted.\n");
                                    ticket_id = set_ticket(client_id, ticket_message);
                                    break;

                                case 3: // Report an issue with an event
                                    printf("\n⚠️ Event issue reported.\n");
                                    ticket_id = set_ticket(client_id, ticket_message);
                                    break;

                                case 4: // Custom feedback
                                    printf("\n📋 General feedback noted.\n");
                                    ticket_id = set_ticket(client_id, ticket_message);
                                    break;
                            }

                            if (ticket_id != -1)
                            {
                                
                                printf("==================================================\n");
                                printf("                 🆘 Support Ticket Created!       \n");
                                printf("==================================================\n");
                                printf("  Your Ticket ID: [%d]\n", ticket_id); 
                                printf("  Our team will contact you shortly.\n");
                                printf("==================================================\n");
                                Sleep(3000);
                                break;

                            }
                            else
                            {
                                printf("\n❌ Failed to create your ticket. Please try again later.\n");
                                Sleep(2000);

                                continue;
                            }
                        }

                    } else if(feedback_choice == 4)
                    {
                        break;
                    }
                
                }
                break;
            case 6:  // notifcations.
                while (1) {
                    const char *notifications_paragraph[] = {
                        "==================================================",
                        "                 🔔 Notifications                ",
                        "==================================================",
                        "📍 Home > Dashboard > Notifications",
                        "",
                        "Stay updated with the latest system alerts, updates,",
                        "and important reminders related to your account or events.",
                        NULL
                    };

                    char *notifications = view_notifications(client_id); // Function to get notifications
                    if (!notifications) {
                        printf("🎉 No notifications to display. Enjoy your day!\n");
                        Sleep(2000);
                        break;
                    }

                    system("cls");
                    display_options(NULL, notifications_paragraph, notifications, "📝 Actions:\n [1] Mark all as read    [2] Clear All   [3] Stop viewing");

                    char action = getch();

                    if(action == '1')
                    {
                        if (mark_all_notifications_read(client_id) == 1) {
                            printf("\n✅ All notifications have been marked as read.\n");
                        } else {
                            printf("\n❌ Failed to mark notifications as read. Please try again.\n");
                        }
                        Sleep(2000);
                    } else if(action == '2')
                    {
                        if (clear_notifications(client_id) == 1) {
                            printf("\n🧹 All notifications have been cleared.\n");
                        } else {
                            printf("\n❌ Failed to clear notifications. Please try again.\n");
                        }
                        Sleep(2000);
                    } else if(action == '3')
                    {
                        printf("\n🔙 Returning to the previous menu...\n");
                        Sleep(2000);
                        free(notifications);
                        break;
                    } else 
                    {
                        printf("\n❌ Invalid input. Please try again.\n");
                        Sleep(2000);
                    }
                    
                }
                break;
            case 7: 
                // confirmation first
                // then logout
                while(1)
                {
                    char confirmation;
                    system("cls");
                    printf("==================================================\n");
                    printf("                 🔒 Logout Confirmation           \n");
                    printf("==================================================\n");
                    printf("Are you sure you want to log out?\n");
                    printf("Your unsaved progress may be lost.\n");
                    printf("--------------------------------------------------\n");
                    printf("   Press 'Y' to log out or 'N' to stay logged in. \n");
                    printf("==================================================\n");
                    printf("👉 Enter your choice: ");
                    
                    scanf("%c", &confirmation);

                    if(confirmation == 'Y' || confirmation == 'y')
                    {
                        update_last_login("stay_logged_in", "0");
                        update_user(client_id, "stay_logged_in", "0");
                        auth_menu();
                    } else if(confirmation == 'N' || confirmation == 'n')
                    {
                        break;
                    } else 
                    {
                        printf("\nPlease pick a valid choice\n");
                        break;
                    }
                }
                break;

        }
    }
}

// highlight ticketing/feedback system
// highlight 
// highlight management for users,events, packages.
// highlight the data & analytics 
// highlight the reminder thing
// highlight; customization 
// highlight export for csv/excel file.

// implement export for reminders, booked events, then the statistics.

// i.e
// user/{id}/booked_event_report_1.csv
// and so on...
int organizer_menu(int organizer_id)
{
    printf("%s\n", read_user(organizer_id, "full_name"));
    int organizer_choice;

    while(1)
    {
        system("cls");
        int event_choice;
        int package_choice, event_id;

        char *prev_reminder = view_reminders(organizer_id);
        char *stat_summary = generate_statistics_summary();
        char organizer_name[50];

        sprintf(organizer_name,"Welcome, %s! Manage your events effortlessly.", read_user(organizer_id, "full_name"));

        char *dashboard_paragraph[] =  {
            "==================================================",
            "              📊 Organizer Dashboard",
            "==================================================",
            "",
            organizer_name,
            "",
            "🌟 Navigation: Home > Organizer Dashboard",
            "",
            get_current_date_time(),
            "",
            stat_summary,
            "",
            prev_reminder,
            "🔧 Select an option to manage your tasks:",
            "==================================================",

            NULL
        };
        char *dashboard_choices[] = {
            "Dashboard Overview       📈 - View key stats and metrics.",
            "Event Management         🎉 - Create, update, or cancel events.",
            "Package Management       📦 - Modify or remove event packages.",
            "Manage Reminders         📅 - Set, update, or view reminders.",
            "Manage Users             👥 - Add, edit, or deactivate users.",
            "Notification and Logs    🔔 - Check notifications and activity logs.",
            "Manage Feedback & Tickets 💬 - Handle client feedback and tickets.",
            "Logout                   🔑 - Log out securely.",
            NULL
        };


        
        display_options(NULL, dashboard_paragraph, dashboard_choices, NULL);
        if(scanf("%d", &organizer_choice) != 1 || organizer_choice < 1 || organizer_choice > 8)
        {
            printf("\nPlease enter a valid choice\n");
            Sleep(2000);
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if(organizer_choice == 1) // dashboard overview
        {
            while(1)
            {
                char *total_events_booked_menu[] = 
                {
                "==================================================",
                "             📊 Total Events Booked Overview      ",
                "==================================================",
                "",
                "📍 Home > Dashboard > Total Events Booked",
                "",
                "==================================================",
                NULL
                };


                char *total_events_booked_options[] = 
                {
                    "By Event Type        📂 - View total bookings per type (e.g., Wedding, Corporate).",
                    "By Month             📅 - View total bookings per month.",
                    "By Venue             📍 - View total bookings per venue.",
                    "View All Events      📋 - See a list of all booked events.",
                    "Export Report        📤 - Save the summary as a CSV or text file.",
                    "Back to Dashboard    🔙 - Return to the main dashboard.",
                    NULL
                };
                display_options(NULL, total_events_booked_menu, total_events_booked_options, NULL);
                
                if(scanf("%d", &organizer_choice) != 1 || organizer_choice < 1 || organizer_choice > 6)
                {
                    printf("\nPlease select a valid option.");
                    Sleep(2000);
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();

                // total event submenu.
                if(organizer_choice == 1) // by event type.
                {
                    system("cls");
                    total_bookings_by_event_type();
                    
                    if(getch() == '\r')
                    {
                        continue;
                    }
                } else if(organizer_choice == 2) // by month
                {
                    system("cls");
                    total_bookings_by_month();
                    if(getch() == '\r')
                    {
                        continue;
                    }
                } else if(organizer_choice == 3) // by venue
                {
                    system("cls");
                    total_bookings_by_venue();
                    if(getch() == '\r')
                    {
                        continue;
                    }
                } else if(organizer_choice == 4) // view all events
                {
                    while(1)
                    {
                        int user_id;
                        system("cls");
                        reveal_all_users_except_organizers();
                        scanf("%d", &user_id);

                        if(user_id == -1)
                        {
                            break;
                        }

                        if(valid_user_id(user_id) != 1)
                        {
                            printf("\n Please enter a valid user id\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        while(1)
                        {
                            char *booked_events_menu[] = {
                                "==================================================",
                                "                  📅 Booked Events               ",
                                "==================================================",
                                "",
                                "📍 Home > Dashboard > My Events > View Booked Events",
                                "",
                                "🔧 Select a booked event to view its details:",
                                "",
                                preview_all_events(user_id),
                                NULL // Null-terminator
                            };
                            
                            int *book_id = NULL;
                            int event_count = 0;


                            display_options(NULL, booked_events_menu, NULL, "Please enter [ANY KEY] to stop viewing");

                            if(getch() == '\r')
                            {
                                break;
                            }

                        }
                    }
                    // ask for user_id
                    // preview all users first.
                    // then you print all its event.
                    // enter to go back

                } else if(organizer_choice == 5) // export report
                {
                    system("cls");
                    generate_event_report(organizer_id);
                    if(getch() == '\r')
                    {
                        continue;
                    }
                    // export report.
                } else if(organizer_choice == 6) // back to dashboard.
                {
                    break;
                }

            }
        } else if (organizer_choice == 2) // event management
        {
            while(1)
            {
                char *event_management_menu[] = {
                    "==================================================",
                    "                 🎉 Event Management              ",
                    "==================================================",
                    "📍 Home > Dashboard > Event Management",
                    "",
                    "==================================================",
                    NULL
                };


                char *event_management_options[] = {
                    "Create Event         🆕 - Start organizing a new event.",
                    "Update Event         ✏️  - Modify details of an existing event.",
                    "Delete Event         ❌ - Permanently remove an event type.",
                    "View Event Details   🔍 - Review specific event information.",
                    "Back to Dashboard    🔙 - Return to the main dashboard.",
                    NULL
                };

                display_options(NULL, event_management_menu, event_management_options, NULL);

                if(scanf("%d", &organizer_choice) != 1 || organizer_choice < 1 || organizer_choice > 5)
                {
                    printf("\n Please select a valid option\n");
                    Sleep(2000);
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();
            
                if(organizer_choice == 1) // create event
                {

                    char event_input[5][100] = {"", "", "", "", ""};
                    int steps_completed = 0;

                    while (1) 
                    {
                        steps_completed = 0;
                        for (int i = 0; i < 5; i++) 
                        {
                            if (strlen(event_input[i]) > 0) 
                            {
                                steps_completed++;
                            }
                        }

                        char progress[50];
                        if (steps_completed < 5) 
                        {
                            sprintf(progress, "Event Creation Progress: [%.*s%.*s] %d%%",
                                    (steps_completed * 20) / 5, "####################",
                                    20 - (steps_completed * 20) / 5, "--------------------",
                                    (steps_completed * 100) / 5);
                        } 
                        else 
                        {
                            sprintf(progress, "Event Creation Progress: [%s####################%s] 100%%", GREEN, RESET);
                        }

                        const char *create_event_menu[] = {
                            "==================================================",
                            "              🛠️ Create Event Type               ",
                            "==================================================",
                            "",
                            "🔧 Fill in the required fields to define an event type.",
                            "",
                            "==================================================",
                            "",
                            progress,
                            "",
                            "==================================================",
                            NULL
                        };

                        char *event_options[] = {"Event Name\t\t", "Description\t\t", "Venues\t\t", "Down Payment Percentage", "Payment Deadline Days"};

                        char *menu_options[7]; // 5 fields + "Create Event" + "Cancel" + NULL
                        for (int i = 0; i < 5; i++) 
                        {
                            menu_options[i] = malloc(100);
                            if (menu_options[i] != NULL) 
                            {
                                snprintf(menu_options[i], 100, "%s\t\t[%s%s%s]",
                                        event_options[i],
                                        strlen(event_input[i]) > 0 ? GREEN : RED,
                                        strlen(event_input[i]) > 0 ? "Filled" : "Empty",
                                        RESET);
                            }
                        }
                        menu_options[5] = strdup("Create Event");
                        menu_options[6] = strdup("Cancel");
                        menu_options[7] = NULL;

                        int menu_choice;
                        display_options(NULL, create_event_menu, menu_options, "Enter the corresponding number for the field to update:");

                        if (scanf("%d", &menu_choice) != 1 || menu_choice < 1 || menu_choice > 7) 
                        {
                            printf("\n❌ Invalid choice. Please select a valid option.\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        if (menu_choice == 1) 
                        { // Event Name
                            printf("\nEnter the Event Name (4-40 characters): ");
                            fgets(event_input[0], sizeof(event_input[0]), stdin);
                            event_input[0][strcspn(event_input[0], "\n")] = 0;

                            if (strlen(event_input[0]) < 4 || strlen(event_input[0]) > 40) 
                            {
                                printf("\n❌ Invalid length! Event Name must be between 4-40 characters.\n");
                                Sleep(2000);
                            }
                        } 
                        else if (menu_choice == 2) 
                        { // Description
                            printf("\nEnter a brief description of the event: ");
                            fgets(event_input[1], sizeof(event_input[1]), stdin);
                            event_input[1][strcspn(event_input[1], "\n")] = 0;
                        } 
                        else if (menu_choice == 3) 
                        { // Venues
                            printf("\nEnter the available venues separated by commas (e.g., School,Jabee,Mcdo): ");
                            fgets(event_input[2], sizeof(event_input[2]), stdin);
                            event_input[2][strcspn(event_input[2], "\n")] = 0;
                        } 
                        else if (menu_choice == 4) 
                        { // Down Payment Percentage
                            while (1) 
                            {
                                printf("\nEnter the required down payment percentage (e.g., 50 for 50%%): ");
                                fgets(event_input[3], sizeof(event_input[3]), stdin);
                                event_input[3][strcspn(event_input[3], "\n")] = 0;

                                int dp_percentage = atoi(event_input[3]);
                                if (dp_percentage < 0 || dp_percentage > 100) 
                                {
                                    printf("\n❌ Invalid percentage! Please enter a value between 0-100.\n");
                                    Sleep(2000);
                                } 
                                else 
                                {
                                    break;
                                }
                            }
                        } 
                        else if (menu_choice == 5) 
                        { // Payment Deadline Days
                            while (1) 
                            {
                                printf("\nEnter the number of days after booking for payment deadline: ");
                                fgets(event_input[4], sizeof(event_input[4]), stdin);
                                event_input[4][strcspn(event_input[4], "\n")] = 0;

                                int deadline_days = atoi(event_input[4]);
                                if (deadline_days <= 0) 
                                {
                                    printf("\n❌ Invalid input! Days must be greater than 0.\n");
                                    Sleep(2000);
                                } 
                                else 
                                {
                                    break;
                                }
                            }
                        } 
                        else if (menu_choice == 6) 
                        { // Create Event
                            if (steps_completed < 5) 
                            {
                                printf("\n❌ All fields must be filled before creating the event.\n");
                                Sleep(2000);
                                continue;
                            }

                            // Review entered information
                            printf("\n==================================================\n");
                            printf("              📋 Review Event Details             \n");
                            printf("==================================================\n");
                            printf("Event Name            : %s\n", event_input[0]);
                            printf("Description           : %s\n", event_input[1]);
                            printf("Venues                : %s\n", event_input[2]);
                            printf("Down Payment Percentage: %s%%\n", event_input[3]);
                            printf("Payment Deadline Days : %s days\n", event_input[4]);
                            printf("==================================================\n");
                            printf("\n❓ Confirm Event Creation (Y/N): ");

                            char confirmation;
                            scanf(" %c", &confirmation);
                            clear_input_buffer();

                            if (confirmation != 'Y' && confirmation != 'y') 
                            {
                                printf("\n🚫 Event creation canceled. Returning to the previous menu.\n");
                                Sleep(2000);
                                continue;
                            }

                            // Create the event type
                            TypeEvent type_event =
                            {
                                .name = "",
                                .description = "", 
                                .venues = ""
                            };

                            type_event.id = generate_typeevent_id();
                            strcpy(type_event.name, event_input[0]);
                            strcpy(type_event.description, event_input[1]);
                            strcpy(type_event.venues, event_input[2]);
                            type_event.dp_percentage = atoi(event_input[3]);
                            type_event.payment_deadline_days = atoi(event_input[4]);

                            if (create_typeevent(type_event) != 1) 
                            {
                                printf("\n❌ Creation Failed. Please Try Again Later...\n");
                                memset(&event_input, 0, sizeof(event_input));
                                Sleep(2000);
                                continue;
                            }

                            printf("\n✅ Event Type '%s' created successfully!\n", event_input[0]);
                            Sleep(2000);
                            break;

                        } 
                        else if (menu_choice == 7) 
                        { // Cancel
                            printf("\n🚫 Event Type creation canceled. Returning to the previous menu.\n");
                            memset(&event_input, 0, sizeof(event_input));
                            break;
                        }
                    } 
                } else if(organizer_choice == 2) // update event
                {
                    // preview list of event type first
                    // then ask for id
                    // then ask for preview the view to update

                    while (1)
                    {
                        char *preview_event = preview_event_type();
                        if (preview_event == NULL) 
                        {
                            printf("\n❌ No events found to update. Returning to the previous menu.\n");
                            Sleep(2000);
                            break;
                        }

                        int event_type_id;
                        const char *event_selection_menu[] = 
                        {
                            "==================================================",
                            "           🛠️ EventEase Dashboard > Event Management     ",
                            "==================================================",
                            "",
                            "==================================================",
                            "        📝 Select the Kind of Event to Update      ",
                            "==================================================",
                            preview_event,
                            NULL
                        };

                        display_options(NULL, event_selection_menu, NULL, "Please select ID to update (-1 to go back):");

                        if (scanf("%d", &event_type_id) != 1 || (valid_typeevent_id(event_type_id) != 1 && event_type_id != -1))
                        {
                            printf("\n❌ Please select a valid event type ID.\n");
                            clear_input_buffer();
                            Sleep(2000);
                            continue;
                        }
                        clear_input_buffer();

                        if (event_type_id == -1)
                        {
                            break;
                        }

                        char event_update_input[5][100] = {"", "", "", "", ""};

                        char *existing_event_name = read_typevent(event_type_id, "event_name");
                        char *existing_description = read_typevent(event_type_id, "description");
                        char *existing_venues = read_typevent(event_type_id, "venues");
                        char *existing_dp_percentage = read_typevent(event_type_id, "dp_percentage");
                        char *existing_payment_deadline_days = read_typevent(event_type_id, "payment_deadline_days");

                        // Validate read pointers
                        if (!existing_event_name || !existing_description || !existing_venues || !existing_dp_percentage || !existing_payment_deadline_days)
                        {
                            printf("\n❌ Failed to fetch event details. Returning to the previous menu.\n");
                            free(preview_event);
                            free(existing_event_name);
                            free(existing_description);
                            free(existing_venues);
                            free(existing_dp_percentage);
                            free(existing_payment_deadline_days);
                            Sleep(2000);
                            break;
                        }

                        strncpy(event_update_input[0], existing_event_name, sizeof(event_update_input[0]) - 1);
                        strncpy(event_update_input[1], existing_description, sizeof(event_update_input[1]) - 1);
                        strncpy(event_update_input[2], existing_venues, sizeof(event_update_input[2]) - 1);
                        strncpy(event_update_input[3], existing_dp_percentage, sizeof(event_update_input[3]) - 1);
                        strncpy(event_update_input[4], existing_payment_deadline_days, sizeof(event_update_input[4]) - 1);

                        free(existing_event_name);
                        free(existing_description);
                        free(existing_venues);
                        free(existing_dp_percentage);
                        free(existing_payment_deadline_days);

                        while (1)
                        {
                            const char *update_event_menu[] = {
                                "==================================================",
                                "              🛠️ Update Event Type               ",
                                "==================================================",
                                "",
                                "🔧 Update the fields for the selected event type.",
                                "",
                                "==================================================",
                                NULL
                            };

                            char *event_options[] = {
                                "Event Name",
                                "Description",
                                "Venues",
                                "Down Payment Percentage",
                                "Payment Deadline Days",
                                "Save Updates",
                                "Cancel Updates",
                                NULL
                            };

                            display_options(NULL, update_event_menu, event_options, "Enter the corresponding number for the field to update:");

                            int menu_choice;
                            if (scanf("%d", &menu_choice) != 1 || menu_choice < 1 || menu_choice > 7)
                            {
                                printf("\n❌ Invalid choice. Please select a valid option.\n");
                                clear_input_buffer();
                                Sleep(2000);
                                continue;
                            }
                            clear_input_buffer();
                            if (menu_choice == 1) 
                            { // Event Name
                                printf("\nEnter the Event Name (4-40 characters): ");
                                fgets(event_update_input[0], sizeof(event_update_input[0]), stdin);
                                event_update_input[0][strcspn(event_update_input[0], "\n")] = 0;

                                if (strlen(event_update_input[0]) < 4 || strlen(event_update_input[0]) > 40) 
                                {
                                    printf("\n❌ Invalid length! Event Name must be between 4-40 characters.\n");
                                    Sleep(2000);
                                }
                            } 
                            else if (menu_choice == 2) 
                            { // Description
                                printf("\nEnter a brief description of the event: ");
                                fgets(event_update_input[1], sizeof(event_update_input[1]), stdin);
                                event_update_input[1][strcspn(event_update_input[1], "\n")] = 0;
                            } 
                            else if (menu_choice == 3) 
                            { // Venues
                                printf("\nEnter the available venues separated by commas (e.g., School,Jabee,Mcdo): ");
                                fgets(event_update_input[2], sizeof(event_update_input[2]), stdin);
                                event_update_input[2][strcspn(event_update_input[2], "\n")] = 0;
                            } 
                            else if (menu_choice == 4) 
                            { // Down Payment Percentage
                                while (1) 
                                {
                                    printf("\nEnter the required down payment percentage (e.g., 50 for 50%%): ");
                                    fgets(event_update_input[3], sizeof(event_update_input[3]), stdin);
                                    event_update_input[3][strcspn(event_update_input[3], "\n")] = 0;

                                    int dp_percentage = atoi(event_update_input[3]);
                                    if (dp_percentage < 0 || dp_percentage > 100) 
                                    {
                                        printf("\n❌ Invalid percentage! Please enter a value between 0-100.\n");
                                        Sleep(2000);
                                    } 
                                    else 
                                    {
                                        break;
                                    }
                                }
                            } 
                            else if (menu_choice == 5) 
                            { // Payment Deadline Days
                                while (1) 
                                {
                                    printf("\nEnter the number of days after booking for payment deadline: ");
                                    fgets(event_update_input[4], sizeof(event_update_input[4]), stdin);
                                    event_update_input[4][strcspn(event_update_input[4], "\n")] = 0;

                                    int deadline_days = atoi(event_update_input[4]);
                                    if (deadline_days <= 0) 
                                    {
                                        printf("\n❌ Invalid input! Days must be greater than 0.\n");
                                        Sleep(2000);
                                    } 
                                    else 
                                    {
                                        break;
                                    }
                                }
                            }
                            if (menu_choice == 6) // Save Updates
                            {
                                printf("\nSaving updates...\n");
                                Sleep(2000);

                                if (update_typeevent(event_type_id, "event_name", event_update_input[0]) != 1 ||
                                    update_typeevent(event_type_id, "description", event_update_input[1]) != 1 ||
                                    update_typeevent(event_type_id, "venues", event_update_input[2]) != 1 ||
                                    update_typeevent(event_type_id, "dp_percentage", event_update_input[3]) != 1 ||
                                    update_typeevent(event_type_id, "payment_deadline_days", event_update_input[4]) != 1)
                                {
                                    printf("\n❌ Failed to update the event type. Please try again later.\n");
                                    continue;
                                }

                                printf("\n✅ Event Type updated successfully!\n");
                                Sleep(2000);
                                break;
                            }
                            else if (menu_choice == 7) // Cancel Updates
                            {
                                printf("\n🚫 Updates canceled. Returning to the previous menu.\n");
                                break;
                            }
                            else
                            {
                                // Handle field-specific updates (similar to original code, omitted here for brevity)
                            }
                        }

                        free(preview_event); // Free allocated memory
                    }
                

                } else if(organizer_choice == 3) // del;ete event
                {
                    // preview event list then ask for id then confirmation.
                    while(1)
                    {
                        char *preview_event = preview_event_type();
                        const char *event_selection_menu[] = 
                        {
                            "==================================================",
                            "           ❌ EventEase Dashboard > Event Management     ",
                            "==================================================",
                            "",
                            "==================================================",
                            "        📝 Select the Kind of Event to Delete      ",
                            "==================================================",
                            preview_event,
                            NULL
                        };

                        display_options(NULL, event_selection_menu, NULL, "Please select ID tp update (-1 to go back):");

                        if(scanf("%d", &organizer_choice) != 1 || (valid_typeevent_id(organizer_choice) != 1 && organizer_choice != -1))
                        {
                            printf("\nPlease select a valid event type id\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        } 
                        if(organizer_choice == -1)
                        {
                            break;
                        }

                        while(1)
                        {
                            char confirmation;

                            system("cls");
                            printf("==================================================\n");
                            printf("             ❌ Delete Event Type                   \n");
                            printf("==================================================\n\n");
                            printf("⚠️ Are you sure you want to delete event: %d\n", organizer_choice);
                            printf("==================================================\n\n");
                            printf("Type 'Y' to confirm or 'N' to cancel: ");

                            scanf(" %c", &confirmation);
                            clear_input_buffer();

                            if (confirmation == 'Y' || confirmation == 'y') 
                            {
                                // put here
                                break;
                            } else if(confirmation == 'N' || confirmation == 'n')
                            {
                                break;
                            } else 
                            {
                                printf("\n🚫 Please selct a valid option.\n");
                                Sleep(2000);
                                continue;

                            }
                        }  

                    }
                
                } else if(organizer_choice == 4) //  View Event Details
                {
                    while(1)
                    {
                        char *preview_event = preview_event_type();
                        const char *event_selection_menu[] = 
                        {
                            "==================================================",
                            "    📝 EventEase Dashboard > Event Management     ",
                            "==================================================",
                            "",
                            "==================================================",
                            "   📝 Select the Kind of Event to Preview Details     ",
                            "==================================================",
                            "",
                            preview_event,
                            NULL
                        };

                        display_options(NULL, event_selection_menu, NULL, "Please select ID tp update (-1 to go back):");

                        if(scanf("%d", &organizer_choice) != 1 || (valid_typeevent_id(organizer_choice) != 1 && organizer_choice != -1))
                        {
                            printf("\nPlease select a valid event type id\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        } 
                        if(organizer_choice == -1)
                        {
                            break;
                        }

                        while(1)
                        {
                            system("cls");
                            char *preview_pkg = preview_pkgs(organizer_choice);
                            printf("==================================================\n");
                            printf("              🎉 Event Details: %d               \n", organizer_choice);
                            printf("==================================================\n\n");
                            printf("📛 Event Name       : %s\n", read_typevent(organizer_choice, "event_name"));
                            printf("📝 Description      : %s\n", read_typevent(organizer_choice, "description"));
                            printf("📍 Venues           : %s\n", read_typevent(organizer_choice, "venues"));
                            printf("📅 Date Created     : %s\n\n", read_typevent(organizer_choice, "date_created"));


                            if (preview_pkg == NULL) {
                                printf("⚠️  Packages         : NO PACKAGE AVAILABLE\n\n");
                            } else {
                                printf("📦 Packages         :\n%s\n", preview_pkg);
                            }

                            printf("==================================================\n\n");
                            printf("👉 Enter [ANY KEY] to stop viewing.\n");

                            if(getch() == '\r')
                            {
                                break;
                            }
                        }
                    }
                    // if clicked it will show it's packages and what not?
                } else if(organizer_choice == 5) // back to dashboard
                {
                    break;
                }
            }       
        } else if(organizer_choice == 3) // package management 
        {
            while(1)
            {
                char *package_management_menu[] = {
                    "==================================================",
                    "               📦 Package Management              ",
                    "==================================================",
                    "📍 Home > Dashboard > Package Management",
                    "",
                    "==================================================",
                    NULL
                };


                char *package_management_submenu[] = {
                    "Add Package          ➕ - Create a new package for your events.",
                    "Edit Package         ✏️  - Modify details of an existing package.",
                    "Delete Package       ❌ - Remove an event package permanently.",
                    "View Packages        🔍 - Explore all available packages.",
                    "Back to Dashboard    🔙 - Return to the main dashboard.",
                    NULL
                };


                display_options(NULL, package_management_menu, package_management_submenu, NULL);

                if(scanf("%d", &organizer_choice) != 1 || organizer_choice < 1 || organizer_choice > 5)
                {
                    printf("\nPlease select a valid event a valid choice.\n");
                    Sleep(2000);
                    clear_input_buffer();
                    continue;
                } 
                clear_input_buffer();

                if(organizer_choice == 1) // add package
                {
                    char package_input[6][100] = {"", "", "", "", "", ""};
                    int steps_completed = 0;
                    int event_type_id = 0;

                    // Select the event type first
                    while (1) {
                        char *preview_event = preview_event_type();
                        const char *event_selection_menu[] = {
                            "==================================================",
                            "             🛠️ EventEase Dashboard > Add Package",
                            "==================================================",
                            "",
                            "==================================================",
                            "       📝 Select the Event Type for the Package    ",
                            "==================================================",
                            preview_event,
                            NULL
                        };

                        display_options(NULL, event_selection_menu, NULL, "Please select Event Type ID (-1 to go back):");

                        if (scanf("%d", &event_type_id) != 1 || (valid_typeevent_id(event_type_id) != 1 && event_type_id != -1)) {
                            printf("\n❌ Please select a valid event type ID.\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        if (event_type_id == -1) {
                            printf("\n🚫 Package creation canceled. Returning to the previous menu.\n");
                            break;
                        }

                        while (1) 
                        {
                            steps_completed = 0;
                            for (int i = 0; i < 6; i++) {
                                if (strlen(package_input[i]) > 0) {
                                    steps_completed++;
                                }
                            }

                            const char *add_package_menu[] = {
                                "==================================================",
                                "                  📦 Add New Package              ",
                                "==================================================",
                                "",
                                "🔧 Fill in the required fields to define a new package.",
                                "",
                                "==================================================",
                                NULL
                            };

                            char *package_options[] = 
                            {
                                "Package Name\t\t\t", 
                                "Price\t\t\t", 
                                "Description\t\t\t", 
                                "Max Guest Count\t\t", 
                                "Duration (in hours)\t\t", 
                                "Inclusions (comma-separated)"
                            };

                            char *menu_options[8]; // 6 fields + "Create Package" + "Cancel" + NULL
                            for (int i = 0; i < 6; i++) 
                            {
                                menu_options[i] = malloc(100);
                                if (menu_options[i] != NULL) {
                                    snprintf(menu_options[i], 100, "%s \t\t%s[%s]%s",
                                            package_options[i],
                                            strlen(package_input[i]) > 0 ? GREEN : RED,
                                            strlen(package_input[i]) > 0 ? "Filled" : "Empty", RESET);
                                }
                            }
                            menu_options[6] = strdup("Create Package");
                            menu_options[7] = strdup("Cancel");
                            menu_options[8] = NULL;

                            int menu_choice;
                            display_options(NULL, add_package_menu, menu_options, "Enter the corresponding number for the field to update:");

                            if (scanf("%d", &menu_choice) != 1 || menu_choice < 1 || menu_choice > 8) 
                            {
                                printf("\n❌ Invalid choice. Please select a valid option.\n");
                                Sleep(2000);
                                clear_input_buffer();
                                continue;
                            }
                            clear_input_buffer();

                            if (menu_choice == 1) 
                            { // Package Name
                                printf("\nEnter the Package Name (4-40 characters): ");
                                fgets(package_input[0], sizeof(package_input[0]), stdin);
                                package_input[0][strcspn(package_input[0], "\n")] = 0;

                                if (strlen(package_input[0]) < 4 || strlen(package_input[0]) > 40) 
                                {
                                    printf("\n❌ Invalid length! Package Name must be between 4-40 characters.\n");
                                    Sleep(2000);
                                }
                            } else if (menu_choice == 2) { // Price
                                while (1) 
                                {
                                    printf("\nEnter the price for the package (e.g., 10000.00): ");
                                    fgets(package_input[1], sizeof(package_input[1]), stdin);
                                    package_input[1][strcspn(package_input[1], "\n")] = 0;

                                    if (atof(package_input[1]) <= 0) 
                                    {
                                        printf("\n❌ Invalid input! Price must be greater than 0.\n");
                                        Sleep(2000);
                                    } else 
                                    {
                                        break;
                                    }
                                }
                            } else if (menu_choice == 3) 
                            { // Description
                                printf("\nEnter a brief description of the package: ");
                                fgets(package_input[2], sizeof(package_input[2]), stdin);
                                package_input[2][strcspn(package_input[2], "\n")] = 0;
                            } else if (menu_choice == 4) 
                            { // Max Guest Count
                                while (1) 
                                {
                                    printf("\nEnter the maximum number of guests allowed (e.g., 100): ");
                                    fgets(package_input[3], sizeof(package_input[3]), stdin);
                                    package_input[3][strcspn(package_input[3], "\n")] = 0;

                                    if (atoi(package_input[3]) <= 0) 
                                    {
                                        printf("\n❌ Invalid input! Guest count must be greater than 0.\n");
                                        Sleep(2000);
                                    } else 
                                    {
                                        break;
                                    }
                                }
                            } else if (menu_choice == 5) 
                            { // Duration
                                while (1) 
                                {
                                    printf("\nEnter the duration of the package in hours (e.g., 5): ");
                                    fgets(package_input[4], sizeof(package_input[4]), stdin);
                                    package_input[4][strcspn(package_input[4], "\n")] = 0;

                                    if (atoi(package_input[4]) < 0) 
                                    {
                                        printf("\n❌ Invalid input! Duration must be 0 or greater.\n");
                                        Sleep(2000);
                                    } else 
                                    {
                                        break;
                                    }
                                }
                            } else if (menu_choice == 6) 
                            { // Inclusions
                                printf("\nEnter the inclusions for the package, separated by commas (e.g., Food,Decor,Music): ");
                                fgets(package_input[5], sizeof(package_input[5]), stdin);
                                package_input[5][strcspn(package_input[5], "\n")] = 0;
                                
                            } else if (menu_choice == 7) 
                            { // Create Package
                                if (package_input[0] != NULL && package_input[1] != NULL && package_input[2] != NULL && package_input[3] != NULL && package_input[4] != NULL && package_input[5] && NULL) 
                                {
                                    printf("\n❌ All fields must be filled before creating the package.\n");
                                    Sleep(2000);
                                    continue;
                                }
                                while(1)
                                {
                                        // Review entered information
                                    system("cls");
                                    printf("\n==================================================\n");
                                    printf("              📋 Review Package Details           \n");
                                    printf("==================================================\n");
                                    printf("Package Name          : %s\n", package_input[0]);
                                    printf("Price                 : PHP %s\n", format_number(atoi(package_input[1])));
                                    printf("Description           : %s\n", package_input[2]);
                                    printf("Max Guest Count       : %s\n", package_input[3]);
                                    printf("Duration              : %s hours\n", package_input[4]);
                                    printf("Inclusions            : %s\n", package_input[5]);
                                    printf("Event Type ID         : %d\n", event_type_id);

                                    
                                    printf("==================================================\n");
                                    printf("\n❓ Confirm Package Creation (Y/N): ");

                                    char confirmation;
                                    scanf("%c", &confirmation);
                                    clear_input_buffer();

                                    if (confirmation == 'Y' || confirmation == 'y') 
                                    {
                                        Package package = {
                                            .id = generate_pkg_id(event_type_id),
                                            .price = atof(package_input[1]),
                                            .max_guest = atoi(package_input[3]),
                                            .duration = atoi(package_input[4]),
                                            .event_type = event_type_id
                                        };

                                        strcpy(package.package_name, package_input[0]);
                                        strcpy(package.description, package_input[2]);
                                        strcpy(package.inclusions, package_input[5]);
                                        package.availability = 1;
                                        if (add_pkg(event_type_id, package) != 1) {
                                            printf("\n❌ Package creation failed. Please try again later.\n");
                                            Sleep(2000);
                                            memset(&package_input, 0, sizeof(package_input));
                                            break;
                                        }

                                        printf("\n✅ Package '%s' created successfully!\n", package_input[0]);
                                        memset(&package_input, 0, sizeof(package_input));

                                        Sleep(2000);
                                        break;
                                    } else if(confirmation == 'N' || confirmation == 'n')
                                    {
                                        break;
                                    } else 
                                    {
                                        printf("\n🚫 Please selct a valid option.\n");
                                        Sleep(2000);
                                        continue;
                                    }
                                }
                                

                                // Store the package
                                
                            } else if (menu_choice == 8) { // Cancel
                                printf("\n🚫 Package creation canceled. Returning to the previous menu.\n");
                                memset(&package_input, 0, sizeof(package_input));
                                break;
                            }
                        }
                    }
                } else if(organizer_choice == 2) // Update package
                {
                    while (1)
                    {
                        char *preview_event = preview_event_type();
                        if (preview_event == NULL) 
                        {
                            printf("\n❌ No events found to update. Returning to the previous menu.\n");
                            Sleep(2000);
                            break;
                        }

                        int event_type_id;
                        const char *event_selection_menu[] = 
                        {
                            "==================================================",
                            "           🛠️ EventEase Dashboard > Event Management     ",
                            "==================================================",
                            "",
                            "==================================================",
                            "        📝 Select the Kind of Event to Update      ",
                            "==================================================",
                            preview_event,
                            NULL
                        };

                        display_options(NULL, event_selection_menu, NULL, "Please select ID to update (-1 to go back):");

                        if (scanf("%d", &event_type_id) != 1 || (valid_typeevent_id(event_type_id) != 1 && event_type_id != -1))
                        {
                            printf("\n❌ Please select a valid event type ID.\n");
                            clear_input_buffer();
                            Sleep(2000);
                            continue;
                        }
                        clear_input_buffer();

                        if (event_type_id == -1)
                        {
                            break;
                        }

                        while (1)
                        {
                            char *preview_package = preview_pkgs(event_type_id); // Preview packages function should return all available packages
                            if (preview_package == NULL)
                            {
                                printf("\n❌ No packages found to update. Returning to the previous menu.\n");
                                Sleep(2000);
                                break;
                            }

                            int package_id;
                            const char *package_selection_menu[] = 
                            {
                                "==================================================",
                                "           🛠️ EventEase Dashboard > Package Management",
                                "==================================================",
                                "",
                                "==================================================",
                                "        📝 Select the Package to Update            ",
                                "==================================================",
                                preview_package,
                                NULL
                            };

                            display_options(NULL, package_selection_menu, NULL, "Please select Package ID to update (-1 to go back):");

                            if (scanf("%d", &package_id) != 1 || (valid_pkg_id(event_type_id,package_id) != 1 && package_id != -1))
                            {
                                printf("\n❌ Please select a valid package ID.\n");
                                clear_input_buffer();
                                Sleep(2000);
                                continue;
                            }
                            clear_input_buffer();

                            if (package_id == -1)
                            {
                                break;
                            }

                            char package_update_input[5][100] = {"", "", "", "", ""};

                            char *existing_package_name = read_pkg(event_type_id, package_id, "package_name");
                            char *existing_description = read_pkg(event_type_id, package_id, "description");
                            char *existing_inclusions = read_pkg(event_type_id, package_id, "inclusions");
                            char *existing_price = read_pkg(event_type_id, package_id, "price");
                            char *existing_max_guest = read_pkg(event_type_id, package_id, "max_guest");
                            char *existing_ = read_pkg(event_type_id, package_id, "max_guest");


                            // Validate read pointers
                            if (!existing_package_name || !existing_description || !existing_inclusions || !existing_price || !existing_max_guest)
                            {
                                printf("\n❌ Failed to fetch package details. Returning to the previous menu.\n");
                                free(preview_package);
                                free(existing_package_name);
                                free(existing_description);
                                free(existing_inclusions);
                                free(existing_price);
                                free(existing_max_guest);
                                Sleep(2000);
                                break;
                            }

                            strncpy(package_update_input[0], existing_package_name, sizeof(package_update_input[0]) - 1);
                            strncpy(package_update_input[1], existing_description, sizeof(package_update_input[1]) - 1);
                            strncpy(package_update_input[2], existing_inclusions, sizeof(package_update_input[2]) - 1);
                            strncpy(package_update_input[3], existing_price, sizeof(package_update_input[3]) - 1);
                            strncpy(package_update_input[4], existing_max_guest, sizeof(package_update_input[4]) - 1);
                            strncpy(package_update_input[5], existing_max_guest, sizeof(package_update_input[4]) - 1);


                            free(existing_package_name);
                            free(existing_description);
                            free(existing_inclusions);
                            free(existing_price);
                            free(existing_max_guest);

                            while (1)
                            {
                                const char *update_package_menu[] = {
                                    "==================================================",
                                    "              🛠️ Update Package                  ",
                                    "==================================================",
                                    "",
                                    "🔧 Update the fields for the selected package.",
                                    "",
                                    "==================================================",
                                    NULL
                                };

                                char *package_options[] = {
                                    "Package Name          - Modify the name of the package.",
                                    "Description           - Update the package description.",
                                    "Inclusions            - Add or remove package inclusions.",
                                    "Price                 - Adjust the package price.",
                                    "Max Guest Count       - Update the maximum number of guests.",
                                    "Duration              - Update duration of package",
                                    "Save Updates          - Save all changes made.",
                                    "Cancel Updates        - Discard changes and go back.",
                                    NULL
                                };

                                display_options(NULL, update_package_menu, package_options, "Enter the corresponding number for the field to update:");

                                int menu_choice;
                                if (scanf("%d", &menu_choice) != 1 || menu_choice < 1 || menu_choice > 8) 
                                {
                                    printf("\n❌ Invalid choice. Please select a valid option.\n");
                                    Sleep(2000);
                                    clear_input_buffer();
                                    continue;
                                }
                                clear_input_buffer();

                                if (menu_choice == 1) 
                                { // Package Name
                                    printf("\nEnter the Package Name (4-40 characters): ");
                                    fgets(package_update_input[0], sizeof(package_update_input[0]), stdin);
                                    package_update_input[0][strcspn(package_update_input[0], "\n")] = 0;

                                    if (strlen(package_update_input[0]) < 4 || strlen(package_update_input[0]) > 40) 
                                    {
                                        printf("\n❌ Invalid length! Package Name must be between 4-40 characters.\n");
                                        Sleep(2000);
                                    }
                                } else if (package_update_input == 4) { // Price
                                    while (1) 
                                    {
                                        printf("\nEnter the price for the package (e.g., 10000.00): ");
                                        fgets(package_update_input[1], sizeof(package_update_input[1]), stdin);
                                        package_update_input[1][strcspn(package_update_input[1], "\n")] = 0;

                                        if (atof(package_update_input[1]) <= 0) 
                                        {
                                            printf("\n❌ Invalid input! Price must be greater than 0.\n");
                                            Sleep(2000);
                                        } else 
                                        {
                                            break;
                                        }
                                    }
                                } else if (menu_choice == 2) 
                                { // Description
                                    printf("\nEnter a brief description of the package: ");
                                    fgets(package_update_input[2], sizeof(package_update_input[2]), stdin);
                                    package_update_input[2][strcspn(package_update_input[2], "\n")] = 0;
                                } else if (menu_choice == 5) 
                                { // Max Guest Count
                                    while (1) 
                                    {
                                        printf("\nEnter the maximum number of guests allowed (e.g., 100): ");
                                        fgets(package_update_input[3], sizeof(package_update_input[3]), stdin);
                                        package_update_input[3][strcspn(package_update_input[3], "\n")] = 0;

                                        if (atoi(package_update_input[3]) <= 0) 
                                        {
                                            printf("\n❌ Invalid input! Guest count must be greater than 0.\n");
                                            Sleep(2000);
                                        } else 
                                        {
                                            break;
                                        }
                                    }
                                } else if (menu_choice == 6) 
                                { // Duration
                                    while (1) 
                                    {
                                        printf("\nEnter the duration of the package in hours (e.g., 5): ");
                                        fgets(package_update_input[4], sizeof(package_update_input[4]), stdin);
                                        package_update_input[4][strcspn(package_update_input[4], "\n")] = 0;

                                        if (atoi(package_update_input[4]) < 0) 
                                        {
                                            printf("\n❌ Invalid input! Duration must be 0 or greater.\n");
                                            Sleep(2000);
                                        } else 
                                        {
                                            break;
                                        }
                                    }
                                } else if (menu_choice == 3) 
                                { // Inclusions
                                    printf("\nEnter the inclusions for the package, separated by commas (e.g., Food,Decor,Music): ");
                                    fgets(package_update_input[5], sizeof(package_update_input[5]), stdin);
                                    package_update_input[5][strcspn(package_update_input[5], "\n")] = 0;
                                    
                                }
                                

                                if (menu_choice == 7) // Save Updates
                                {
                                    printf("\nSaving updates...\n");
                                    Sleep(2000);

                                    if (update_pkg(event_type_id, package_id, "package_name", package_update_input[0]) != 1 ||
                                        update_pkg(event_type_id, package_id, "description", package_update_input[1]) != 1 ||
                                        update_pkg(event_type_id, package_id, "inclusions", package_update_input[2]) != 1 ||
                                        update_pkg(event_type_id , package_id, "price", package_update_input[3]) != 1 ||
                                        update_pkg(event_type_id, package_id, "max_guest", package_update_input[4]) != 1)
                                    {
                                        printf("\n❌ Failed to update the package. Please try again later.\n");
                                        continue;
                                    }

                                    printf("\n✅ Package updated successfully!\n");
                                    Sleep(2000);
                                    break;
                                }
                                else if (menu_choice == 8) // Cancel Updates
                                {
                                    printf("\n🚫 Updates canceled. Returning to the previous menu.\n");
                                    memset(&package_update_input, 0, sizeof(package_update_input));
                                    break;
                                }
                            }

                            free(preview_package); // Free allocated memory
                        }

                                            }
                } else if(organizer_choice == 3) // delete package
                {
                    while (1) 
                    {
                        int event_type_id;
                        int package_id;
                        char *event_preview = preview_event_type();

                        const char *event_selection_menu[] = 
                        {
                            "==================================================",
                            "          🛠️ EventEase Dashboard > Delete Package",
                            "==================================================",
                            "📍 Home > Dashboard > Package Management > Delete Package",
                            "",
                            "🔧 Select the Event Type associated with the package.",
                            "",
                            "==================================================",
                            event_preview,
                            NULL
                        };
                        display_options(NULL, event_selection_menu, NULL, "Enter the Event Type ID (-1 to go back):");

                        if (scanf("%d", &event_type_id) != 1 || (valid_typeevent_id(event_type_id) != 1 && event_type_id != -1)) 
                        {
                            printf("\n❌ Invalid Event Type ID. Please select a valid option.\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        if (event_type_id == -1) 
                        {
                            printf("\n🚫 Package deletion process canceled. Returning to the previous menu.\n");
                            break;
                        }

                        while (1) 
                        {
                            char *package_preview = preview_pkgs(event_type_id);

                            const char *package_selection_menu[] = 
                            {
                                "==================================================",
                                "          🛠️ EventEase Dashboard > Delete Package",
                                "==================================================",
                                "📍 Home > Dashboard > Package Management > Delete Package",
                                "",
                                "🔧 Select the Package you wish to delete.",
                                "",
                                "==================================================",
                                package_preview,
                                NULL
                            };
                            display_options(NULL, package_selection_menu, NULL, "Enter the Package ID (-1 to go back):");

                            if (scanf("%d", &package_id) != 1 || (valid_pkg_id(event_type_id, package_id) != 1 && package_id != -1)) 
                            {
                                printf("\n❌ Invalid Package ID. Please select a valid option.\n");
                                Sleep(2000);
                                clear_input_buffer();
                                continue;
                            }
                            clear_input_buffer();

                            if (package_id == -1) 
                            {
                                printf("\n🚫 Package deletion process canceled. Returning to the previous menu.\n");
                                break;
                            }

                            while (1) 
                            {
                                char confirmation;

                                system("cls");
                                printf("==================================================\n");
                                printf("              ❌ Confirm Package Deletion         \n");
                                printf("==================================================\n");
                                printf("⚠️  Are you sure you want to delete the package?\n\n");
                                printf("   Event Type ID: %d\n", event_type_id);
                                printf("   Package ID   : %d\n", package_id);
                                printf("\n--------------------------------------------------\n");
                                printf("This action is irreversible.\n");
                                printf("==================================================\n");
                                printf("👉 Type 'Y' to confirm or 'N' to cancel: ");

                                scanf(" %c", &confirmation);
                                clear_input_buffer();

                                if (confirmation == 'Y' || confirmation == 'y') 
                                {
                                    if (delete_pkg(event_type_id, package_id) == 1) 
                                    {
                                        printf("\n✅ Package ID %d successfully deleted!\n", package_id);
                                        Sleep(2000);
                                    } 
                                    else 
                                    {
                                        printf("\n❌ Failed to delete Package ID %d. Please try again later.\n", package_id);
                                        Sleep(2000);
                                    }
                                    break;
                                } 
                                else if (confirmation == 'N' || confirmation == 'n') 
                                {
                                    printf("\n🚫 Package deletion canceled. Returning to the previous menu.\n");
                                    Sleep(2000);
                                    break;
                                } 
                                else 
                                {
                                    printf("\n❌ Invalid choice. Please select 'Y' or 'N'.\n");
                                    Sleep(2000);
                                    continue;
                                }
                            }
                        }
                    }

                } else if(organizer_choice == 4) // view packages
                {
                    while (1) 
                    {
                        int event_type_id;
                        int package_id;
                        char *event_preview = preview_event_type();

                        const char *event_selection_menu[] = 
                        {
                            "==================================================",
                            "          🛠️ EventEase Dashboard > Preview Package",
                            "==================================================",
                            "📍 Home > Dashboard > Package Management > Preview Package",
                            "",
                            "🔧 Select the Event Type associated with the package.",
                            "",
                            "==================================================",
                            event_preview,
                            NULL
                        };
                        display_options(NULL, event_selection_menu, NULL, "Enter the Event Type ID (-1 to go back):");

                        if (scanf("%d", &event_type_id) != 1 || (valid_typeevent_id(event_type_id) != 1 && event_type_id != -1)) 
                        {
                            printf("\n❌ Invalid Event Type ID. Please select a valid option.\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        if (event_type_id == -1) 
                        {
                            printf("\n🚫 Package deletion process canceled. Returning to the previous menu.\n");
                            break;
                        }

                        while (1) 
                        {
                            char *package_preview = preview_pkgs(event_type_id);

                            const char *package_selection_menu[] = 
                            {
                                "==================================================",
                                "          🛠️ EventEase Dashboard > Preview Package",
                                "==================================================",
                                "📍 Home > Dashboard > Package Management > Preview Package",
                                "",
                                "🔧 Select the Package you wish to preview.",
                                "",
                                "==================================================",
                                package_preview,
                                NULL
                            };
                            display_options(NULL, package_selection_menu, NULL, "Enter [ANY KEY] to stop viewing");

                            if(getch() == '\r')
                            {
                                break;
                            }
                        }
                    }
                } else if(organizer_choice == 5) // back to dashboard
                {
                    break;
                }

                
            }
        } else if(organizer_choice == 4) // maanage reminders
        {
            char *manage_reminders_menu[] = 
            {
                "==================================================",
                "                ⏰ Manage Reminders               ",
                "==================================================",
                "📍 Home > Dashboard > Manage Reminders",
                "",
                "🔧 What would you like to do?",
                "",
                "==================================================",
                NULL
            };

            char *manage_reminders_options[] = 
            {
                "View All Reminders         📅 - See a list of all scheduled reminders.",
                "Add New Reminder           ➕ - Schedule a new reminder for an event.",
                "Update Reminder            ✏️ - Modify details of an existing reminder.",
                "Delete Reminder            ❌ - Remove a reminder from the system.",
                "Back to Dashboard          🔙 - Return to the main dashboard.",
                NULL
            };

            while(1)
            {
                display_options(NULL, manage_reminders_menu, manage_reminders_options, NULL);
                
                if(scanf("%d", &organizer_choice) != 1 || organizer_choice < 1 || organizer_choice > 5)
                {
                    printf("\nPlease enter a valid choice\n");
                    Sleep(2000);
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();

                if(organizer_choice == 1) // view all reminders
                {
                    while(1)
                    {
                        int user_id;
                        system("cls");
                        reveal_all_users_except_organizers();
                        scanf("%d", &user_id);

                        if(user_id == -1)
                        {
                            break;
                        }

                        if(valid_user_id(user_id) != 1)
                        {
                            printf("\n Please enter a valid user id\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        while(1)
                        {
                            system("cls");
                            printf("%s\n", view_reminders(user_id));
                            printf("\nPlease enter [ANY KEY] to stop previwing");
                            if(getch() == '\r')
                            {
                                break;
                            }
                        }
                    }
                    // ask for which user ask for id. 

                }else if(organizer_choice == 2) // add new reminder
                {
                    char reminder_input[2][50] = {"", ""};
                    // ask whcich user first?
                    while(1)
                    {
                        int user_id;
                        system("cls");
                        reveal_all_users_except_organizers();
                        scanf("%d", &user_id);

                        if(user_id == -1)
                        {
                            break;
                        }

                        if(valid_user_id(user_id) != 1)
                        {
                            printf("\n Please enter a valid user id\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        while(1)
                        {
                            int reminder_choice;
                            const char *add_reminder_menu[] = {
                                "==================================================",
                                "                  ⏰ Add New Reminder             ",
                                "==================================================",
                                "",
                                "🔧 Fill in the required fields to create a new reminder.",
                                "",
                                "==================================================",
                                NULL
                            };

                            char *reminder_options[] = {
                                "Reminder Message\t\t\t", 
                                "Due Date (YYYY-MM-DD HH:MM:SS)\t",
                            };

                            char *options_reminder[4];
                            
                            for (int i = 0; i < 2; i++) {
                                    options_reminder[i] = malloc(100);
                                    if (options_reminder[i] != NULL) {
                                        snprintf(options_reminder[i], 100, "%s %s[%s]%s",
                                                reminder_options[i],
                                                strlen(reminder_input[i]) > 0 ? GREEN : RED,
                                                strlen(reminder_input[i]) > 0 ? "Filled" : "Empty", RESET);
                                    }
                            }
                            options_reminder[2] = strdup("Confirm");
                            options_reminder[3] = strdup("Cancel / Go Back");
                            options_reminder[4] = NULL;

                            display_options(NULL, add_reminder_menu, options_reminder, NULL);

                            if(scanf("%d", &reminder_choice) != 1 || reminder_choice < 1 || reminder_choice > 4)
                            {
                                printf("\nPLease eneter a valid choice\n");
                                Sleep(2000);
                                clear_input_buffer();
                                continue;
                            }
                            clear_input_buffer();
                            if(reminder_choice == 1) // reminder message
                            {
                                printf("\nEnter message for reminder: ");
                                fgets(reminder_input[0], sizeof(reminder_input[0]), stdin);
                                reminder_input[0][strcspn(reminder_input[0], "\n")] = 0;
                            } else if(reminder_choice == 2) // date
                            {
                                while(1)
                                {
                                    char date_str[50];
                                    char formatted_date[20];
                                    char formatted_time[10];
                                    system("cls");
                                    printf("\n==================================================\n");
                                    printf("               \U0001F4C5 Set Reminder Due Date\n");
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
                                        strcpy(reminder_input[1], formatted_date);

                                    } else 
                                    {
                                        Sleep(2000);
                                        continue;
                                    }
                                    break;
                                }
                            } else if(reminder_choice == 3) // confirmation.
                            {
                                if(reminder_input[0] != NULL && reminder_input[1] != NULL)
                                {
                                   while(1)
                                    {
                                        system("cls");
                                        printf("\n==================================================\n");
                                        printf("             📋 Review New Reminder            \n");
                                        printf("==================================================\n");
                                        printf("User ID               : %d\n", user_id);
                                        printf("Message               : %s\n", reminder_input[0]);
                                        printf("Due Date              : %s\n", reminder_input[1]);
                                        printf("==================================================\n");
                                        printf("\n❓ Confirm Changes (Y/N): ");

                                        char confirmation;
                                        scanf(" %c", &confirmation);
                                        clear_input_buffer();

                                        if (confirmation == 'Y' || confirmation == 'y') 
                                        {
                                            set_reminder(user_id, reminder_input[0], reminder_input[1]);
                                            memset(&reminder_input, 0, sizeof(reminder_input));
                                            printf("\nSuccessfully added reminder for user: %d\n");
                                            Sleep(3000);
                                            break;
                                            
                                        } else if(confirmation == 'N' || confirmation == 'n')
                                        {
                                            printf("\n🚫 Changes not saved. Returning to the previous menu.\n");
                                            Sleep(2000);
                                            break;
                                        } else 
                                        {
                                            printf("\nPlease enter valid option.\n");
                                            Sleep(2000);
                                        }

                                        // Save the updated package
                                    }
                                } else 
                                {
                                    printf("\nPlease make sure you have filled both fields.\n");
                                    continue;
                                }
                                // confirm deatils yadayada
                            } else if(reminder_choice == 4) // cancel
                            {
                                memset(&reminder_input, 0, sizeof(reminder_input));
                                break;
                            }
                        }
                    }

                    // 
                    // int user_id, const char *message, const char *due_date

                }else if(organizer_choice == 3) // update reminder status
                {
                    int user_id;
                    system("cls");
                    reveal_all_users_except_organizers();
                    scanf("%d", &user_id);

                    if(user_id == -1)
                    {
                        break;
                    }

                    if(valid_user_id(user_id) != 1)
                    {
                        printf("\n Please enter a valid user id\n");
                        Sleep(2000);
                        clear_input_buffer();
                        continue;
                    }
                    clear_input_buffer();

                    while(1)
                    {
                        int reminder_id;
                        char status[20];
                        system("cls");
                        char *prev_reminder = view_reminders(user_id);

                        if(prev_reminder == NULL)
                        {
                            Sleep(3000);
                            break;
                        }

                        printf("%s\nPlease enter id (-1 to go back): ", prev_reminder);
                        scanf("%d", &reminder_id);

                        if(reminder_id == -1)
                        {
                            break;
                        }

                        printf("\nPlease enter status for reminder id (PENDING or COMPLETE) ");

                        if(scanf("%s", status) != 1 || strcmp(status, "PENDING") != 0 && strcmp(status, "COMPLETE") != 0)
                        {
                            printf("\nPlease enter a valid option.\n");
                        }

                        if(update_reminder_status(user_id, reminder_id, strcmp(status, "PENDING") == 0 ? "PENDING" : "COMPLETE") == 1)
                        {
                            printf("\nReminder Updated Successfully!\n");
                            Sleep(2000);
                            break;
                        }
                        // either PENDING or COMPELTE
                    }
                }else if(organizer_choice == 4) // delete reminder
                {
                    int user_id;
                    system("cls");
                    reveal_all_users_except_organizers();
                    scanf("%d", &user_id);

                    if(user_id == -1)
                    {
                        break;
                    }

                    if(valid_user_id(user_id) != 1)
                    {
                        printf("\n Please enter a valid user id (-1 to cancel/go back)\n");
                        Sleep(2000);
                        clear_input_buffer();
                        continue;
                    }
                    clear_input_buffer();

                    while(1)
                    {
                        int reminder_id;
                        char status[20];
                        char *prev_reminder = view_reminders(user_id);
                        if(prev_reminder == NULL)
                        {
                            Sleep(3000);
                            break;
                        }
                        printf("\n%s\n", prev_reminder);
                        printf("\nPlease enter id (-1 to go back): ");
                        if(scanf("%d", &reminder_id) != 1 || valid_reminder_id(user_id, reminder_id) != 1 && reminder_id != -1)
                        {
                            printf("\nPlease enter a valid id\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        if(reminder_id == -1)
                        {
                            break;
                        }

                        char confirmation;

                        printf("Please confirm deletion of reminder ID : %d \nY or N: ", reminder_id);
                        scanf("%c", &confirmation);    
                        clear_input_buffer();

                        if(confirmation == 'Y' || confirmation == 'y')
                        {
                            delete_reminder(user_id, reminder_id);
                            Sleep(2000);
                            break;
                        } else if (confirmation == 'N' || confirmation == 'n') 
                        {
                            break;
                        } else 
                        {
                            printf("\nPlease enter a valid option\n");
                            continue;
                        }
                        // either PENDING or COMPELTE
                    }
                } else if(organizer_choice == 5) // back to dashbboard
                {
                    break;
                }
            }
        } else if(organizer_choice == 5) // manager users
        {
            char *manage_users_menu[] = 
            {
                "==================================================",
                "                 👥 Manage Users                  ",
                "==================================================",
                "Administer user accounts effectively:",
                "",
                NULL
            };

            char *manage_users_options[] = 
            {
                "Add User               ➕ - Create a new user account.",
                "Edit User              ✏️ - Modify existing user details.",
                "Deactivate User        🛑 - Disable user accounts temporarily.",
                "Manage Booked Event        - ",
                "View User Details      🔍 - Review specific user information.",
                "Back to Dashboard      🔙 - Return to the main dashboard.",
                NULL
            };
            
            while(1)
            {
                int option;
                display_options(NULL, manage_users_menu, manage_users_options, NULL);
                
                if(scanf("%d", &option) != 1 || option < 1 || option > 5)
                {
                    printf("\nPlease enter a valid choice\n");
                    Sleep(2000);
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();

                if(option == 1) // add user
                {
                    const char *fields[] = {"Username", "Password", "Full Name", "Phone Number", "Email", "Select Role"};
                    char input[6][MAX_INPUT_SIZE] = {"", "" , "","","",""}; // Array to store user inputs
                    User new_user = {0};

                    while (1) 
                    {
                        system("cls");

                        // Header
                        const char *paragraph[] = {
                            "==================================================",
                            "                 👤 Add New User                  ",
                            "==================================================",
                            "",
                            "🔧 Admin Panel - Add a New User Account",
                            "- Fill out all required fields to create a new user.",
                            "- Ensure all details are accurate before submission.",
                            "",
                            "==================================================",
                            NULL 
                        };

                        // Display Form
                        char *options[8];
                        char masked_password[50]; // For masking password input

                        if (strlen(input[1]) > 0) 
                        {
                            for (int i = 0; i < strlen(input[1]); i++) 
                            {
                                masked_password[i] = '*';
                            }
                            masked_password[strlen(input[1])] = '\0'; // Null-terminate
                        }

                        for (int i = 0; i < 6; i++) 
                        {
                            options[i] = malloc(100);
                            if(options[i] != NULL)
                                sprintf(options[i], "%s\t[%s%s%s] : %s", 
                                    fields[i], 
                                    strlen(input[i]) > 0 ? GREEN : RED, 
                                    strlen(input[i]) > 0 ? "Filled" : "Empty", 
                                    RESET, 
                                    i == 1 ? masked_password : input[i]);                            if(i == 5)
                            {
                                strcat(options[i] , "\n");
                            }
                        }
                        options[6] = "Submit";
                        options[7] = "Cancel";
                        options[8] = NULL;

                        display_options(NULL, paragraph, options, "Enter the corresponding number to update a field:");

                        int choice;
                        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 8) 
                        {
                            printf("\n❌ Invalid input. Please enter a valid option (1-8).\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        if (choice == 8) // Cancel
                        {
                            memset(&new_user, 0, sizeof(new_user));
                            memset(input, 0, sizeof(input));
                            printf("\n🚫 User addition canceled. All fields have been reset.\n");
                            Sleep(2000);
                            break;
                        }

                        if (choice == 1) // Username
                        {
                            printf("\nEnter Username: ");
                            fgets(input[0], MAX_INPUT_SIZE, stdin);
                            input[0][strcspn(input[0], "\n")] = 0; // Remove trailing newline

                            if (strlen(input[0]) == 0) 
                            {
                                printf("\n❌ Username cannot be empty. Please try again.\n");
                                Sleep(2000);
                                memset(input[0], 0, MAX_INPUT_SIZE);
                                continue;
                            } 
                            else if (username_exist(input[0])) 
                            {
                                printf("\n❌ Username already exists. Please try a different one.\n");
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
                                    printf("\n❌ Password must be at least 6 characters long, contain an uppercase letter, a digit, and a special character.\n");
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
                                    printf("\n❌ Passwords do not match. Please try again.\n");
                                    Sleep(2000);
                                    memset(input[1], 0, MAX_INPUT_SIZE);
                                } 
                                else 
                                {
                                    printf("\n✅ Password set successfully.\n");
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
                                    printf("\n❌ Full Name cannot be empty.\n");
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
                                    printf("\n❌ Invalid phone number. Must start with '09' and be 11 digits long.\n");
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
                                    printf("\n❌ Invalid email format. Please ensure it contains '@' and a valid domain.\n");
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
                                    printf("\n❌ Invalid role. Please choose 'client' or 'organizer'.\n");
                                    Sleep(2000);
                                    memset(input[5], 0, MAX_INPUT_SIZE);
                                    continue;
                                } 
                                else if (strcmp(input[5], "organizer") == 0) 
                                {
                                    char *roles_users = read_all_user_by_key("role");
                                    if (roles_users && strstr(roles_users, "organizer")) 
                                    {
                                        printf("\n❌ An organizer account already exists. Please choose a different role.\n");
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
                                printf("\n❌ Error: Please complete all required fields before submitting.\n");
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
                                printf("\n✅ User '%s' created successfully!\n", new_user.username);
                                Sleep(2000);
                                break;
                            } 
                            else 
                            {
                                printf("\n❌ Error: User creation failed. Please try again later.\n");
                                Sleep(2000);
                            }
                        }
                    }

                } else if (option == 2) // edit user
                {
                    int user_id;
                    while(1)
                    {
                        int user_id;
                        system("cls");
                        reveal_all_users_except_organizers();
                        scanf("%d", &user_id);

                        if(user_id == -1)
                        {
                            break;
                        }

                        if(valid_user_id(user_id) != 1)
                        {
                            printf("\n Please enter a valid user id\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        const char *fields[] = {"Username", "Password", "Full Name", "Phone Number", "Email", "Role"};
                        char input[6][MAX_INPUT_SIZE] = {""}; // Array to store user inputs
                        User current_user = {0}; // This should be filled with the user's current details before editing

                        while(1)
                        {                    
                            system("cls");
                            reveal_all_users_except_organizers();
                            scanf("%d", &current_user.id);

                            if(current_user.id == -1)
                            {
                                break;
                            }

                            if(valid_user_id(current_user.id) != 1)
                            {
                                printf("\n Please enter a valid user id (-1 to cancel/go back)\n");
                                Sleep(2000);
                                clear_input_buffer();
                                continue;
                            }
                            clear_input_buffer();

                            // Pre-fill input array with current user data
                            strcpy(input[0], current_user.username);
                            strcpy(input[1], current_user.password); // You may want to display masked password
                            strcpy(input[2], current_user.full_name);
                            strcpy(input[3], current_user.phone);
                            strcpy(input[4], current_user.email);
                            strcpy(input[5], current_user.role);

                            while (1) 
                            {
                                system("cls");

                                // Header
                                const char *paragraph[] = {
                                    "==================================================",
                                    "                   ✏️ Edit User                  ",
                                    "==================================================",
                                    "",
                                    "🔧 Admin Panel - Modify User Details",
                                    "- Update any of the following fields as needed.",
                                    "- Ensure all details are accurate before saving changes.",
                                    "",
                                    "==================================================",
                                    NULL 
                                };

                                // Display Form
                                char *options[8];
                                char masked_password[50]; // For masking password input
                                if (strlen(input[1]) > 0) 
                                {
                                    for (int i = 0; i < strlen(input[1]); i++) 
                                    {
                                        masked_password[i] = '*';
                                    }
                                    masked_password[strlen(input[1])] = '\0'; // Null-terminate
                                }

                                for (int i = 0; i < 6; i++) 
                                {
                                    options[i] = malloc(100);
                                    if (options[i] != NULL)
                                    {
                                        snprintf(options[i], 100, "%s: %s", fields[i], i == 1 ? masked_password : input[i]);
                                    }
                                }
                                options[6] = "Save Changes";
                                options[7] = "Cancel";
                                options[8] = NULL;

                                display_options(NULL, paragraph, options, "Enter the corresponding number to edit a field:");

                                int choice;
                                if (scanf("%d", &choice) != 1 || choice < 1 || choice > 8) 
                                {
                                    printf("\n❌ Invalid input. Please enter a valid option (1-8).\n");
                                    Sleep(2000);
                                    clear_input_buffer();
                                    continue;
                                }
                                clear_input_buffer();

                                if (choice == 8) // Cancel
                                {
                                    printf("\n🚫 Edit canceled. No changes were made.\n");
                                    memset(&options, 0, sizeof(options));
                                    Sleep(2000);
                                    break;
                                }

                                if (choice == 1) // Username
                                {
                                    printf("\nEnter New Username: ");
                                    fgets(input[0], MAX_INPUT_SIZE, stdin);
                                    input[0][strcspn(input[0], "\n")] = 0; // Remove trailing newline

                                    if (strlen(input[0]) == 0) 
                                    {
                                        printf("\n❌ Username cannot be empty. Please try again.\n");
                                        Sleep(2000);
                                        strcpy(input[0], current_user.username); // Reset to original value
                                        continue;
                                    } 
                                    else if (username_exist(input[0]) && strcmp(input[0], current_user.username) != 0) 
                                    {
                                        printf("\n❌ Username already exists. Please try a different one.\n");
                                        Sleep(2000);
                                        strcpy(input[0], current_user.username); // Reset to original value
                                        continue;
                                    }
                                }

                                else if (choice == 2) // Password
                                {
                                    while (1)
                                    {
                                        printf("\nEnter New Password: ");
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
                                            printf("\n❌ Password must be at least 6 characters long, contain an uppercase letter, a digit, and a special character.\n");
                                            Sleep(2000);
                                            continue;
                                        }

                                        // Confirm password
                                        char confirm_password[MAX_INPUT_SIZE];
                                        printf("Confirm Password: ");
                                        get_password(confirm_password, MAX_INPUT_SIZE);

                                        if (strcmp(input[1], confirm_password) != 0) 
                                        {
                                            printf("\n❌ Passwords do not match. Please try again.\n");
                                            Sleep(2000);
                                        } 
                                        else 
                                        {
                                            printf("\n✅ Password updated successfully.\n");
                                            break;
                                        }
                                    }
                                }

                                else if (choice == 3) // Full Name
                                {
                                    printf("\nEnter New Full Name: ");
                                    fgets(input[2], MAX_INPUT_SIZE, stdin);
                                    input[2][strcspn(input[2], "\n")] = 0;

                                    if (strlen(input[2]) == 0) 
                                    {
                                        printf("\n❌ Full Name cannot be empty.\n");
                                        Sleep(2000);
                                        strcpy(input[2], current_user.full_name); // Reset to original value
                                    }
                                }

                                else if (choice == 4) // Phone Number
                                {
                                    printf("\nEnter New Phone Number: ");
                                    fgets(input[3], MAX_INPUT_SIZE, stdin);
                                    input[3][strcspn(input[3], "\n")] = 0;

                                    if (strlen(input[3]) != 11 || strncmp(input[3], "09", 2) != 0 || !isdigit(input[3][2])) 
                                    {
                                        printf("\n❌ Invalid phone number. Must start with '09' and be 11 digits long.\n");
                                        Sleep(2000);
                                        strcpy(input[3], current_user.phone); // Reset to original value
                                    }
                                }

                                else if (choice == 5) // Email
                                {
                                    printf("\nEnter New Email: ");
                                    fgets(input[4], MAX_INPUT_SIZE, stdin);
                                    input[4][strcspn(input[4], "\n")] = 0;

                                    char *at_symbol = strchr(input[4], '@');
                                    char *dot_symbol = strrchr(input[4], '.');

                                    if (!at_symbol || !dot_symbol || at_symbol == input[4] || strlen(dot_symbol + 1) < 2 || dot_symbol < at_symbol + 2) 
                                    {
                                        printf("\n❌ Invalid email format. Please ensure it contains '@' and a valid domain.\n");
                                        Sleep(2000);
                                        strcpy(input[4], current_user.email); // Reset to original value
                                    }
                                }

                                else if (choice == 6) // Role
                                {
                                    printf("\nEnter New Role (client/organizer): ");
                                    fgets(input[5], MAX_INPUT_SIZE, stdin);
                                    input[5][strcspn(input[5], "\n")] = 0;

                                    if (strcmp(input[5], "client") != 0 && strcmp(input[5], "organizer") != 0) 
                                    {
                                        printf("\n❌ Invalid role. Please choose 'client' or 'organizer'.\n");
                                        Sleep(2000);
                                        strcpy(input[5], current_user.role); // Reset to original value
                                    }
                                }

                                else if (choice == 7) // Save Changes
                                {
                                    printf("\nSaving changes...\n");

                                    if (update_user(current_user.id, "username", input[0]) != 1 ||
                                        update_user(current_user.id, "password", input[1]) != 1 ||
                                        update_user(current_user.id, "full_name", input[2]) != 1 ||
                                        update_user(current_user.id, "phone", input[3]) != 1 ||
                                        update_user(current_user.id, "email", input[4]) != 1 ||
                                        update_user(current_user.id, "role", input[5]) != 1)
                                    {
                                        printf("\n❌ Failed to save changes. Please try again later.\n");
                                        Sleep(2000);
                                        continue;
                                    }

                                    printf("\n✅ User '%s' updated successfully!\n", input[0]);
                                    Sleep(2000);
                                    break;
                                }
                            }

                        }
                    }
                } else if (option == 3) // Book Event Manage Book Event
                {

                    while(1)
                    {
                        int user_id;
                        system("cls");
                        reveal_all_users_except_organizers();
                        scanf("%d", &user_id);

                        if(user_id == -1)
                        {
                            break;
                        }

                        if(valid_user_id(user_id) != 1)
                        {
                            printf("\n Please enter a valid user id\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        int book_event_id;

                        char *events = prev_events(user_id);
                        while (1)
                        {
                            system("cls");
                            printf("==================================================\n");
                            printf("                 📅 BookedEvents               \n");
                            printf("==================================================\n\n");
                            printf("It seems there are no events currently managed. 🛠️\n");
                            printf("Why not get started by creating or updating event types?\n");
                            printf("✨ Build a lineup of amazing events to offer clients.\n");
                            printf("Manage packages, bookings, and more to grow your event catalog! 🎉\n");
                            printf("\n==================================================\n\n");
                            printf("Press [ANY KEY] to return to Dashboard");

                            if (getch() == '\r')
                            {
                                organizer_menu(organizer_id);
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
                            events,
                            NULL // NULL-terminated to mark the end of the array
                        };
                        
                        display_options(NULL, booked_events_menu, NULL, "Enter event id to manage (-1 to cancel):");
                        scanf("%d", &book_event_id);

                        if(book_event_id == -1)
                        {
                            printf("Exiting Managing Booked Events...");
                            Sleep(2000);
                            break;
                        }
                        if(valid_event_id(user_id, book_event_id) != 1)
                        {
                            printf("\nPlease enter a valid choice/id\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }

                        clear_input_buffer();

                        // preview booked events of user 

                        while(1)
                        {
                            log_event(user_id, 0, "User selected to update an event.");
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
                                log_event(user_id, 1, "Invalid event update choice. Returning to events menu.");
                                printf("Invalid choice. Returning to events menu.\n");
                                Sleep(500);
                                continue;
                            }

                            if (field_choice == (int)key_count + 1) {
                                log_event(user_id, 1, "Event update canceled. Returning to events menu.");
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
                                log_event(user_id, 1, "Event update canceled by the user.");
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
                                    if(update_event(user_id, book_event_id, selected_key, new_value) == 1)
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

                            if (update_event(user_id, book_event_id, selected_key, new_value) == 1) 
                            {
                                log_event(user_id, 0, "Event updated successfully.");
                                printf("%s updated successfully!\n", selected_label);
                            } else {
                                log_event(user_id, 2, "Failed to update event.");
                                printf("Failed to update %s. Please try again.\n", selected_label);
                            }

                        }
                    }
                    // preview all users
                    // preview booked events of users
                        // if booked events == nul lthen return
                    // options are ->  Update Booked Event, Preview.
                    /* code */
                } else if (option == 4) // view all user details
                {
                     while(1)
                    {
                        int user_id;
                        system("cls");
                        reveal_all_users_except_organizers();
                        
                        if(getch() == '\r')
                        {
                            break;
                        }
                    }
                    /* code */
                } else if (option == 5) //back to dashboard
                {
                   break;
                }
                   
            }
        } else if(organizer_choice == 6) // notifcation and logs
        {
            while(1)
            {
                int menu_choice;
                const char *notifications_paragraph[] = {
                    "==================================================",
                    "              🔔 Admin Notification Panel         ",
                    "==================================================",
                    "📍 Home > Dashboard > Notifications",
                    "",
                    "Efficiently manage system notifications for users.",
                    "",
                    "Select an action from the options below:",
                    NULL
                };

                const char *notifications_options[] = {
                    "View Notifications     👀 - Review all recent alerts and updates.",
                    "Notify User            📣 - Send notifications directly to a user.",
                    "Back to Dashboard      🔙 - Return to the admin dashboard.",
                    NULL
                };
                display_options(NULL, notifications_paragraph, notifications_options , NULL);
                if(scanf("%d",&menu_choice) != 1 || menu_choice < 1 || menu_choice > 3)
                {
                    clear_input_buffer();
                    printf("\nPlease enter a valid option\n");
                    Sleep(2000);
                    continue;
                }
                // proceed patas logs?
                if(menu_choice == 1) 
                {
                    int user_id;
                    while(1)
                    {
                        int user_id;
                        system("cls");
                        reveal_all_users_except_organizers();
                        scanf("%d", &user_id);

                        if(user_id == -1)
                        {
                            break;
                        }

                        if(valid_user_id(user_id) != 1)
                        {
                            printf("\n Please enter a valid user id\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        while(1)
                        {
                            char *notifications = view_notifications(user_id); // Function to get notifications
                            if (!notifications) {
                                printf("🎉 No notifications to display. Enjoy your day!\n");
                                Sleep(2000);
                                break;
                            }

                            system("cls");
                            display_options(NULL, NULL, notifications, "📝 Actions:\n [1] Mark all as read    [2] Clear All   [3] Stop viewing");

                            char action = getch();

                            if(action == '1')
                            {
                                if (mark_all_notifications_read(user_id) == 1) {
                                    printf("\n✅ All notifications have been marked as read.\n");
                                } else {
                                    printf("\n❌ Failed to mark notifications as read. Please try again.\n");
                                }
                                Sleep(2000);
                            } else if(action == '2')
                            {
                                if (clear_notifications(user_id) == 1) {
                                    printf("\n🧹 All notifications have been cleared.\n");
                                } else {
                                    printf("\n❌ Failed to clear notifications. Please try again.\n");
                                }
                                Sleep(2000);
                            } else if(action == '3')
                            {
                                printf("\n🔙 Returning to the previous menu...\n");
                                Sleep(2000);
                                free(notifications);
                                break;
                            } else 
                            {
                                printf("\n❌ Invalid input. Please try again.\n");
                                Sleep(2000);
                            }
                            
                        }
                    }
                    // preview users 
                    // then view
                } else if(menu_choice == 2)
                {
                    int message[256];
                    while(1)
                    {
                        int user_id;
                        system("cls");
                        reveal_all_users_except_organizers();
                        scanf("%d", &user_id);

                        if(user_id == -1)
                        {
                            break;
                        }

                        if(valid_user_id(user_id) != 1)
                        {
                            printf("\n Please enter a valid user id\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();
                        
                        printf("\nPlease enter the message for the nofication (-1 to go back) ");
                        scanf("%s", message);
                    

                        if(strcmp(message, "-1") == 0)
                        {
                            printf("\nGoing Back...\n");
                            clear_input_buffer();
                            Sleep(2000);
                            continue;
                        }

                        
                        printf("\nAdded notifcation for User ID: %d\n", user_id);
                        printf("Return now ...");
                        add_notification(user_id, message, "Unread");
                        Sleep(4000);
                        break;
                    }
                    // preview user
                    // then notify 
                    // two param to add notifcastion for a certain user.


                } else if(menu_choice == 3) // update message or status>
                {
                    break;
                }
            }
        } else if(organizer_choice == 7) // 
        {
            while (1)
            {
                const char *feedback_support_menu[] = {
                    "==================================================",
                    "           📋 Organizer Feedback & Support        ",
                    "==================================================",
                    "",
                    "📍 Home > Dashboard > Feedback & Support",
                    "",
                    "🔧 Manage client feedback and support tickets.",
                    "   Select an option below to proceed:",
                    "",
                    "==================================================",
                    NULL
                };

                const char *feedback_support_options[] = {
                    "👀 View Feedback        - Review client feedback, ratings, and comments.",
                    "🎟️ Manage Tickets       - View and manage support tickets submitted by clients.",
                    "🔙 Back to Dashboard    - Return to the main dashboard.",
                    NULL
                };

                display_options(NULL, feedback_support_menu, feedback_support_options, "Enter your choice:");

                int organizer_choice;
                if (scanf("%d", &organizer_choice) != 1 || organizer_choice < 1 || organizer_choice > 3)
                {
                    printf("\n❌ Invalid input. Please select a valid option (1-3).\n");
                    Sleep(2000);
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer();

                if (organizer_choice == 1) // View Feedback
                {
                    while (1)
                    {
                        system("cls");
                        char *feedback_data = preview_feedback();
                        if (feedback_data == NULL)
                        {
                            printf("No feedback available at the moment.\n");
                        }
                        else
                        {
                            printf("%s\n", feedback_data);
                        }

                        printf("==================================================\n");
                        printf("Press [ANY KEY] to stop viewing.\n");

                        if (getch() == '\r')
                        {
                            break;
                        }
                    }
                }
                else if (organizer_choice == 2) // Manage Tickets
                {
                    while(1)
                    {
                        int user_id;
                        system("cls");
                        reveal_all_users_except_organizers();
                        scanf("%d", &user_id);

                        if(user_id == -1)
                        {
                            break;
                        }

                        if(valid_user_id(user_id) != 1)
                        {
                            printf("\n Please enter a valid user id\n");
                            Sleep(2000);
                            clear_input_buffer();
                            continue;
                        }
                        clear_input_buffer();

                        while(1)
                        {
                            const char *manage_tickets_menu[] = {
                                "==================================================",
                                "                🎟️ Manage Support Tickets         ",
                                "==================================================",
                                "📍 Home > Dashboard > Manage Tickets",
                                "",
                                "🔧 Review and respond to client support tickets.",
                                "",
                                "==================================================",
                                NULL
                            };

                            const char *manage_tickets_options[] = {
                                "👀 View All Tickets      - Display all submitted support tickets.",
                                "✅ Mark Ticket as Resolved - Mark a specific ticket as resolved.",
                                "❌ Delete a Ticket        - Remove a specific ticket.",
                                "🔙 Back to Feedback Menu  - Return to the feedback menu.",
                                NULL
                            };

                            display_options(NULL, manage_tickets_menu, manage_tickets_options, "Enter your choice:");

                            int ticket_choice;
                            if (scanf("%d", &ticket_choice) != 1 || ticket_choice < 1 || ticket_choice > 4)
                            {
                                printf("\n❌ Invalid input. Please select a valid option (1-4).\n");
                                Sleep(2000);
                                clear_input_buffer();
                                continue;
                            }
                            clear_input_buffer();

                            if (ticket_choice == 1) // View All Tickets
                            {
                                while(1)
                                {
                                    system("cls");
                                    printf("==================================================\n");
                                    printf("             👀 Viewing All Support Tickets       \n");
                                    printf("==================================================\n");

                                    char *tickets = view_tickets(user_id);
                                    if (tickets == NULL)
                                    {
                                        printf("No tickets available at the moment.\n");
                                        Sleep(3000);
                                    }
                                    else
                                    {
                                        printf("%s\n", tickets);
                                        free(tickets);
                                    }

                                    printf("==================================================\n");
                                    printf("Press [ANY KEY] to return.\n");
                                    if(getch() == '\r')
                                    {
                                        break;
                                    }
                                }
                            }
                            else if (ticket_choice == 2) // Mark Ticket as Resolved
                            {
                                int ticket_id;
                                printf("\nEnter the Ticket ID to mark as resolved: ");
                                if (scanf("%d", &ticket_id) != 1 || !valid_ticket_id(user_id,ticket_id))
                                {
                                    printf("\n❌ Invalid Ticket ID. Please try again.\n");
                                    clear_input_buffer();
                                    Sleep(2000);
                                    continue;
                                }
                                clear_input_buffer();

                                if (update_ticket(user_id, ticket_id, "RESOLVED") == 1)
                                {
                                    printf("\n✅ Ticket #%d has been marked as resolved.\n", ticket_id);
                                }
                                else
                                {
                                    printf("\n❌ Failed to update ticket status. Please try again.\n");
                                }
                                Sleep(2000);
                            }
                            else if (ticket_choice == 3) // Delete a Ticket
                            {
                                int ticket_id;
                                printf("\nEnter the Ticket ID to delete: ");
                                if (scanf("%d", &ticket_id) != 1 || !valid_ticket_id(user_id,ticket_id))
                                {
                                    printf("\n❌ Invalid Ticket ID. Please try again.\n");
                                    clear_input_buffer();
                                    Sleep(2000);
                                    continue;
                                }
                                clear_input_buffer();

                                if (delete_ticket(user_id, ticket_id) == 1)
                                {
                                    printf("\n✅ Ticket #%d has been deleted successfully.\n", ticket_id);
                                }
                                else
                                {
                                    printf("\n❌ Failed to delete the ticket. Please try again.\n");
                                }
                                Sleep(2000);
                            }
                            else if (ticket_choice == 4) // Back to Feedback Menu
                            {
                                break;
                            }
                        }
                    }
                }
                else if (organizer_choice == 3) // Back to Dashboard
                {
                    printf("\n🔙 Returning to the dashboard...\n");
                    Sleep(2000);
                    break;
                }
            }

        }
         else if(organizer_choice == 8
         )
        {
            while(1)
            {
                char confirmation;
                system("cls");
                printf("==================================================\n");
                printf("                 🔒 Logout Confirmation           \n");
                printf("==================================================\n");
                printf("Are you sure you want to log out?\n");
                printf("Your unsaved progress may be lost.\n");
                printf("--------------------------------------------------\n");
                printf("   Press 'Y' to log out or 'N' to stay logged in. \n");
                printf("==================================================\n");
                printf("👉 Enter your choice: ");
                
                scanf("%c", &confirmation);

                if(confirmation == 'Y' || confirmation == 'y')
                {
                    update_last_login("stay_logged_in", "0");
                    update_user(organizer_id, "stay_logged_in", "0");
                    auth_menu();
                } else if(confirmation == 'N' || confirmation == 'n')
                {
                    break;
                } else 
                {
                    printf("\nPlease pick a valid choice\n");
                    break;
                }
            }
        }
    }
} 


// int organizer_menu(int organizer_id)
// {
//     int organizer_choice;

//     while(1)
//     {
//         system("cls");
//         int event_choice;
//         printf("Enter your choice: ");
//         scanf("%d", &organizer_choice);

//         TypeEvent type_event =
//         {
//             .name = "",
//             .description = "", // \0
//             .venues = "" // delimiter kay 'comma'
//         };

//         int valid_event_name;

//         int data_filled;

//         switch(organizer_choice)
//         {
//              case 1:
//                 // Event Management
//                 // prompt user for event_choice
//                 // create new eventtype

//                 while(1)
//                 {
//                     system("cls");
//                     printf("-- Create New Event Type Menu --\n");
//                     printf("Please enter the remaining information\n\n\n");

//                     char type_member[4][100] = {"Name", "Description", "Venues", "Type of Event"};

//                     for (int i = 0; i < 4; i++)  
//                     {
//                         char *field = NULL;
//                         switch (i) 
//                         {
//                             case 0: field = type_event.name; break;
//                             case 1: field = type_event.description; break;
//                             case 2: field = type_event.venues; break;
//                             case 3: field = type_event.type; break;
//                         }

                        
//                         if (field && strlen(field) > 0) 
//                         {
//                             printf("%d. %s %s(/)%s\n", i + 1, type_member[i], GREEN, RESET);
//                         } else 
//                         {
//                             printf("%d. %s %s(x)%s\n", i + 1, type_member[i], RED, RESET);
//                         }
//                     }
//                     printf("5. Confirm Creation\n");
//                     printf("6. Cancel Creation.\n");
//                     printf("\nEnter Choice: ");
//                     scanf("%d", &event_choice);
//                     getchar();

//                     if(event_choice == 1)
//                     {
                        
//                             // event name
//                         if(event_choice == 1)
//                         {
//                             // validate kung unsay kalyangan
//                             // i.e; dapat ang gi input kay 4-12 chars.
//                             while(!valid_event_name)
//                             {
//                                 printf("\nEnter Event Type Name: ");
//                                 fgets(type_event.name, sizeof(type_event.name), stdin);
//                                 type_event.name[strcspn(type_event.name, "\n")] = 0; // para mawala ang nextline


//                                 // check check kung si name blahb lah blah
//                                 size_t name_size = strlen(type_event.name);

//                                 if(name_size < 4 || name_size > 12)
//                                 {
//                                     printf("Invalid length! Event Name Must be 4-12 Characters!\n");
//                                 } else
//                                 {
//                                     data_filled++; // + 1
//                                     valid_event_name = 1;
//                                 }
//                             }
//                         }
                        
//                     }  else if(event_choice == 5) // confirm.
//                     {
//                         if(data_filled == 4)
//                         {

//                         } else
//                         {
//                             printf("Please fill in the fields first.");
//                             Sleep(1000);
//                             continue;
//                         }
//                     } else if(event_choice == 6) // cancel nga part.
//                     {
//                         memset(&type_event, 0, sizeof(type_event)); 
//                     }
//                 }
//                 break;
//             case 2:
                
//                 break;
//             case 3:
                
//                 break;
//             case 4:
                
//                 break;
//             case 5:
                
//                 break;
//             case 6:

//                 break;
//             case 7:
            
//                 break;
//             case 8:

//                 break;
            
//             default:
//                 printf("\nInvalid choice. Please enter a number between 1 and 8.\n");
//                 Sleep(1000);
//                 break;
//         }
//     }
// } 