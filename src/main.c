#include <stdio.h>

#include "data_handler.h"
#include "dashboard.h"

int main()
{
    
}


/*
1. Dashboard
2. Create Event
3. My Events
4. Reports
5. Reviews
6. Exit
*/
const int list_choices[] = {1, 2, 3, 4, 5, 6};


int is_valid_choice(int n);

// Displays the main menu of the program.
void display_menu()
{
    int user_choice;
    char *menu_options[] = {"Dashboard", "Create Event", "My Events","Reports","Reviews", "Exit"};
    
    do {
        printf("Select an Option\n");

        for(int i = 0; i < sizeof(menu_options) / sizeof(*menu_options); i++)
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

int auth(char username[], int password)
{
    // check file
    // close file
    // return 1 for auth
    // return 0 for failed

}

char get_format()
{

}