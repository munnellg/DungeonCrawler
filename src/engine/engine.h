#ifndef ENGINE_H
#define ENGINE_H

#include "controller.h"

int  engine_init           ( void );
void engine_register_state ( struct state *state );
void engine_quit           ( void );

#endif