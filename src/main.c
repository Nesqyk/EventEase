#include <stdio.h>
#include <string.h>


// #include "include/data_handler.h"

void display_menu();

int main()
{
    display_menu();
}



/*
1. Dashboard
2. Create Event
3. My Events
4. Reports
5. Reviews
6. Exit
*/
int event_ids[10];

int is_valid_choice(int n);

// Displays the main menu of the program.
void display_menu()
{
    char auth_file_name[30];
    sprintf(auth_file_name, "%sauth.txt", DATA_DIR)
    FILE *auth_file = fopen("")
    // auth first 
    // check if username: and password is empty;
    
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