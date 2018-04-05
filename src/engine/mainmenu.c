#include "mainmenu.h"

#include "utils.h"
#include "command.h"

#include <stdio.h>
#include <stdlib.h>

static struct cmd_table *main_menu;

static int
new_game ( char *cmd, void *data ) {
	controller_log_event(EVENT_START_NEW_GAME);
	return 1;
}

static int
display_story ( char *cmd, void *data ) {
	return 1;
}

static int
display_help ( char *cmd, void *data ) {
	return 1;
}

static int
quit_game ( char *cmd, void *data ) {
	controller_log_event(EVENT_QUIT_GAME);
	return 1;
}

struct menu_item {
	char    *name;           // human readable name of the menu item
	char    *other_names[4]; // allow up to 4 "alternative" names
	cmd_ptr  action;         // what happens when we choose this item
} menu_items[] = {
	{ 
		.name = "New Game", 
		.other_names = { "new", "n", NULL },
		.action = new_game
	},
	{ 
		.name = "Story", 
		.other_names = { "s", NULL },
		.action = display_story
	},
	{ 
		.name = "Help", 
		.other_names = { "h", NULL },
		.action = display_help
	},
	{ 
		.name = "Quit", 
		.other_names = { "q", NULL },
		.action = quit_game
	},
};


static void
register_command ( char *name, cmd_ptr action ) {
	if ( cmd_table_register( main_menu, name, action, NULL ) < 0 ) {
		ERROR("Problem registering %s command", name);
	}
}

static int
print_main_menu ( void ) {
	for ( size_t i=0; i<sizeof(menu_items)/sizeof(menu_items[0]); i++ ) {
		fprintf( stdout, "%lu. %s\n", i+1, menu_items[i].name );	
	}
	return 1;
}

static int
main_menu_update ( char *input ) {	
	int status;

	status = cmd_table_execute( main_menu, input );
	if ( status < 0 ) {
		fprintf( stdout, "\"%s\" is not a valid command\n", input );
	}

	return 1;
}

int
main_menu_init ( void ) {
	char buf[3] = {0};
	
	main_menu = cmd_table_new( 13, NULL, str2lower );

	if (!main_menu) {
		ERROR("Allocation for main menu commands failed");
		return 0;
	}

	for ( size_t i=0; i<sizeof(menu_items)/sizeof(menu_items[0]); i++ ) {
		sprintf(buf, "%lu", i+1);

		register_command ( buf, menu_items[i].action );
		register_command ( menu_items[i].name, menu_items[i].action );

		char **p = menu_items[i].other_names;
		while (*p) {
			register_command ( *p, menu_items[i].action );
			p++;
		}
	}

	return 1;
}

void
main_menu_register_state ( struct state *state ) {
	state->enter  = NULL;
	state->render = print_main_menu;
	state->update = main_menu_update;
	state->exit   = NULL;
}

void
main_menu_quit ( void ) {
	cmd_table_free(main_menu);
}
