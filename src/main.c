#include <stdio.h>


int main()
{
    // check sa if si data.txt kung exist ba sya

    char data[] = {"reports.txt", "events.txt"};
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


// initiates the main menu of the program
void init_menu()
{
    int choice;
    char choices_in_string[] = {"Dasboard", "Create Event", "My Events","Reports","Reviews", "Exit"};
    
    do {
        if(is_valid_choice(choice) != 1)
        {
            continue;
        }

        scanf("%d", &choice);

        switch(choice)
        {
            // TODO: i sud mga initializer diri for each event in accord sa ilahang assigned value.
        }
    } while(1);
}


int is_valid_choice(int n)
{
    if (n < 1 && n > 6)
    {
        return 1;
    }
    return 0;
}