#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_handler.h"
#include "events.h"

/*
Still need to implement stuffs.
*/

int generate_cost(int id);
int calculate_profit(int id);
void generate_receipt(int id);

int init_reports()
{
    int option;
    int id; 

    do
    {
        printf("\nGenerate Reports\n");
        printf("1. Search Event by ID\n");
        printf("2. Cost Breakdown\n");
        printf("3. Profit Calculation\n");
        printf("4. Receipt \n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option)
        {
            case 1:
                preview_events(); 
                continue;

            case 2:
                preview_events(); 
                printf("\nInput ID: ");
                scanf("%d", &id);

                
                printf("Invalid ID. Please try again.\n");
                while(is_valid_id(id) != 1)
                {
                    printf("Please Enter a valid ID");
                    while(getchar() != '\n');                 
                }

                int cost = generate_cost(id);
                printf("Generated Cost: %d\n", cost);
                continue;

            case 3:
                preview_events(); 
                printf("\nInput ID: ");
                scanf("%d", &id);

                printf("Invalid ID. Please try again.\n");
                while(is_valid_id(id) != 1)
                {
                    printf("Please Enter a valid ID");
                    while(getchar() != '\n');    
                }

                int profit = calculate_profit(id);
                printf("Generated Profit: %d", profit);
                continue;

            case 4:
                preview_events(); 
                printf("Input ID: ");
                scanf("%d", &id);

                printf("Invalid ID. Please try again.\n");
                while(is_valid_id(id) != 1)
                {
                    printf("Please Enter a valid ID");
                    while(getchar() != '\n');
                }
                generate_receipt(id);
                continue;

            case 5:
                printf("Exiting Reports...\n");
                return -1;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (option != 5);
    return 0;
}

// Generate cost for the event
int generate_cost(int id)
{
    // goods ni
    if (!is_valid_id(id))
    {
        printf("Invalid ID.\n");
        return -1;
    }

    int cost = atoi(read_event(id, "cost"));
    
    return cost;
}

// Calculate profit for the event
int calculate_profit(int id)
{
    if (!is_valid_id(id))
    {
        printf("Invalid ID.\n");
        return -1;
    }

    int cost = atoi(read_event(id, "cost"));
    int balance = atoi(read_event(id, "balance"));
    
    return cost - balance;
    
}

// Generate receipt for the event
void generate_receipt(int id)
{
    if (!is_valid_id(id))
    {
        printf("Invalid ID.\n");
        return;
    }

    int cost = atoi(read_event(id, "cost"));
    int balance = atoi(read_event(id, "balance"));
    int profit = calculate_profit(id);
    
    printf("\nReceipt for Event %d\n", id);
    printf("ID: %d\n", id);
    printf("Client Name: %s\n", read_event(id, "client_name"));
    printf("Cost: %d\n", cost);
    printf("Balance: %d\n", balance);
    printf("Profit: %d\n", profit);
    printf("Date Created: %s\n", read_event(id, "date_created"));
    return;
}