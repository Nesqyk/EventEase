#include <stdio.h>

#include <data_handler.h>

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


int isValidChoice(int n);

// Displays the main menu of the program.
void DisplayMenu()
{
    int user_choice;
    char menu_options[][20] = {"Dashboard", "Create Event", "My Events","Reports","Reviews", "Exit"};
    
    do {
        if(isValidChoice(user_choice) != 1)
        {
            continue;
        }

        printf("Select an Option\n");

        for(int i = 0; i < sizeof(menu_options) / sizeof(menu_options[0]); i++)
        {
            printf("%d. %s\n", i + 1, menu_options[i]);
        }

        scanf("%d", &user_choice);

        switch(user_choice)
        {
            // TODO: Ari initialize sa tanan :>
            // TODO: Initialize events according to their assigned value.
        }
    } while(1);
}


int isValidChoice(int n)
{
    if (n >= 1 && n <= 6)
    {
        return 1;
    }
    return 0;
}