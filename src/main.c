#include <stdio.h>
#include <stdlib.h>

#include "input.h"

#define PROMPT "$ " //the macro for replaceing the prompt for the $ as in the terminal

int main(void)
{
    char line[MAX_LINE + 1]; // + 1 for the closing '\0' (I put it at the end of the input.c file)

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

        //test to check if it is working
        puts(line); // read_line() drops the '\n', so puts() adds it back!!!
    }
}
