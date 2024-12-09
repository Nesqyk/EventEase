#include <stdio.h>
#include <stdlib.h>

#include "events.h"


void print_event_values(int id)
{    
    char *cost_str = read_event(id, "cost");
    char *balance_str = read_event(id, "balance");
    char *no_attendee_str = read_event(id, "no_attendee");

    // Convert strings to numbers
    float cost = (cost_str != NULL) ? atof(cost_str) : 0.0;
    float balance = (balance_str != NULL) ? atof(balance_str) : 0.0;
    int no_attendee = (no_attendee_str != NULL) ? atoi(no_attendee_str) : 0;

    // Print the values
    printf("Client Name: %s\n", read_event(id, "client_name"));
    printf("Cost: %.2f\n", cost);
    printf("Balance: %.2f\n", balance);
    printf("No Attendee: %d\n", no_attendee);
    printf("Venue: %s\n", read_event(id, "venue"));
    printf("Completion Date: %s\n", read_event(id, "completion_date"));
}
