# Command Line Utility Program
## Overview
This C program implements a simple command-line utility that can do various tasks and operations on the system. Here are some of the commands it is capable of running:
- calculate: Solves a prefix arithmetic expression.
- time: Displays the local time.
- path: Displays the working directory.
- sys: Displays some basic system information.
- put: Copies specified files into a chosen directory, with an option to overwrite any existing directories.
- get: Displays the content of a file, 40 lines at a time.
- quit: Exits the program.

## Features
- Cross-Platform Compatibility: There is support for both POSIX-compliant systems (like Linux and macOS) and Windows.
- Prefix Expression Evaluation: Uses a recursion function to evaluate prefix expressions.
- File and Directory Management: The put command can create directories as well as copies files, while the get command reads file content 40 lines at a time.
- System Information: Displays operating system and CPU information.

## Dependencies
- Standard C libraries: stdio.h, stdlib.h, string.h, unistd.h, sys/types.h, sys/stat.h, time.h, dirent.h, errno.h.
- Platform-specific headers:
    - POSIX: sys/stat.h, sys/types.h.
    - Windows: direct.h.

## Compilation
To compile the program, you need to use a C compiler like gcc.

### Linux/macOS:
gcc -o utility-program Assignment1P1-TiamAdnum.c

### Windows:
gcc -o utility-program Assignment1P1-TiamAdnum.c -D _WIN32

## Usage
Once compiled, run the command:
./utility-program

## Commands
1. calculate: Solve a prefix expression.

calculate <expression>
Example: calculate + 3 4

2. time: Display the current local time.

time

3. path: Display the current working directory.

path

4. sys: Display basic system information.

sys

5. put: Copy files into a specified directory. Use -f to force overwrite existing directories.

put <folder-name> <file-name> <file-name> -f

6. get: Display the content of a file, 40 lines at a time.

get <file-name>

7. quit: End the program.

quit

## Error Handling
The program gracefully handles errors such as missing files, invalid directories, and incorrect command inputs.
