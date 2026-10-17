#include "parser.h"

#include <stddef.h>

// Where the parser is right now. Each character is handled according to the
// current state, and some characters switch to another state.
enum parse_state {
    BETWEEN_WORDS, // skipping spaces, waiting for the next word to start
    IN_WORD,       // inside a word
};

//This is for parsing lines
enum parse_result parse_line(char *line, char *argv[], int *argc)
{
    enum parse_state state = BETWEEN_WORDS;
    int count = 0;  // words found so far
    size_t in = 0;  // where we read the next character from
    size_t out = 0; // where we write the next character of a word to

    // The words are written back into the same buffer, at `out`. `out` never
    // gets ahead of `in`, so we never overwrite a character we haven't read.
    while (line[in] != '\0') { // until the eol
        char c = line[in];
        in++;

        switch (state) {
        case BETWEEN_WORDS:
            if (c == ' ') {
                break; // just another space between words: skip it
            }
            if (count == MAX_WORDS) { // this is when the line would have too many args
                return PARSE_TOO_MANY_ARGS;
            }
            argv[count] = &line[out]; // a word starts here!
            count++;
            line[out] = c;
            out++;
            state = IN_WORD;
            break;

        case IN_WORD:
            if (c == ' ') {
                line[out] = '\0'; // the space ends the word: close it with '\0'
                out++;
                state = BETWEEN_WORDS;
            } else {
                line[out] = c;
                out++;
            }
            break;
        }
    }

    if (state == IN_WORD) {
        line[out] = '\0'; // the line ended inside a word: close that word too
    }

    argv[count] = NULL; // marks the end of the list!!
    *argc = count;
    return PARSE_OK;
}
