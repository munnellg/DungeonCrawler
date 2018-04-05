#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>

#define CMD_MAX_LEN 64

typedef uint32_t (*hashfptr) ( char *s );
typedef void (*preprocfptr) ( char *dest, char *src, int max ); 
typedef int (*cmd_ptr) ( char *cmd, void *usrptr );

struct cmd {
	char cmd_code[CMD_MAX_LEN];
	cmd_ptr command;
	void *usrptr;
};

struct cmd_table {
	struct cmd **hashes;   // hash table key lookup
	struct cmd  *commands; // hash table value commands
	hashfptr hashf;        // hash function used by table
	preprocfptr preprocf;  // preprocessing before hashing 
	int size;              // number of commands in the table
	int hash_max;          // maximum number of items in table
};

struct cmd_table* cmd_table_new      ( int hash_max, hashfptr hashf,
	preprocfptr preprocf );
int               cmd_table_execute  ( struct cmd_table *ct, char *cmd );
int               cmd_table_register ( struct cmd_table *ct, char *cmd,
	cmd_ptr callback, void *usrptr );
void              cmd_table_free     ( struct cmd_table *ct );

#endif