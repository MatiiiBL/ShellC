#ifndef SHELLC_EXEC_H
#define SHELLC_EXEC_H

// Runs a command as a new process and waits until it has finished.
// argv[0] (is the path of the program), the other words are its arguments,
// and the list ends with NULL (as parse_line() makes it).
void run_command(char *argv[]);

#endif
