# TinyShell
Recreation of Unix Shell

Design overview:
The parse function is used to tokenize the input line into separate arguments. It replaces all white spaces with zeros until it encounters a non-white space character,
indicating the beginning of an argument. The function then saves the address to argv[] and skips all non-white spaces, constituting the argument. 
 
The execute function is used to handle singular commands as well as multiple commands separated by semicolons. It splits the input line into separate commands and then parses and executes each command concurrently in a child process.
This is achieved by using the fork system call to create a new process for each command and the execvp system call to replace the current process with a new one. 
Additionally, the function waits for all child processes to finish before proceeding.

The main function is used to handle both interactive and batch modes. It checks the command line arguments to determine the mode of operation. If a batch file is provided, the program reads commands from the file,
otherwise, it runs in interactive mode, prompting the user for input. After reading each line, it executes the commands using the execute function.

In interactive mode, the program displays the "Shell -> " prompt before reading user input. This ensures that users are prompted for input when running the shell interactively.

Complete specification:
The "quit" command is handled within the execute function. If the "quit" command is encountered, the program exits after all other commands on the line have been executed.

Lines that have no commands between semi-colons are handled by the parse() function. All non-white space characters are ignored, and the function only adds commands to the input line when non-whitespace characters are found.

In cases where the execvp function fails to execute (improper command), an error is displayed to the user. Similarly, when forking a process fails, another error is displayed.

If an improper batch file is input into the program, or if the file fails to open, an error is displayed and the program exits. If the user utilizes the wrong syntax when inputting the file, a message is displayed showing proper input.
