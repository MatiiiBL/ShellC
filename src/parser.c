#include "parser.h"

#include <stdbool.h>
#include <stddef.h>


//This is for parsing lines
enum parse_result parse_line(char *line, char *argv[], int *argc)
{
    int count = 0; // words found so far
    size_t i = 0;  // where we are in line

    while (true) {
        // skip the spaces before the next word
        while (line[i] == ' ') {
            i++;
        }
        if (line[i] == '\0') { // eol (no more words)
            break;
        }

        if (count == MAX_WORDS) { // this is when the line would have too many args
            return PARSE_TOO_MANY_ARGS;
        }
        argv[count] = &line[i]; // a word starts here!
        count++;

        // walk to the end of the word
        while (line[i] != ' ' && line[i] != '\0') {
            i++;
        }
        if (line[i] == ' ') {
            line[i] = '\0'; // the space becomes now the word's closing '\0'
            i++;
        }
    }

    argv[count] = NULL; // marks the end of the list!!
    *argc = count;
    return PARSE_OK;
}
