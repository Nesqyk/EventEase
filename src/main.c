#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    client_menu(75);
}