#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "exec.h"
#include "input.h"
#include "parser.h"

#define PROMPT "$ " //the macro for replaceing the prompt for the $ as in the terminal

// Does nothing: see survive_closed_pipes().
static void on_sigpipe(int sig)
{
    (void)sig;
}

// Writing to a pipe that nobody reads anymore (e.g. our stderr after
// `./shellc 2>&1 | head -1` has finished) sends us SIGPIPE, which kills the
// shell by default. With this empty handler the write just fails instead.
// (Not SIG_IGN: an ignored signal stays ignored in the programs we start,
// while a handler is reset to the default by exec, so they are unaffected.)
static void survive_closed_pipes(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof sa); // start with every field set to 0
    sa.sa_handler = on_sigpipe;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // don't interrupt other system calls either
    sigaction(SIGPIPE, &sa, NULL);
}

int main(void)
{
    char line[MAX_LINE + 1]; // + 1 for the closing '\0' that the read_line will add
    char *argv[MAX_WORDS + 1]; // the words of the line, then a NULL
    int argc;                  // how many words

    survive_closed_pipes();

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
        case PARSE_MISMATCHED_QUOTES:
            fputs("error: mismatched quotes\n", stderr);
            continue;
        }

        if (argc == 0) { // blank line: nothing to be done!!!
            continue;
        }

        // Built-ins first: the shell runs those itself, without a new process.
        switch (run_builtin(argc, argv)) {
        case BUILTIN_EXIT:
            return EXIT_SUCCESS; // `exit`: give control back to whoever started us
        case BUILTIN_RAN:
            continue;
        case NOT_A_BUILTIN:
            break;
        }

        run_command(argv);
    }
}
