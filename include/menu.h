#ifndef MENU_H
#define MENU_H

#include "utils.h"
#include "users.h"
#include "events.h"

int auth_menu();

typedef struct {
    const char *key;
    const char *display_name;
} UserField;

int client_menu(int client_menu);

#endif //MENU.H