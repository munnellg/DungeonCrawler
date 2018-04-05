#ifndef MAINMENU_H
#define MAINMENU_H

#include "controller.h"

int  main_menu_init           ( void );
void main_menu_register_state ( struct state *state );
void main_menu_quit           ( void );

#endif