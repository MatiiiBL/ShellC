#include "builtins.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

// cd DIR: changes the shell's current directory to DIR (absolute like /usr,
// or relative like bin or ..). Programs started later inherit it.
static void builtin_cd(int argc, char *argv[])
{
    if (argc != 2) { // the word "cd" plus exactly one directory
        fputs("error: cd needs exactly one argument\n", stderr);
        return;
    }
    if (chdir(argv[1]) == -1) {
        perror("error: cd failed"); // e.g. "error: cd failed: No such file or directory"
    }
}

enum builtin_result run_builtin(int argc, char *argv[])
{
    // strcmp() returns 0 when the two strings are equal.
    if (strcmp(argv[0], "exit") == 0) {
        // Any arguments are ignored: the spec only asks for `exit`, and the
        // shell always ends with status 0.
        return BUILTIN_EXIT;
    }

    if (strcmp(argv[0], "cd") == 0) {
        builtin_cd(argc, argv);
        return BUILTIN_RAN;
    }

    return NOT_A_BUILTIN;
}
