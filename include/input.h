//just the header for checking if it exists or not. If it doesn't.
//I just skip the definition of it (just in case to prevent already defined
//errors)
#ifndef SHELLC_INPUT_H
#define SHELLC_INPUT_H

//to usesize_t
#include <stddef.h>

// I will only accept a line with max 1000 characters (including \n)

#define MAX_LINE 1000

enum read_result {
    READ_OK,       // a whole line is in the buffer (without the \n) - 0
    READ_EOF,      // EOF (nothing more to read) - 1
    READ_TOO_LONG, // if the line didn't fit and was "skipped" - 2
    READ_NUL,      // if the line contained a '\0' byte and was skipped - 3
    READ_ERROR,    // if reading stdin failed for whatever reason (errno tells why) - 4
};


//I read from stin to buffer dropping the \n 
//I return one of the 5 states
//Note: the buffer must be MAX_LINE + 1 
enum read_result read_line(char *buf, size_t size);

#endif
