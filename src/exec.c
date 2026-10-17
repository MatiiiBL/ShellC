#include "exec.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void run_command(char *argv[])
{
    // fork() makes a copy of this whole process, and both copies go on
    // running from here. In the copy (the "child") fork() returns 0; in the
    // original (the "parent", our shell) it returns the child's process id.
    // If no copy could be made (e.g. too many processes), it returns -1.
    pid_t pid = fork();

    if (pid == -1) {
        perror("error: fork failed");
        // No child to wait for. (waitpid(-1, ...) would mean something like "wait for ANY
        // child", so we must not get there.)
        return;
    }

    if (pid == 0) {
        // Child: turn this copy into the program we want to run.
        // execvp() finds the program like this: if the name contains a '/'
        // (like /bin/ls or ./a.out), it is used as a path directly; if not
        // (like ls), each folder listed in $PATH is tried until one has it.
        // If execvp() works, it never comes back here.
        execvp(argv[0], argv);

        // Still here, so execvp() failed, and errno says why (e.g. "No such
        // file or directory"). stderr has no buffer, so this is written at once.
        if (errno == ENOENT && strchr(argv[0], '/') == NULL) {
            // a plain name that none of the $PATH folders has
            fprintf(stderr, "error: %s: command not found\n", argv[0]);
        } else {
            fprintf(stderr, "error: %s: %s\n", argv[0], strerror(errno));
        }

        // This copy must never go back to the shell's loop (there would be
        // two shells!), so it leaves right away.
        // _exit() instead of exit(): it doesn't flush the stdio buffers the
        // child inherited from the shell, so nothing gets printed twice.
        // 127 is the exit code shells use for "the command could not be run".
        _exit(127);
    }

    // Parent: wait until the child has finished.
    int status;
    while (waitpid(pid, &status, 0) == -1) {
        if (errno != EINTR) {
            perror("error: waitpid failed");
            return;
        }
        // EINTR: a signal interrupted the waiting, so just wait again
    }

    // status packs up how the child ended; the W...() macros unpack it.
    if (WIFEXITED(status)) {
        // It ended by itself (returned from main or called exit) with a code:
        // 0 means success, anything else means something went wrong.
        int code = WEXITSTATUS(status);
        if (code != 0) {
            fprintf(stderr, "error: command exited with code %d\n", code);
        }
    } else if (WIFSIGNALED(status)) {
        // A signal killed it (e.g. a crash, or someone ran kill -9 on it).
        fprintf(stderr, "error: command terminated by signal %d\n", WTERMSIG(status));
    }
}
