# ShellC

[![build](https://github.com/MatiiiBL/ShellC/actions/workflows/build.yml/badge.svg)](https://github.com/MatiiiBL/ShellC/actions/workflows/build.yml)

A tiny Unix shell written in C. Think of it as a very, very small `bash`.

## What it can do

- I'll just show a simple scenario case of what it looks like:


```
$ echo hello world
hello world
$ /usr/bin/printf "The cat's name is %s.\n" 'Pepe Gonzalez'
The cat's name is Pepe Gonzalez.
$ cd /usr
$ pwd
/usr
$ false
error: command exited with code 1
$ nope
error: nope: command not found
error: command exited with code 127
$ echo "oops, I forgot to close this
error: mismatched quotes
$ exit
```

Everything the shell itself prints (the `$ ` prompt and the `error:` lines) goes
to *stderr*.

## Building and running

You need a C compiler (`clang` or `gcc`) and `make`. That's it: no extra
libraries, just plain C and the standard Unix system calls. 

```sh
make          # builds build/shellc
make run      # builds it and starts the shell
make debug    # an extra-careful build in build/debug/shellc (more on that below)
make clean    # deletes the build/ folder
```

You can pipe the commands if you want to! 

```sh
printf 'echo hi\ncd /usr\npwd\n' | ./build/shellc
```

## How it works

Every line goes through the same few steps:

```
read a line ──► split it into words ──► is it cd or exit? ──yes──► the shell does it itself
                                                │
                                                no
                                                ▼
                                    start the program, wait for it,
                                    and say so if it failed
```

The code is split up the same way, one file per step:

| File | Functionality |
|---|---|
| `src/main.c` | the main loop: show the prompt, then read → split → run |
| `src/input.c` | reads one line of input |
| `src/parser.c` | splits the line into words and deals with the quotes |
| `src/builtin.c` | the built-in commands, `cd` and `exit` |
| `src/exec.c` | starts programs and reports how they ended |

Each one has a matching header in `include/`.

## Also...

It reads the input one byte at a time. Sounds slow, but it's because the programs that the shell starts shares the input. So, if the shell grabs everything at once, it could take lines that were ment to for the proram.

For instance:

```sh
printf 'cat\nhello\n' | ./build/shellc
```

`cat` should get the `hello`, and it does, because the shell only ever takes its
own line.

There's no malloc anywhere. The limit is at most 100 arguments, so a couple of fixed-size
arrays are enough more than enough!

cd and exit are built in. 

A failed command never becomes a second shell! Cause if it doesn't work, the fork would print the error and quit instantly with _exit(127)

I had a bug that if the output goes to a pipe that noone's reading, the system would kill it with the SIGPIPE signal, but now the shell catches the signal with a handler that literally does nothing and moves on. (if i just ignore it would be passed to every program that it starts)

Finally: this project was done by using assistive AI to help with concepts that were unknown to me as there's some aspects of the project that didn't grasp fully before. However, everything in this project was completely written and tested by myself! 