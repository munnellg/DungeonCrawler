#ifndef CONTROLLER_H
#define CONTROLLER_H

#define EVENT_START_NEW_GAME 0
#define EVENT_QUIT_GAME      1
#define EVENT_GAME_OVER      2
#define EVENT_SHOW_MAIN_MENU 3

struct state {
	int (*enter)  (void);
	int (*render) (void);
	int (*update) ( char * input );
	int (*exit)   (void);
};

void controller_log_event ( int event );

int  launch_program   ( void );

#endif