#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*typedef struct ListNode
{
       int pid;
       char cmd[256];
       struct ListNode *link;
}Sll;*/
typedef struct Sll
{
    char cmd[256]; // Ensure this is big enough for your command
    int pid;
    struct Sll *link;
} Sll;



char *external_commands[160]={NULL};
char *builtins[] = {"echo","printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};
void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

//void copy_change(char *prompt, char *input_string);

int check_command_type(char *command);
void echo(char *input_string,char *str);
void execute_internal_commands(char *input_string);
//void signal_handler(int sig_num);
void extract_external_commands(char **external_commands);
void execute_external_commands(char *input_string);

void insert_node(char *cmd, int pid);
void print_linked_list();
void handler(int sig);
void freeList();

#endif
