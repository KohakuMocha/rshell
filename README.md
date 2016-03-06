RSHELL
===============

Program Overview
-------------------
This project is a simple command shell that reads in user-input commands on one line and works similarly to the bash version of shell.
This if the for the 2nd Assignment for **CS100: Software Construction** at [UC Riverside](http://ucr.edu), taught by Brian Crites.

Installation Guide / How to Use
-------------------------------
Clone this project at: [https://github.com/jhsie007/rshell.git](https://github.com/jhsie007/rshell.git).

Once cloned, enter a shell and type `cd rshell` to enter the project directory.

1. Type `make` to compile the `rshell.cpp` and all executables will appear in the `./bin` folder, which will be created by Makefile.
    1. Type `make rshell` to compile and place `rshell` in the `./bin` folder


2. Running the executable using your existing shell will start a new pseudo-shell  which will prompt you with `username@hostname$ ` and wait for input.

3. Press enter after typing your desired commands, flags, and connectors.
    - Usage Structure: `[command]` `[argument(s)]` `[connector]` `[...]`.
    - Typing the `exit` command closes the executable.

Updates
-----------------------------
- Version 1.1:
    - fixed spacing problems when using connectors
    - added `()` or precedence operators
    - added `test` functionality
        - (use: `test` `[flag]` `[file_or_directory_name]`)
    - added `[]` for use of the test functionality
        - (proper use: `[ [flag] [file_or_directory_name] ]`)

Bugs/Known Issues/Limitations
-----------------------------
###rshell
* The `cd` command and all of its argument variants are not available in this version of shell. This may be an error related to permissions or availability of built-in commands of `execvp()`.
* In addition to the `cd` command having trouble in relation to recognizing directories, any other directory related command will fail to execute (ex: `ls` `[directory_name]`)
* The username and server identifier for the process line does not always appear on all stations when rshell is executing.
* When chaining a large number of improper commands using the `||` connector, not all error messages from `execvp()` will always be shown.
* When using parentheses, rshell will not be able to properly process input 2 spaces after the closing parentheses and will return an error.
* When using parentheses, having a larger number of open parentheses will not prompt the user for further input but will only return an error and ask for new input.

###io redirection
* Our input and output redirection does not work. (ex: `echo [string] > [file_name]` is unable to make a new file )

