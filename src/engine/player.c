#include "player.h"

#include "utils.h"
#include <stdio.h>

#define MAX_NAME 64

#define PLAYER_BASE_HEALTH        15
#define PLAYER_BASE_STRENGTH       5
#define PLAYER_BASE_DEFENCE        5
#define PLAYER_BASE_SPEED          5
#define PLAYER_BASE_LEVEL          1
#define PLAYER_BASE_EXPERIENCE     0

struct player {
	char name[MAX_NAME];
	int  max_health, health, strength, defence, speed, level, exp;
} player;

void
player_generate ( void ) {
	int happy = 0;
	char buf[LONG_STRING] = {0};

	player.max_health = PLAYER_BASE_HEALTH;
	player.health     = PLAYER_BASE_HEALTH;
	player.strength   = PLAYER_BASE_STRENGTH;
	player.defence    = PLAYER_BASE_DEFENCE;
	player.speed      = PLAYER_BASE_SPEED;
	player.level      = PLAYER_BASE_LEVEL;
	player.exp        = PLAYER_BASE_EXPERIENCE;

	while (!happy) {
		utils_prompt_user(player.name, MAX_NAME, "Player Name");
		player_print_stats();		
		utils_prompt_user(buf, LONG_STRING, "Accept choices? [y/n]");
		happy = (buf[0] == 'y');
	}
}

void
player_print_stats ( void ) {
	fprintf(stdout, "%s\n", player.name);
	fprintf(stdout, "|    HP   | STR | DEF | SPD |\n");
	fprintf(stdout, "| %3d/%3d | %3d | %3d | %3d |\n",
		player.health, player.max_health, player.strength, player.defence,
		player.speed );
	fprintf(stdout, "Level     : %d\n", player.level);
	fprintf(stdout, "Experience: %d\n", player.exp);
}

int
player_shout ( void ) {
    static int volume = 0;

    volume %= 4;
    switch(volume) {
        case 0:
            fprintf(stdout, "Aargh.\n");
            break;
        case 1:
            fprintf(stdout, "AARGH!\n");
            break;
        case 2:
            fprintf(stdout, "YEARGH!!!\n");
            break;
        case 3:
            fprintf(stdout, "GWARBLEGARBLEGARBLE!!!\n");
            break;
        default:
            break;
    }

    return volume++;
}

int
player_jump ( void ) {
    static unsigned int height=0;

    height %= 4;
    switch(height) {
        case 0:
            fprintf(stdout, "hop\n");
            break;
        case 1:
            fprintf(stdout, "hip hop\n");
            break;
        case 2:
            fprintf(stdout, "hoppity hip hop\n");
            break;
        case 3:
            fprintf(stdout, "With a single great bound, you leap into orbit\n");
            utils_delay(3000);
            fprintf(stdout, "You suffocate slowly in the vacuum of space...\n");
            utils_delay(3000);
            fprintf(stdout, "Just kidding\n");
            break;
        default:
            break;
    }

    return height++;
}