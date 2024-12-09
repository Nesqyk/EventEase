/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_handler.h"
#include "events.h"

#define MAX_EVENTS 100
#define MAX_LENGTH 100

// Global variable para ni sa na search na ID sa option 1
int lastSearchedId = -1;

int generate_cost(int id);
// void generate_cost(int ids[], char names[][MAX_LENGTH], double costs[], int count);
void calculate_profit(int ids[], char names[][MAX_LENGTH], double costs[], double balance[], int count);
void generate_receipt(int ids[], char names[][MAX_LENGTH], double costs[], double balance[], int count);
//  -> search id first 
// choices
// cost, profit, receipt (if possible)
// cost args -
// cost - gibayad sa client.

void inititalize_reports()
{
    int option; 

    do
    {
        //Menu
        printf("\nGenerate Reports\n");
        printf("1. Search Event by ID\n");
        printf("2. Cost Breakdown\n");
        printf("3. Profit Calculation\n");
        printf("4. Receipt (Printable Format)\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option)
        {
            // options ni user
            case 1:
                list_events();
                break;
            case 2:
                list_events();
                int id;

                printf("Input ID: ");
                scanf("%d", &id);

                if(!s_valid_id(id))
                {
                    printf("Invalid ID please try again.");
                    continue;
                }
                int cost = generate_cost(id);
                printf("Generated Cost: %d", cost);
                break;
            case 3:
                calculate_profit(ids, names, costs, balance, eventCount);
                break;
            case 4:
                generate_receipt(ids, names, costs, balance, eventCount);
                break;
            case 5:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (option != 5);

    return 0;
}

//  -> search id first
//Himo sa cost
int generate_cost(int id)
{
        if (lastSearchedId == -1)
    {
        printf("No event ID has been searched. Please search the event ID.\n");
        return;
    }

    //ambot if mag fprintf ba sd ko diri
    for (int i = 0; i < count; i++)
    {
        if (ids[i] == lastSearchedId)
        {
            printf("Cost Breakdown for Event %s (ID: %d):\n", names[i], ids[i]);
            printf("Total Cost: %.2lf\n", costs[i]);
            return;
        }
    }
    printf("Event with ID %d not found.\n", lastSearchedId);

   // check sa if si ID kay naa.
    if(!is_valid_id(id))
    {
        printf("invalid id. ");
    }

    // atoi will convert your char * into int.
    int cost = atoi(read_event(id, "cost"));
    return cost;
}

//Calculate sa profit
void calculate_profit(int id)
{
    if (lastSearchedId == -1)
    {
        printf("No event ID has been searched. Please search the event ID.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (ids[i] == lastSearchedId)
        {
            double profit = balance[i] - costs[i]; //calculate ang profit
            printf("Profit for Event %s (ID: %d): %.2lf\n", names[i], ids[i], profit);
            return;
        }
    }
    printf("Event with ID %d not found.\n", lastSearchedId);
}

//Himog Receipt
void generate_receipt(int ids[], char names[][MAX_LENGTH], double costs[], double balance[], int count)
{
    if (lastSearchedId == -1)
    {
        printf("No event ID has been searched. Please search the event ID.\n");
        return;
    }
    //ambot if mag fprintf ba sd ko diri
    for (int i = 0; i < count; i++)
    {
        if (ids[i] == lastSearchedId)
        {
            //Print tanan naa sa events apil na ang cost og profit
            printf("Receipt for The Event");
            printf("Event ID: %d\n", ids[i]);
            printf("Event Name: %s\n", names[i]);
            printf("Cost: %.2lf\n", costs[i]);
            printf("Revenue: %.2lf\n", balance[i]);
            printf("Profit: %.2lf\n", balance[i] - costs[i]);
            return;
        }
    }
    printf("Event with ID %d not found.\n", lastSearchedId);
}
*/