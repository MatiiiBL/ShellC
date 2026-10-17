#include "builtins.h"

#include <string.h>

enum builtin_result run_builtin(int argc, char *argv[])
{
    (void)argc; // not needed yet: `exit` doesn't look at its arguments

    // strcmp() returns 0 when the two strings are equal.
    if (strcmp(argv[0], "exit") == 0) {
        // Any arguments are ignored: the spec only asks for `exit`, and the
        // shell always ends with status 0.
        return BUILTIN_EXIT;
    }

    return NOT_A_BUILTIN;
}
