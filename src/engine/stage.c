#include "stage.h"

#include "utils.h"
#include "dungeon.h"
#include <stdio.h>
#include <string.h>

static char *directions[] = {
	"North",
	"West",
	"South",
	"East",
};

static struct room *current_room;
static char passage_desc[LONG_STRING];
static int current_room_id = 0;

static int
count_passages ( int walls ) {
	walls = walls - ((walls >> 1) & 0x55555555);
	walls = (walls & 0x33333333) + ((walls >> 2) & 0x33333333);
	return (((walls + (walls >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

int
stage_load ( int room_id ) {	
	static size_t num_directions = sizeof(directions)/sizeof(directions[0]);

	struct room *temp = dungeon_get_room(room_id);
	if ( !temp ) { return 0; }
	current_room = temp;
	current_room_id = room_id;

	int n = count_passages( (~current_room->walls & WALLS) );

	switch(n) {
		case 0:
			sprintf( passage_desc, "There is no way out of this room");
			break;
		case 1:
			sprintf( passage_desc, "There is a passage to the ");
			break;
		default:
			sprintf( passage_desc, "There are passages to the ");
	}
		
	for ( int i=0; i<num_directions; i++ ) {
		if ( !(current_room->walls & (1<<i)) ) {
			strcat( passage_desc, directions[i] );
			strcat( passage_desc, ", " );			
		}
	}
	
	// delete trailing comma/space by appending a full stop	
	size_t slen = strlen(passage_desc);
	passage_desc[slen-2] = '.';

	return 1;
}

void
stage_move ( int d ) {
	if ( (current_room->walls & (1<<d)) ) {
		fprintf(stdout, "There is no passage to the %s.\n", directions[d]);
		return;
	}

	stage_load( dungeon_get_offset_room_id( current_room_id, 1 << d ) );

	fprintf(stdout, "You travel %s.\n", directions[d]);
}

void
stage_describe ( void ) {
	fprintf(stdout, "%s\n\n", current_room->desc);
	fprintf(stdout, "%s\n", passage_desc);
}