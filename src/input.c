#include "input.h"

#include <errno.h>
#include <stdbool.h>
#include <unistd.h>


//helper for reading one byte at a time
static int read_byte(char *c)
{
    for (;;) {
        //signed size type in case read returns -1!!
        ssize_t n = read(STDIN_FILENO, c, 1); //stdin is just 0 but I write it so its more readable

        //If we got something!
        if (n == 1) {
            return 1;
        }
        //If 0 bytes, then it means the input is over! (just eof)
        if (n == 0) {
            return 0;
        }
        //just in case if we are interrupted by a signal to just repeat (not sure about this line, I'll test it later)
        if (errno != EINTR) { 
            return -1;
        }
    }
}

//the one that will read the whole line
enum read_result read_line(char *buf, size_t size)
{
    size_t len = 0;            // amount stored in buf so far
    size_t max_len = size - 1; // the last byte of buf is kept for the closing '\0'
    bool got_anything = false; // did we read at least one byte, even just '\n'?
    bool too_long = false;
    bool has_nul = false;      //the \0

    while (true) {
        char c;                //the byte we just read
        int r = read_byte(&c); //what the read_byte() returned

        if (r == -1) { // if the reading failed
            return READ_ERROR;
        }
        if (r == 0) { // if we don't have more input, we just leave the loop
            break;
        }
        got_anything = true;

        if (c == '\n') { // if eof

            // The '\n' counts towards the limit too: if buf is already full,
            // the line plus its '\n' is too long.
            if (len == max_len) { //I just check if the len got the buf full (including the "\n")
                too_long = true;
            }
            break;
        }

        //if we have a null terminator 
        if (c == '\0') {
            has_nul = true;
        }

        // in case we've got still room in the buf, we just store the byte
        if (len < max_len) {
            buf[len] = c;
            len++;
        } else { // buf is now full (so we just drop the byte buuut keep reading until I get \n 
            too_long = true;
        }
    }

    buf[len] = '\0'; // officially ready to close the string close the string

    //just returning the status now
    if (!got_anything) {
        return READ_EOF;
    }
    if (too_long) {
        return READ_TOO_LONG;
    }
    if (has_nul) {
        return READ_NUL;
    }
    return READ_OK;
}
