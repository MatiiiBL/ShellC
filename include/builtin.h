#ifndef SHELLC_BUILTIN_H
#define SHELLC_BUILTIN_H

// Built-in commands are run by the shell itself, without starting a new
// process (fork + exec). This matters for commands that must change the
// shell itself, like `exit` (which has to end the shell, not a child) and
// `cd` (which has to change the shell's own current directory).

enum builtin_result {
    NOT_A_BUILTIN, // argv[0] is not a built-in: run it as a program instead
    BUILTIN_RAN,   // it was a built-in, and it has run
    BUILTIN_EXIT,  // it was `exit`: the shell should stop now
};

// If argv[0] is the name of a built-in command, runs it.
// argc is the number of words in argv (as parse_line() gives it).
enum builtin_result run_builtin(int argc, char *argv[]);

#endif
