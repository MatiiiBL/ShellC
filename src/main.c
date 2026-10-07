#include <stdio.h>
#include <stdlib.h>

#define PROMPT "$ " //the macro for replace the prompt for the $ as in the terminal

int main(void)
{
    char line[1024];

    //infinite loop for asking the user prompts
    for (;;) {
        //remember that the wording asks me to put in in stderr!
        fputs(PROMPT, stderr);

        if (fgets(line, sizeof line, stdin) == NULL) {
            break; //this is our EOF (could also happen if the buffer gets full (in our case >1023))
        }

        //test to check if it is working
        fputs(line, stdout);
    }

    return EXIT_SUCCESS;
}
