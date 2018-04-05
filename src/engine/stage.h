#ifndef STAGE_H
#define STAGE_H

#define STAGE_NORTH 0
#define STAGE_WEST  1
#define STAGE_SOUTH 2
#define STAGE_EAST  3

int  stage_load     ( int room_id );
void stage_describe ( void );
void stage_move     ( int direction );

#endif