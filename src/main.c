#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "parser.h"

#define PROMPT "$ " //the macro for replaceing the prompt for the $ as in the terminal

int main(void)
{
    char line[MAX_LINE + 1]; // + 1 for the closing '\0' that the read_line will add
    char *argv[MAX_WORDS + 1]; // the words of the line, then a NULL
    int argc;                  // how many words

    //infinite loop for asking the user prompts
    for (;;) {
        //remember that the wording asks me to put in in stderr!
        fputs(PROMPT, stderr);

        switch (read_line(line, sizeof line)) {
        case READ_OK:
            break;
        case READ_EOF:
            return EXIT_SUCCESS;
        case READ_TOO_LONG:
            fprintf(stderr, "error: line too long (max %d characters, newline included)\n", MAX_LINE); //I just use d cause it's an int
            continue;
        case READ_NUL:
            fputs("error: line contains a NUL byte\n", stderr);
            continue;
        case READ_ERROR:
            perror("error: cannot read input");
            return EXIT_FAILURE;
        }

        switch (parse_line(line, argv, &argc)) { //if it's fine
        case PARSE_OK:
            break;
        case PARSE_TOO_MANY_ARGS: //Give the user the warning
            fprintf(stderr, "error: too many arguments (max %d)\n", MAX_ARGS);
            continue;
        }

        if (argc == 0) { // blank line: nothing to be done!!!
            continue;
        }

        
        putchar('\n');
    }
}
