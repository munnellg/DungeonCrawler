#include "dungeon.h"

#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

static struct room dungeon[] = {
	{
		.desc=
		"You are in a room made entirely of solid, grey stone. The air is\n"\
		"cold. You sense that somewhere in the depths of the maze, dark\n"\
		"creatures are stirring.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc=
		"You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc=
		"You gaze into the abyss. And the abyss gazes back.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc=
		"Oh look. There's a portrait of Hitler on the wall of this room.\n"\
		"Guess this dungeon is definitely evil so.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc=
		"The only item in this room is a single solitary Easter egg. You\n"\
		"suspect that the egg has less than honourable intentions.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc=
		"A machine sits in the middle of this room. A label on the side\n"\
		"reads, \"Room Descriptor Generator\". A sign under that sign reads\n"\
		"\"Out of order\"\n\n"
		"Bollocks!",
		.in_dungeon=0,
		.walls=0
	},

	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc=
		"Christ, I don't know!\n\n"\
		"A koala bear.\n\n" \
		"There is an passive aggressive little koala bear in this room. It\n"\
		"is ignoring you.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc="You are in a room.",
		.in_dungeon=0,
		.walls=0
	},
	{
		.desc=
		"There is a sign on the wall of this room. It reads:\n\n"\
		"    If I ever finish this game, there will be a boss battle here\n"\
		"       --- Gary",
		.in_dungeon=0,
		.walls=0
	},
};

static int dungeon_width=0, dungeon_height=0;

static int
offset_index ( int ci, int wall, int w, int h ) {
	return ci+(wall==WALL_EAST)-(wall==WALL_WEST)+
			w*(wall==WALL_SOUTH)-w*(wall==WALL_NORTH);
}

//==== FUNCTION =============================================================== 
// Generate a dungeon using a randomized version of Primm's algorithm
//============================================================================= 
static int
generate_dungeon ( int w, int h ) {
	int ci, wi, ni, wall;	
	int len = 0;                               // wall list length of zero
	int *wlist = calloc( w*h*4, sizeof(int) ); // empty wall list
	
	// if wall list allocation failed, report error
	if (!wlist) {
		ERROR("Failed to allocate wall list for dungeon gen");
		return 0; 
	} 
	
	ci = rand()%(w*h);          // choose random starting room
	dungeon[ci].in_dungeon = 1; // mark room as part of dungeon

	// add walls of room to wall set
	for( wall=1; wall<0x10; wall<<=1 ) {
		wlist[len++] = (ci<<4)|wall; // LO 4 bits are walls HI are room index
	}
	
	while(len > 0) {
		// select a random wall set from the wall list
		wi = rand()%len;
		ci = wlist[wi]>>4;
		wall = wlist[wi]&WALLS;

		// determine the neighbour on the other side of the wall
		ni = offset_index( ci, wall, w, h );

		// check bounds
		if(ni>=0 && ni<(w*h) && (ni%w==ci%w||ni/w==ci/w)) {
			// check if we've already visited the neighbouring room
			if( !(dungeon[ni].in_dungeon) ) {
				// if not, form a wall between current room and neighbour
				dungeon[ci].walls &= ~wall;				
				dungeon[ni].walls &= ~(((wall<<2)+(wall>>2))&WALLS);
				// mark as part of dungeon and add neighbour to wall set
				dungeon[ni].in_dungeon = 1;
				for( wall=1; wall<0x10; wall<<=1 ) {
					if( wall & dungeon[ni].walls ) { 
						wlist[len++] = (ni<<4)|wall; 
					}
				}				
			}
		}

		// remove the wall from the list of available walls
		len--;			
		wlist[wi] = wlist[len];
	}

	free(wlist);

	return 1;
}

static void
destroy_dungeon ( void ) {
	size_t dungeon_size = (sizeof(dungeon)/sizeof(dungeon[0]));
	for ( int i = 0; i<dungeon_size; i++ ) {
		dungeon[i].in_dungeon = 0; 
		dungeon[i].walls = WALLS;
	}
}

int
dungeon_init ( void ) {
	destroy_dungeon ();
	
	// Dungeon must be square (makes some things a little easier).
	dungeon_width = dungeon_height = sqrt(sizeof(dungeon)/sizeof(dungeon[0]));
	if( !generate_dungeon( dungeon_width, dungeon_height ) ) {
		return 0;
	}

	return 1;
}

struct room*
dungeon_get_room ( int room_id ) {
	if ( room_id < 0 || room_id >= dungeon_height*dungeon_width ) {
		return NULL;
	}

	return &dungeon[room_id];
}

int
dungeon_get_offset_room_id ( int room_id, int modifier ) {
	return offset_index( room_id, modifier, dungeon_width, dungeon_height );
}