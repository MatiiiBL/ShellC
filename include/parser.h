#ifndef SHELLC_PARSER_H
#define SHELLC_PARSER_H

// The spec lets us reject a command line with more than 100 arguments.
// The command itself is not an argument, so a line may have 101 words.

//Built this for allowing up to 100 arguments (the command is not an argument, so the line can have 101 words!)
#define MAX_ARGS 100
#define MAX_WORDS (MAX_ARGS + 1)


enum parse_result {
    PARSE_OK,            // 0
    PARSE_TOO_MANY_ARGS, // 1 - more than MAX_ARGS arguments
};

//this is for splitting the line into words separated by spaces 
//argv will point to the location where the word starts (cannot reuse this) BUT, it needs space
//for the max_words + 1 pointers (after the LAST word we will get a null so we can use execvp (cause we will know where the list ends))
// also, the argc gets the number of words
enum parse_result parse_line(char *line, char *argv[], int *argc);

#endif
