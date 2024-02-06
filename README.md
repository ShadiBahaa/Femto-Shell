# Femto Shell (version 3)
## Description:
### A simulation for the bash shell. Some commands are implemented from scratch (pwd, cd, echo, and exit) and the other external commands are executed using execvp system call. 
## Features:
### 1- You can choose between two formats of shell: Formal one (Which is similar to the real one: shows the current path and waits for the command), and Informal one (Where the path is replaced with jokes that are selected randomly)
### 2- Executes all external commands and some of internal commands. (with high ability to recognize spaces that are not part of the command)
### 3- Handles all system calls and libraries errors and handles also any mistyping by the user or misuse which occurs as a result of passing wrong arguments.
## Structure:
### 1- src directory : contains all C source files (shell C file, commands implementations files)
### 2- include directory : contains header files (utilities, commands headers, and shell header)
### 3- Makefile: responsible for building the project and outputing the executable file
## How to run:
### 1- Download the repository.
### 2- Open the terminal in the repo directory.
### 3- Install make using the following command: sudo apt install make.
### 3- Write in the terminal (make all) to execute the makefile.
### 4- Inside the output folder, run femto_shell.exe file by typing (./femto_shell.exe) in the terminal
### 5- Enjoy!
## Test Video:
### Check the test video here: https://drive.google.com/file/d/1wrFxI8GE-0koQTMloY6rsb7JkzPuF3TH/view?usp=sharing
