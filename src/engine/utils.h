#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define LONG_STRING 512

#define LOGLEVEL_DISABLE 0
#define LOGLEVEL_ERROR   1
#define LOGLEVEL_INFO    2
#define LOGLEVEL_DEBUG   3

// Default log level is ERROR
#ifndef LOGLEVEL
#define LOGLEVEL LOGLEVEL_ERROR
#endif

// logging backend
#define _LOG(level, ...) {					\
	fprintf(stderr, level);					\
	fprintf(stderr, " ");					\
	fprintf(stderr, "[%s:%i] : ", __FILE__, __LINE__);	\
	fprintf(stderr, __VA_ARGS__);				\
	fprintf(stderr, "\n");					\
    }

// error message macro
#if (LOGLEVEL >= LOGLEVEL_ERROR)
#define ERROR(...) {			\
	_LOG("ERROR", __VA_ARGS__);	\
	}
#else
#define ERROR(...) {}
#endif

// info message macro
#if (LOGLEVEL >= LOGLEVEL_INFO)
#define INFO(...) {			\
	_LOG("INFO ", __VA_ARGS__);	\
    }
#else
#define INFO(...) {}
#endif

// debug message macro
#if (LOGLEVEL >= LOGLEVEL_DEBUG)
#define DEBUG(...) {			\
	_LOG("DEBUG", __VA_ARGS__);	\
    }
#else
#define DEBUG(...) {}
#endif
void utils_delay( unsigned long ms );
int  utils_prompt_user ( char *buf, int maxbuf, char *msg );

void str2lower ( char *dest, char *src, int max );

#endif