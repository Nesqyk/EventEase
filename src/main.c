#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

#include "menu.h"
#include "organizer.h"



int main()
{
    /* TypeEvent type_event = {
        .id = generate_typeevent_id(),
        .description = "This is a test event",
        .type = "Birthday",
        .venues = "School, Jabee, Mcdo",
        .name = "BDAAAYY"
    };

    create_typeevent(type_event);

    printf("%s\n", read_eventtype_all("description"));
    printf("%s", read_typevent(1, "event_id")); */
    
    // Package pkg = {
    //     .availability =  1,
    //     .created_date = "nah",
    //     .description = "Test Package",
    //     .event_type = 1,
    //     .id = generate_pkg_id(1),
    //     .inclusions = "Test inclusions",
    //     .max_guest = 100,
    //     .package_name = "A",
    //     .price = 10000
    // };
    // add_pkg(1, pkg);
    
    // char * pkg_prev = preview_pkgs(1);

    // if(pkg_prev != NULL)
    // {
    //     printf("%s", pkg_prev);
    // }
    // printf("%s", preview_pkgs(1));

    SetConsoleOutputCP(CP_UTF8); // for emojis. 
    // add_notification(3156, "Last christmas", "Unread");
        // char *prev_reminders = view_reminders(2968);
        // printf("%s\n", prev_reminders);
    auth_menu();

    add_notification(2968, "Test Notifcation", "Unread");

    // char *venues = read_typevent(1, "venues");
    // printf("%s", venues);
    // printf("%d", valid_event_id(3156, 209));
    // int id = reveal_organizer_id();
    // printf("%d", id);
    // printf("%d", login_user("2768", "Hello@123"));

    // printf("%s\n", preview_events(3156));

    // int client_id = 3156;  // Example client ID
    // int event_count = 0;
    // int book_id = 0;

    // char **events = preview_events(client_id, &event_count, &book_id);

    // if (events == NULL || event_count == 0) {
    //     printf("No events found for client ID %d.\n", client_id);
    // } else {
    //     printf("Found %d events:\n", event_count);
    //     for (int i = 0; i < event_count; i++) {
    //         printf("%s\n", events[i]);
    //         free(events[i]);  // Free each event string
    //     }
    //     free(events);  // Free the array of pointers
    // }

}
