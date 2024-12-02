//Di pa sure kung mogana ni especially kay dili pa complete ang details

/*
3. ####  My Events
    - Features:
        - Search Event:
            - By Event ID or Client Name.
    - View Event:
        - Display overview including: ID, Name, Date, Venue, Status.
    - Add status legend: Upcoming, Tomorrow, Canceled.
    - Edit Event:
        - Enter Event ID.
    - Editable Fields:
        - Client Name
        - Cost
        - Venue
        - Number of Attendees
        - Classification.
    - Navigation: Back to Main Menu.
*/
#include <stdio.h>
#include <string.h>

// Function to search for an event by ID
void searchById(int event_id[], char client_name[][100], long cost[], int no_attendee[], char venue[][100], int size, int searchId) {
    int found = 0;

    // Loop through the IDs to find the matching one
    for (int i = 0; i < size; i++) {
        if (event_id[i] == searchId) {
            printf("ID: %d\n", event_id[i]);
            printf("Name: %s\n", client_name[i]);
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

int main() {
    // Arrays for event data
    int event_id[] = {101, 102, 103, 104}; // Placeholder IDs
    char client_name[][100] = {"Alice", "Bob", "Charlie", "David"}; // Placeholder names
    long cost[] = {90000, 80000, 70000, 85000}; // Placeholder costs
    int no_attendee[] = {45, 60, 50, 55}; // Placeholder number of attendees
    char venue[][100] = {"Sea", "Cottage", "Boracay", "Cebu"}; // Placeholder venues

    int size = sizeof(event_id) / sizeof(event_id[0]);

    int searchId;
    printf("Enter ID to search: ");
    scanf("%d", &searchId);

    searchById(event_id, client_name, cost, no_attendee, venue, size, searchId);

    return 0;
}

