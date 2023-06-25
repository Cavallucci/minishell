# Minishell

Welcome to the Minishell project! This project aims to create a minimalistic shell in C language, providing a basic command interpreter. This shell allows you to execute system commands, manage processes, and includes some additional features.

## Key Features

- System command interpretation: Minishell allows you to execute commands built into the operating system, such as `ls`, `cd`, `mkdir`, `cat`, etc.
- Process management: You can run commands in the background or foreground, as well as handle signals and child processes.
- Environment variables: Minishell enables management of environment variables like `PATH` and accessing their values.
- Redirections: You can use redirection operators to redirect standard input/output of commands to files.
- Pipes: Minishell supports command chaining using pipes (`|`), allowing you to redirect the output of one command to the input of another.

## Requirements

- Operating System: Linux, macOS, or Windows (with WSL emulator)
- Compiler: GCC (GNU Compiler Collection)

## Installation

1. Clone this GitHub repository to your local machine:

    git clone https://github.com/Cavallucci/Minishell.git

2. Navigate to the project directory:

    cd Minishell

3. Compile the project using the Makefile:

    Make

4. Run the generated executable file:

    ./minishell

## Usage

Once you have executed the `minishell` file, you can start using the shell by entering commands. Here are some examples of commands you can try:

- Execute a system command:

    $ ls
    $ cat file.txt
    $ mkdir new_directory

- Change directory:

    $ cd path/to/directory

- Use environment variables:

    $ echo $PATH

- Use redirections:

    $ ls > files.txt
    $ cat < file.txt

- Use pipes:

    $ ls | grep "pattern"


For more information on the features and supported commands of Minishell, you can refer to the built-in documentation or the project's source files.


## Authors

- Marion Kralik
- Laura Cavallucci
