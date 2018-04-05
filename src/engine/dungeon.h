#ifndef DUNGEON_H
#define DUNGEON_H

#define WALLS       0x0F // all wall bits
#define WALL_NORTH  0x01 // bit which represents if wall to North is open
#define WALL_WEST   0x02 // bit which represents if wall to West is open
#define WALL_SOUTH  0x04 // bit which represents if wall to South is open
#define WALL_EAST   0x08 // bit which represents if wall to East is open

struct room {
	char *desc;
	int in_dungeon;
	int walls;
};

int          dungeon_init               ( void );
struct room *dungeon_get_room           ( int room_id );
int          dungeon_get_offset_room_id ( int room_id, int modifier );

#endif