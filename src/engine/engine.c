#include "engine.h"

#include "utils.h"
#include "player.h"
#include "command.h"
#include "dungeon.h"
#include "stage.h"
#include "controller.h"

static struct cmd_table *commands;

static int
cmd_shout ( char *cmd, void *data ) {
	player_shout();
	return 1;
}

static int
cmd_jump ( char *cmd, void *data ) {
	player_jump();
	return 1;
}

static int
cmd_look ( char *cmd, void *data ) {
	stage_describe();
    return 1;
}

static int
cmd_north ( char *cmd, void *data ) {
	stage_move(STAGE_NORTH);
	return 1;
}

static int
cmd_south ( char *cmd, void *data ) {
	stage_move(STAGE_SOUTH);
	return 1;
}

static int
cmd_east ( char *cmd, void *data ) {
	stage_move(STAGE_EAST);	
	return 1;
}

static int
cmd_west ( char *cmd, void *data ) {
	stage_move(STAGE_WEST);
	return 1;
}

static int
cmd_menu ( char *cmd, void *data ) {
	controller_log_event(EVENT_SHOW_MAIN_MENU);
    return 1;
}

static int
cmd_quit ( char *cmd, void *data ) {
	controller_log_event(EVENT_QUIT_GAME);
	return 1;
}

static int
engine_enter ( void ) {
	// create a dungeon for us to explore
	if(!dungeon_init()) { 
		ERROR("Dungeon initialization failed");
		return 0; 
	}

	player_generate();

	if ( !stage_load(0) ) {
		ERROR("Stage failed to load starting room");
		return 0;
	}

	return 1;
}

static int
engine_update ( char *input ) {	
	
	if ( cmd_table_execute( commands, input ) < 0 ) {
		fprintf( stdout, "\"%s\" is not a valid command\n", input );
	}

	return 1;
}

static int
engine_exit ( void ) {
	return 1;
}

static void
register_command ( char *name, cmd_ptr action, int register_short ) {
	char short_name[2] = {0};
	if ( cmd_table_register( commands, name, action, NULL ) < 0 ) {
		ERROR("Problem registering %s command", name);
	}

	if ( register_short ) {
		short_name[0] = name[0];
		if ( cmd_table_register( commands, short_name, action, NULL ) < 0 ) {
			ERROR("Problem registering %s command", name);
		}
	}	
}

int
engine_init ( void ) {
	commands = cmd_table_new( 18, NULL, str2lower );

	if (!commands) {
		ERROR("Allocation for game engine commands failed");
		return 0;
	}

	register_command( "jump",  cmd_jump,  1 );
	register_command( "north", cmd_north, 1 );
	register_command( "south", cmd_south, 1 );
	register_command( "east",  cmd_east,  1 );
	register_command( "west",  cmd_west,  1 );
	register_command( "shout", cmd_shout, 0 );
	register_command( "menu",  cmd_menu,  1 );
	register_command( "look",  cmd_look,  1 );
	register_command( "quit",  cmd_quit,  1 );

	return 1;
}

void
engine_register_state ( struct state *state ) {
	state->enter  = engine_enter;
	state->update = engine_update;
	state->exit   = engine_exit;
}

void
engine_quit ( void ) {
	cmd_table_free(commands);
}