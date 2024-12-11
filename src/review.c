/*
5. Check Reviews

    Check Reviews Menu
    1. Search Event by ID

        >Available Events: ...

        >Event Found: ...
            1. View Reviews
            2. Add Review
            3. Delete Review
            4. Back to Review Menu

    2. Back to Main Menu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "data_handler.h" // wala ko kasabot unsa gamit ani huhu
#include "events.h"

// function declarations
void reviews_menu();
int search_event_by_id(int event_id);
void add_review(int event_id);
void delete_review(int event_id);
void display_reviews(int event_id);
void display_event_list();



// menu for reviews
void init_menu()
{
    int event_id;

    do {
        system("cls");
        printf("--- Check Reviews Menu ---\n");
        printf("1. Search Event by ID\n");
        printf("2. Back to Main Menu\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
            // choices ni user
            case 1:
                printf("\nAvailable Events:\n");
                display_event_list();

                printf("\nEnter Event ID: ");
                scanf("%d", &event_id);

                if (search_event_by_id(event_id))
                    {
                    int action_choice;
                    do {
                        system("cls");
                        printf("--- Event Found: %s ---\n", event_names[event_id - 1]);
                        printf("1. View Reviews\n");
                        printf("2. Add Review\n");
                        printf("3. Delete Review\n");
                        printf("4. Back to Reviews Menu\n");
                        printf("Enter your choice: ");
                        scanf("%d", &action_choice);

                        switch (action_choice)
                        {
                            case 1:
                                display_reviews(event_id);
                                break;
                            case 2:
                                add_review(event_id);
                                break;
                            case 3:
                                delete_review(event_id);
                                break;
                            case 4:
                                goto back_to_reviews_menu;
                            default:
                                printf("Invalid choice. Try again.\n");
                        }
                    } while (1);
                } else {
                    printf("Event not found. Please try again.\n");
                    printf("Press Enter to continue...");
                    getchar();
                    getchar();
                }
                break;
            case 2:
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    back_to_reviews_menu:; // label para ma balik sa reviews menu
    } while (1);
}

int search_event_by_id(int event_id)
{
    for (int i = 0; i < event_count; i++)
    {
        if (event_ids[i] == event_id)
        {
            return 1;
        }
    }
    return 0;
}

void display_event_list()
{
    for (int i = 0; i < event_count; i++)
    {
        printf("ID: %d - %s\n", event_ids[i], event_names[i]);
    }
}

void display_reviews(int event_id)
{
    printf("\n\n--- Reviews for Event ID %d ---\n", event_id);
    int found = 0;

    for (int i = 0; i < review_count; i++)
    {
        if (review_event_ids[i] == event_id)
        {
            printf("\nReview #%d:\n", i + 1);
            printf("Review: %s\n", reviews[i]);
            printf("Date & Time: %s\n", review_timestamps[i]);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No reviews found for this event.\n");
    }

    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

void add_review(int event_id)
{
    if (review_count >= MAX_REVIEWS)
    {
        printf("Cannot add more reviews. Storage is full.\n");
        return;
    }

    char review[200];
    printf("Enter your review: ");
    getchar();
    fgets(review, sizeof(review), stdin);
    review[strcspn(review, "\n")] = '\0';

    // get current date and time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[100];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    // add review to database
    review_event_ids[review_count] = event_id;
    strcpy(reviews[review_count], review);
    strcpy(review_timestamps[review_count], timestamp);
    review_count++;

    printf("Review added successfully for Event ID %d!\n\n", event_id);
    printf("Press Enter to continue...");
    getchar();
}

void delete_review(int event_id)
{
    printf("\n\n--- Delete Reviews for Event ID %d ---\n", event_id);
    int found = 0;

    for (int i = 0; i < review_count; i++)
    {
        if (review_event_ids[i] == event_id)
        {
            printf("%d. %s\n", i + 1, reviews[i]);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No reviews found for this event.\n\n");
        printf("Press Enter to continue...");
        getchar();
        getchar();
        return;
    }

    printf("Enter the review number to delete: ");
    int review_num;
    scanf("%d", &review_num);

    if (review_num < 1 || review_num > review_count || review_event_ids[review_num - 1] != event_id) {
        printf("Invalid review number.\n");
    } else {
        for (int i = review_num - 1; i < review_count - 1; i++) {
            review_event_ids[i] = review_event_ids[i + 1];
            strcpy(reviews[i], reviews[i + 1]);
            strcpy(review_timestamps[i], review_timestamps[i + 1]);
        }
        review_count--;
        printf("Review deleted successfully.\n");
    }

    printf("Press Enter to continue...");
    getchar();
    getchar();
}

*/