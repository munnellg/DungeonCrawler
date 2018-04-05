#include "controller.h"

#include "utils.h"
#include "mainmenu.h"
#include "engine.h"
#include <time.h>
#include <stdlib.h>

#define MAX_EVENTS 16 // shouldn't come anywhere close to this

typedef enum {
	STATE_MAIN_MENU,
	STATE_IN_GAME,
	STATE_COUNT
} state_code;

struct state states[STATE_COUNT];

int events[MAX_EVENTS];
int nevents = 0;

int curstate = STATE_MAIN_MENU;
int status   = EXIT_SUCCESS;
int quit     = 0;

static int
initialize( void ) {
	// initialize random number generator
	srand(time(NULL));

	if( !main_menu_init() ) {
		ERROR("Failed to initialize main menu");
		return 0;
	}

	if( !engine_init() ) {
		ERROR("Failed to initialize game engine");
		return 0;
	}

	main_menu_register_state( &states[STATE_MAIN_MENU] );
	engine_register_state( &states[STATE_IN_GAME] );

	return 1;
}

static void
switch_to_state ( state_code state ) {
	if ( state >= STATE_COUNT ) {
		ERROR("Invalid state %d", state);
		return;
	}
	
	if( states[curstate].exit ) { states[curstate].exit(); }	

	curstate = state;
	if( !states[curstate].enter ) { return; }

	if ( !states[curstate].enter() ) {		
		status = EXIT_FAILURE;
		quit = 1;
	}
}

static void
handle_events ( void ) {
	for ( int i=0; i<nevents; i++ ) {
		switch(events[i]) {
			case EVENT_START_NEW_GAME:
				switch_to_state(STATE_IN_GAME);
				break;
			case EVENT_QUIT_GAME:
				quit = 1;
				break;
			case EVENT_GAME_OVER:
			case EVENT_SHOW_MAIN_MENU:
				switch_to_state(STATE_MAIN_MENU);
				break;
			default:
				ERROR("Invalid event code 0x%X", events[i]);
		}
	}

	nevents = 0;
}

static void
terminate ( void ) {
	main_menu_quit();
}

int
launch_program ( void ) {	
	char buf[LONG_STRING];
	
	if (!initialize()) {
		status = EXIT_FAILURE;
		quit = 1;
	}

	while (!quit) {
		if( states[curstate].render ) states[curstate].render( );

		if( !utils_prompt_user( buf, LONG_STRING, NULL ) ) {
			ERROR("Problem reading user input");
		} else {
			states[curstate].update( buf );
		}
		handle_events();

		printf("\n");
	}

	terminate();

	return status;
}

void
controller_log_event ( int event ) {
	if (nevents < MAX_EVENTS) {
		events[nevents++] = event;	
	}	
}