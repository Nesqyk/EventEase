//Di pa sure kung mogana ni especially kay dili pa complete ang details


#include <stdio.h>
#include <string.h>

void searchById(int event_id[], char client_name[][100], char event_name[][100], long cost[], int no_attendee[], char venue[][100], int size, int searchId);
void viewEvent(char client_name[][100], char event_name[][100], int size);

int main()
{
    // Arrays for event data
    int event_id[] = {101, 102, 103, 104}; // Placeholder IDs
    char client_name[][100] = {"Alice", "Bob", "Charlie", "David"}; // Placeholder names
    char event_name[][100] = {"Funeral", "Wedding", "Birthday", "Birthday"}; //Placeholder event
    long cost[] = {90000, 80000, 70000, 85000}; // Placeholder costs
    int no_attendee[] = {45, 60, 50, 55}; // Placeholder number of attendees
    char venue[][100] = {"Sea", "Cottage", "Boracay", "Cebu"}; // Placeholder venues
    int choice;

    int size = sizeof(event_id) / sizeof(event_id[0]);
    int searchId;

    printf("===My Events Menu===\n");
    printf("1. Search Event Id\n");
    printf("2. View Events\n");
    printf("3. Edit Event\n");
    printf("4. Back to Main Menu\n");

    printf("\nEnter Choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
    case 1:
    printf("\nEnter ID to search: ");
    scanf("%d", &searchId);

    searchById(event_id, client_name, event_name, cost, no_attendee, venue, size, searchId);
    case 2:
        viewEvent(client_name, event_name, size);
    }

    return 0;
}


// Function to search for an event by ID
void searchById(int event_id[], char client_name[][100], char event_name[][100], long cost[], int no_attendee[], char venue[][100], int size, int searchId)
{
    int found = 0;

    // Loop through the IDs to find the matching one
    for (int i = 0; i < size; i++) {
        if (event_id[i] == searchId) {
            printf("ID: %d\n", event_id[i]);
            printf("Name: %s\n", client_name[i]);
            printf("Event: %s\n", event_name[i]);
            printf("Cost: %ld\n", cost[i]);
            printf("No. of Attendees: %d\n", no_attendee[i]);
            printf("Venue: %s\n", venue[i]);
            found = 1;
            break;
        }
    }

    // If no record is found
    if (!found) {
        printf("Record with ID %d not found.\n", searchId);
    }
}
void viewEvent(char client_name[][100], char event_name[][100], int size)// Still incomplete details, most likely placeholder details
{
    printf("\nEVENTS\t\tNAME\n");

    for(int i=0; i<size; i++)
    {
        printf("\n%s\t\t%s", event_name[i], client_name[i]);// Something is wrong sa pagline up sa second and third, still trying to figure it out
    }

}

