# Asssignment-2: SimpleShell
##### Abhirup Das | Armaan Singh
## File Structure

```
SIMPPLESHELL
|    
|---bin
|    |---fib.c
|    |---fib
|    |---helloworld.c
|    |---helloworld
|    |---file.txt
|    |---Shellscript.sh
|
|---Colors
|    |---color.c
|    |---color.h
|    |---Makefile
|
|---Shellcommands
|    |---shell.c
|    |---shell.h
|    |---Makefile
|
|---main.c
|---Makefile

```
## Code_Explanantion
### Shellcommmands
#### shell.c
```
It includes functionalities for executing commands, handling input/output redirection, and maintaining a command history.

The main components of the code are as follows:

1. Signal Handling:
- The Escape_sequence function is used to handle signals, specifically SIGINT and SIGCHLD.
- When SIGINT is received, it prints a newline character and displays the exit sequence for each executed command.
- When SIGCHLD is received, it waits for child processes to terminate.

2. Command Execution:
- The launch function is responsible for executing commands.
- It handles different modes of execution based on the value of the mode parameter.
- If the command is not found using the which command, it prints an error message.
- If the command is found, it creates an array of arguments (args) and uses execvp to execute the command.

3. Shell Loop:
- The shell_loop function is the main loop of the shell program.
- It displays the shell prompt, reads user input using fgets, and removes the newline character from the input.
- It handles empty input and updates the command history (user_input array).
- It checks for background execution and input/output redirection using pipes.
- It executes commands using the launch function and measures the execution time.
- It updates the exit sequence for each executed command.

-----Function-wise Explanation-----
The 'Escape_sequence()' function handles the signals SIGINT and SIGCHLD. When SIGINT is received, it prints the exit_sequence for each executed command and exits the program. When SIGCHLD is received, it waits for child processes to terminate.

The 'trim()' function removes leading and trailing whitespace characters from a string.

The 'lastBack()' function checks if the last character of a string is an ampersand (&), indicating a background process.

The 'shell_loop()' function is the main loop of the shell program. It prompts the user for input, handles empty input, and stores the user's input in an array. It also detects if the input ends with an ampersand, indicating a background process. It then launches the command, captures the start and end time of the execution, and calculates the duration. The execution details are stored in the exit_sequence array.
```
####  shell.h
```
It contains all the neccessary imports for shell.c to run smoothly.
```

#### Makefile
```
This makefile is for creating the shell.o and moving it to the bin folder so that later on it can be linked with main.o
```
### main.c
```
It is used to run the shell_loop function from the Shellcommands folder

```
### Makefile (main)
```
This Makefile is used to compile and link shell.c, color.c, and main.c into the target binary file (bin/main) using the specified compiler and flags. The clean target is provided to remove the build artifacts.
```
### Colors
#### color.c
```
This c file is just for printing in different colours 
```

