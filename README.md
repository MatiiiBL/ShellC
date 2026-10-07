# ShellC

A Unix shell written in C.

## Requirements

- A C17 compiler (`clang` or `gcc`)
- `make`

## Build & run

```sh
make          # build into build/shellc
make run      # build and run
make debug    # rebuild with AddressSanitizer + UBSan
make clean    # remove build/
```

## Project layout

```
ShellC/
├── include/   # header files (.h)
├── src/       # source files (.c)
├── build/     # compiled output (not tracked)
└── Makefile
```
