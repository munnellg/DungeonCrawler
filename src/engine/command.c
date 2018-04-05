#include "command.h"

#include "utils.h"
#include <string.h>
#include <stdlib.h>

static uint32_t
hash_w2v ( char *s ) {
	uint64_t hash = 0;	  
	while (*s) { hash = hash * 0x101 + *s++; };  	
	return hash;
}

static void
no_preprocessing ( char *dest, char *src, int max ) {
	strncpy( dest, src, max-1 );
	dest[max-1] = 0;
}

//=== FUNCTION ================================================================
//         Name: table_lookup
//  Description: Find a value in the hash table using linear probing.
//               If a value is not in the hash table then this function will
//               return the hash where it **should** be. Hence this function
//               can be used both for lookup and finding an insertion point.
//               If the table is full then this function will return -1.
//=============================================================================
static int
lookup ( struct cmd_table *ct, char *s ) {
	int hash = ct->hashf(s)%ct->hash_max;   // hash the input string
	struct cmd *cmd = ct->hashes[hash]; // get table entry for the key
	int start = hash;        // guard against loops if table full

	// if entry is NULL key not in table, but should be here so return this hash
	while ( cmd ) {                     
		// Command has a value. Check if its the value we are looking for
		if ( !strncmp(cmd->cmd_code, s, CMD_MAX_LEN) ) {   
			// if so, return reference to this command
			break;                        
		}
		// If value is not the one we are looking for, probe
		hash = (hash+1)%ct->hash_max; // increment hash on collision		
		cmd = ct->hashes[hash];       // get new hash entry to see if empty
		if ( hash%ct->hash_max == start ) { // we've looped. Table full
			hash = -1;                // -1 is error code for full table
			break;                    // return error
		} 
	}

	// return hash index of key (-1 if table full and key not in table )
	return hash; 
}

struct cmd_table*
cmd_table_new ( int hash_max, hashfptr hashf, preprocfptr preprocf ) {
	struct cmd_table *ct;

	// Allocate memory for hash table. malloc returns NULL if allocation fails
	ct = malloc( sizeof(struct cmd_table) );
	// Ensure allocation succeeded
	if (ct) {
		// Allocate zeroed memory for hashes and buckets
		ct->hashes = calloc( hash_max, sizeof( struct cmd *) );
		ct->commands = calloc( hash_max, sizeof( struct cmd ) );
		
		// Initialize other hash table values
		ct->size=0; 
		ct->hash_max = hash_max;
		if (!hashf) hashf = hash_w2v;
		if (!preprocf) preprocf = no_preprocessing;

		ct->hashf = hashf;
		ct->preprocf = preprocf;

		// if any of our allocations failed, release any successfully allocated
		// memory and return NULL;
		if ( !ct->hashes || !ct->commands ) {
			cmd_table_free(ct);
			ct = NULL;
		}
	}
 	
 	// return the newly allocated table or NULL on failure
	return ct;
}

//=== FUNCTION ================================================================
//         Name: table_insert
//  Description: Add an entry to the hash table
//=============================================================================
int
cmd_table_execute ( struct cmd_table *ct, char *cmd_code ) {
	char buf[CMD_MAX_LEN];	
	ct->preprocf( buf, cmd_code, CMD_MAX_LEN );

	int hash = lookup( ct, buf );

	// Check if the command is valid
	if ( hash < 0 ) { return -1; }	
	if ( !ct->hashes[hash] )  { return -1; }

	return ct->hashes[hash]->command( cmd_code, ct->hashes[hash]->usrptr );
}

//=== FUNCTION ================================================================
//         Name: table_insert
//  Description: Add an entry to the hash table
//=============================================================================
int
cmd_table_register ( struct cmd_table *ct, char *cmd_code, cmd_ptr callback,
		void *usrptr ) {
	char buf[CMD_MAX_LEN];
	ct->preprocf( buf, cmd_code, CMD_MAX_LEN );

	int hash = lookup( ct, buf );
	if ( hash < 0 ) { ERROR("Command table full"); return -1; }
	
	if ( !ct->hashes[hash] )  {
		// Add the new key to the hash table
		ct->hashes[hash] = &ct->commands[ct->size];		
		strncpy( ct->commands[ct->size++].cmd_code, buf, CMD_MAX_LEN );
	}

	// Command in table. Set up pointers
	ct->hashes[hash]->command = callback;
	ct->hashes[hash]->usrptr = usrptr;

	return 1;
}

void
cmd_table_free ( struct cmd_table *ct ) {
	// before attempting to free anything, make sure there is something to free
	if (ct) {
		if ( ct->commands ) { free(ct->commands); }
		if ( ct->hashes ) { free(ct->hashes); }
		free(ct);
	}
}