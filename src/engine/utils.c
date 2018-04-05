#include "utils.h"
#include <ctype.h>

// Delay works differently on different operating systems
// Figure out the OS at compile time and implement the delay function as 
// appropriate
#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

#include <windows.h>
// Delay for windows
void utils_delay( unsigned long ms ) {
    Sleep( ms );
}

#else  /* assume POSIX */

#include <unistd.h>

// Delay for Linux
void utils_delay( unsigned long ms ) {
    usleep( ms * 1000 );
}

#endif 

int
utils_prompt_user ( char *buf, int maxbuf, char *msg ) {
	int i;

    // prompt the user for input
	if (msg) {
		fprintf( stdout, "%s: ", msg );
	} else {
        fprintf( stdout, ">> " );
    }

    // if the read fails, then report the error by returning 0
    if( !fgets( buf, maxbuf-1, stdin ) ) {
    	DEBUG( "fgets returned NULL" );
        return 0;
    } 

    // replace newlines at the end of the string with a NULL 
    // character. They're just noise anyway.
    for( i=0; i<maxbuf; i++ ) {
        if(buf[i] == '\n') {
            buf[i] = '\0';
            return 1;
        }
    }

    // standard safety check. Make sure the string is definitely terminated
    // at some point. Handles the case where the user's input was longer 
    // than the size of the input buffer. We won't lose any data from the 
    // stream because we deliberately read one less character than the 
    // buffer can hold anyway.
    buf[maxbuf-1] = 0;

    return 1;
}

void
str2lower ( char *dest, char *src, int max ) {
    int i;  
    for ( i=0; i<max-1 && src[i] != 0; i++ ) {
        dest[i] = tolower(src[i]);
    }
    dest[i] = 0;
}