#include "main.h"
char prompt[50] = "minishell:~$";
char input_string[20];
int pid;
Sll *head = NULL;
void fg_bg(Sll *head, int flag);
void insert_node(char *cmd, int pid) 
{
    //Allocation of Memory for Every node
    Sll *new_node = (Sll *)malloc(sizeof(Sll));
    if (new_node == NULL)
    {
        perror("Memory allocation failed");
        exit(1);
    }
    //copy the command to node->cmd
    strcpy(new_node->cmd, cmd);
    //assign the pid value to node->pid    new_node->pid = pid;
    new_node->link = NULL;
    //if head contain null address then attach new node to null
    if (head == NULL)
    {
        head = new_node;
    }
    //attach upcoming new nodes to insert at correct position 
    else
    {
        Sll *temp = head;
        while (temp->link != NULL)
       	{
            temp = temp->link;
        }
        temp->link = new_node;
    }
}

//this function is used to free the linked list
void freeList() 
{
    Sll *temp = head;
    Sll *temp1 = head->link;
    while (temp1 != NULL)
    {
        free(temp);
        temp = temp1;
        temp1 = temp1->link;
    }
    free(temp);
}
void fg_bg(Sll *head, int flag)
{
       // Traverse to the last node
       Sll *temp = head;  // Temporary pointer to avoid modifying the original head
       while (temp->link)
       {
              temp = temp->link;
       }

       // Foreground
       if (flag)
       {
              printf("%s\n", temp->cmd);
              kill(temp->pid, SIGCONT);
              waitpid(temp->pid, NULL, WUNTRACED);
       }

       // Background
       else
       {
              printf("%s\t&\n", temp->cmd);
              kill(temp->pid, SIGCONT);
       }
}
//handler function change the behaviour of the signals
void handler(int sig)
{
   
}
/*void scan_input(char *prompt, char *input_string) 
{
    //calling the exctract_external_commands function
    extract_external_commands(external_commands);
    //super loop execute the instructions infinte times
    while (1) 
    {
	//print the prompt mini_shell $:
        printf("%s ", prompt);
        //It will read the user input
        scanf(" %[^\n]", input_string);  
	//here can change the prompt name by assigning PS1=shell
        if (strstr(input_string, "PS1") != NULL) 
	{
            if (strchr(input_string, ' ') != NULL || (input_string[4] == '='))
	    {
                printf("Command Not Found\n");
                break;
            }
	    else 
	    {
                strcpy(prompt, input_string + 4);
            }
        }
	else if (strncmp(input_string, "bg ", 3) == 0)
	{
	    int pid = atoi(input_string + 3);
	    bg_command(pid);
	}
	else if (strncmp(input_string, "fg ", 3) == 0)
	{
	    int pid = atoi(input_string + 3);
	    fg_command(pid);
	}
	//checking the user input and executing it based on internal or external
       	else 
	{
	    //no input again ,ask for the user input
            if (strlen(input_string) == 0) 
	    {
                continue;
            }
            //get the commands of first cmd
            char *command = get_command(input_string);
	    //by calling check_command_type it return command type eithet external or internal or no command
            int command_type = check_command_type(command);
	    //if command is builtin call the execute_internal_commands function by passing input as argument
            if (command_type == BUILTIN)
	    {
                execute_internal_commands(input_string);
            }
	    //if command is external call the execute_external_commands by passing input as argument
	    else if (command_type == EXTERNAL)
	    {
                execute_external_commands(input_string);
            }
	    else 
	    {
		//cmd is not present ,print no such command
                printf("No such command: %s\n", command);
            }
        }
    }
}*/
void scan_input(char *prompt, char *input_string)
{
    //calling the exctract_external_commands function
    extract_external_commands(external_commands);

    //super loop execute the instructions infinite times
    while (1)
    {
        //print the prompt mini_shell $:
        printf("%s ", prompt);

        //It will read the user input
        scanf(" %[^\n]", input_string);

        // Check if user wants to change the prompt name by assigning PS1=shell
        if (strstr(input_string, "PS1") != NULL)
        {
            if (strchr(input_string, ' ') != NULL || (input_string[4] == '='))
            {
                printf("Command Not Found\n");
                break;
            }
            else
            {
                strcpy(prompt, input_string + 4);
            }
        }
        // Checking the user input and executing it based on internal or external
        else
        {
            // If no input, ask for the user input again
            if (strlen(input_string) == 0)
            {
                continue;
            }

            // Get the first command from the input string
            char *command = get_command(input_string);

            // Check the command type (internal, external, or no command)
            int command_type = check_command_type(command);

            // If command is internal, execute internal commands
            if (command_type == BUILTIN)
            {
                // Execute internal commands like 'cd', 'exit', etc.
                execute_internal_commands(input_string);
            }
            // If command is external, execute external commands
            else if (command_type == EXTERNAL)
            {
                execute_external_commands(input_string);
            }
            // Check for foreground/background command
            else if (strstr(input_string, "fg") != NULL || strstr(input_string, "bg") != NULL)
            {
                int flag = (strstr(input_string, "fg") != NULL) ? 1 : 0;
                fg_bg(head, flag);  // Pass the external_commands list and flag
            }
            else
            {
                // Command is not recognized
                printf("No such command: %s\n", command);
            }
        }
    }
}

//Get Command Defination,used to get the first cmd of the input string
char *get_command(char *input_string) 
{
    //characte array command to store string
    char command[20];
    strcpy(command, input_string);
    //here is to return first word means first cmd in input string
    return strtok(command, " ");
}
//This defination of execut_internal_commands,this is used to execute built in commands
void execute_internal_commands(char *input_string)
{
    //If given input is exit,it will terminate the programme normally
    if (strcmp(input_string, "exit") == 0)
    {
        exit(0);
    }
    //If user input is pwd,it show the present working directory with the help of getcwd
    else if (strcmp(input_string, "pwd") == 0)
    {
        char buf[100];
        if (getcwd(buf, sizeof(buf)) == NULL)
       	{
            perror("getcwd");
        }
       	else
       	{
            printf("%s\n", buf);
        }
    }
    //With using cd can change the directory ,by the user
    else if (strncmp(input_string, "cd ", 3) == 0) 
    {
        if (chdir(input_string + 3) == -1)
       	{
            printf("-bash: cd: %s: No such file or directory\n", input_string + 2);
        } 
	else
       	{
            strcat(prompt, input_string + 2);
        }
    } 
    //if Give input as echo $$ ,it give output as present process ID
    else if (strcmp(input_string, "echo $$") == 0)
    {
        printf("The process ID is %d\n", getpid());
    }
    //Here echo with few lines of txt ,it as printf function in c
    else if (strncmp(input_string, "echo", 4) == 0)
    {
        char *str = malloc(30);
        if (str == NULL) 
	{
            perror("Memory allocation is Failed\n");
            exit(1);
        }
	//calling the echo function to print the extra text that is add with echo
        echo(input_string, str);
        free(str);
    } 
    //To check environment variable is present or not
    else if (strcmp(input_string, "echo $SHELL") == 0)
    {
        char *env = getenv("SHELL");
        if (env) 
	{
            printf("%s\n", env);
        }
       	else 
	{
            printf("Environment variable SHELL not found.\n");
        }
    }
}
//Echo function command defination
void echo(char *input_string, char *str) 
{
    //copy the user input to str for modifying
    strcpy(str, input_string);
    //Declared array of character pointers
    char *args[10];
    //at first index echo memeory location pointer is present
    args[0] = "echo";
    int i = 3;
    //After echo key ,neglet the space by moving the pointer index forward
    while (str[i++] == ' ');
    //check after the space any double quotation are present
    if (str[i] == '"')
    {
        i++;
	//store at position 1 in array pointer from str+i
        args[1] = str + i;
        while (str[i++] == '"');
        str[i] = '\0';
    } 
    else
    {
	//store str+i starting address ,it in pointer at positon 1 in array
        args[1] = str + i;
    }
    args[2] = NULL;
    //call the fork to create child
    pid = fork();
    if (pid == 0) 
    {
	//execute the excevpe by paasing args[0] which echo and starting address of args
        if (execvp(args[0], args) == -1) 
	{
            perror("Execvp Failed\n");
        }
    } 
    else 
    {
	//use wait until child resource to be cleared ,to stop child become a zombie or orphan process
        wait(NULL);
    }
}
//Defination foe execute_external_commands and input argument as input_string
void execute_external_commands(char *input_string) 
{
    //allocate memory dynamically and return address of memeory to store in char *str pointer
    char *str = malloc(30);
    //check if memory is allocated or not
    if (str == NULL)
    {
        perror("Memory Allocation Failed\n");
        exit(1);
    }
    //copy the input_string to str pointer
    strcpy(str, input_string);
    //check if input_string contain any pipe sysmbols,if true execute the instructions inside the if loop
    if (strchr(input_string, '|') != NULL)
    {
	//declare a array of character pointers to store each cmd in input string
        char *cmds[20];
	//Extract the commands on separate without includeing pipe symbol,by using strtok
        char *cmd = strtok(str, "|");
	//count is declared for giving position in cmds ,for stroing every cmds without pipe
        int count = 0;
	//run while loop and store all the cmds
        while (cmd != NULL) 
	{

            cmds[count++] = cmd;
	    //After calling first time strtok need to pass null and delimeter for separating the each token based pipe symbol
            cmd = strtok(NULL, "|");
        }
	//at last need to store NULL 
        cmds[count] = NULL;
        //Declare the pipefds array which contain pipe file descriptors,size of array based on count
        int pipefds[2 * (count - 1)];
	//create the pipes by passing pipefds base address to the pipe() function,every time calling it create two pipe file desc        riptor
	//run loop based on cmds count on either sides of the pipe
        for (int i = 0; i < count - 1; i++)
       	{
	    //if pipe function return -1 means pipes creation is failed,else it success
            if (pipe(pipefds + i * 2) == -1)
	    {
                perror("Pipe creation failed");
                exit(1);
            }
        }
        //create child process ,the count of child processes  is based on numbers of count of group of commands 
        for (int i = 0; i < count; i++)
       	{
	    //create a child process ,by calling fork() function
            pid = fork();
            if (pid == 0)
	    {
		//call insert_node function to store cmds on each node at every child processs execute
		insert_node(cmds[i],getpid());
		//This signal is default,pess ctrl+c terminate the running process return to the promp
                //signal(SIGINT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
                //Declare a array of character pointer that stores,each individual commands from cmds array of pointers
                char *args[10];
		//by using strtok ,we can extract each individual commands
                char *arg = strtok(cmds[i], " ");
                int j = 0;
		//by running while loop can extract multiple commands
                while (arg != NULL)
	       	{
                    args[j++] = arg;
                    arg = strtok(NULL, " ");
                }
		//at last need to store NULL,because it required when executing through execvp
                args[j] = NULL;
             //below are the instructions that are used in every pipe ,which pipefd is closed and openend for writing and read
                if (i > 0)
	       	{
                    if (dup2(pipefds[(i - 1) * 2], 0) == -1)
		    {
                        perror("dup2 failed for input");
                        exit(1);
                    }
                }

                if (i < count - 1) 
		{
                    if (dup2(pipefds[i * 2 + 1], 1) == -1) 
		    {
                        perror("dup2 failed for output");
                        exit(1);
                    }
                }

                for (int k = 0; k < 2 * (count - 1); k++) 
		{
                    close(pipefds[k]);
                }
                //Here execvp is used to execute the command by passing arguments througn args arrya,it will execute all the comm                commands in child process
                if (execvp(args[0], args) == -1)
	       	{
                    perror("Execvp failed");
                    exit(1);
                }
            } 
	    //If child process is not created pid would be less than 0
	    else if (pid < 0)
	    {
                perror("Fork failed");
		//exit(1) is for abnormal termination
                exit(1);
            }
	    
        }
	//behaviour of signal SIGINT ctrl+c changed to handle handler,press ctrl+c handler function will execute
        signal(SIGINT, handler);
	//press ctrl+z just pause the process,it will not terminate
        signal(SIGTSTP, handler);
        for (int i = 0; i < 2 * (count - 1); i++)
       	{
	    //close all the pipe file descriptors which are not used in parent proccess
            close(pipefds[i]);
        }
        //run for loop until all the child process resources is cleared by using wait ,times of child process
        for (int i = 0; i < count; i++) 
	{
            wait(NULL);
        }
	//call the print_linked_list to print the values inside the list
	//print_linked_list();
    } 
    //here can execute the input string which doesnt contain pipe | symbol
    else
    {
	//use array of character pointers to store individuls commands,
        char *args[15];
	//by the help of strtok can separate each cmds based on space berween them ,using space as a delimeter
        args[0] = strtok(str, " ");
        int i = 1;
        char *arg = strtok(NULL, " ");
	//use while loop for mul to cmds
        while (arg != NULL)
       	{
            args[i++] = arg;
            arg = strtok(NULL, " ");
        }
        args[i] = NULL;
        //call the fork to create a child for executing cmds using execvp
        pid = fork();
        if (pid == 0)
       	{
	    //execvp fails it return -1 else it will execute cmds
            if (execvp(args[0], args) == -1) 
	    {
                perror("Error executing command");
                exit(1);
            }
        }
	//if creating child failed pid is less than -1
       	else if (pid < 0) 
	{
            perror("Fork failed");
            exit(1);
        } 
	else
       	{
	    //press ctrl+c ,execute handler function
            signal(SIGINT, handler);
	    //press ctrl+z ,pause the current running process
            signal(SIGTSTP, handler);
	    //insert_node function is called and stored the pid of child
            insert_node(input_string, pid);
           // print_linked_list();
	    //wait is used to clear the resources of child
            wait(NULL);
        }
    }
}
//Defination of check_command_type,which is used to check cmds are built in or external
int check_command_type(char *command)
{
    //by running loop to check builtin array conatain cmds which matche with input command
    for (int i = 0; builtins[i] != NULL; i++) 
    {
	//if command is matched ,it return BUILTIN,macro is header file
        if (strcmp(command, builtins[i]) == 0) 
	{
            return BUILTIN;
        }
    }
    //by running llop to check external_commands array conatain cmds which match with input command
    for (int i = 0; external_commands[i] != NULL; i++) 
    {
	//if command is matched,it return EXTERNAL
        if (strcmp(command, external_commands[i]) == 0) 
	{
            return EXTERNAL;
        }
    }
    //if cmds not external or internal it return NO_COMMAND
    return NO_COMMAND;
}
//Defination of extract_external_commands
void extract_external_commands(char **external_commands)
{
    //using system call function open,try to open the command.txt file in read-only mode
    int fd = open("command.txt", O_RDONLY);
    //if file not opened it return -1 to fd
    if (fd == -1)
    {
	//if fd==-1 ,print error in opening file
        perror("Error opening file");
        return;
    }
    //Allocate memory dynamically using malloc and return the address to the character pointer to store string
    char *line = malloc(16);
    //if memory not alloacted it return null to pointer
    if (!line) 
    {
        perror("Memory allocation failed");
        exit(1);
    }
    char ch;
    //declare the row and col=0,for storing characters based on col indicate size of string and row size of external_commands
    int row = 0, col = 0;
    //by using the read extract every character from txt file
    while (read(fd, &ch, 1) > 0) 
    {
	//if ch=='\n' oe ch=='\0'
        if (ch == '\n' || ch == '\0')
       	{
	    //at last of line array store null character to make it as a string
            line[col] = '\0';
	    //cmds that to stroe in external_commands is less than 160 check condition when adding to array
            if (row < 160) 
	    {
               //allocate memory for every pointer in array to store address of each cmds
                external_commands[row] = malloc(16);
                if (external_commands[row] == NULL)
	       	{
                    perror("Memory allocation Failed for Array of Character Pointers:\n");
                    exit(1);
                }
		//copy the stored commands in line into pointers in external_commands array
                strcpy(external_commands[row], line);
		//increment the count of row
                row++;
            }
	    //col to make to 0 as it need for extract every cmds
            col = 0;
        }
       	else 
	{
	    //strore each character to make it a string 
            line[col++] = ch;
        }
    }
    //free the memory allocated for pointer line
    free(line);
    //close the file descriptor fd
    close(fd);
}

int main() 
{
    system("clear");
    //press of ctrl+c execute the handler function
    signal(SIGINT, handler);
    //press of ctr+z pause the current running process
    signal(SIGTSTP, handler);
    //scan_input call to pass prompt and input_string
    scan_input(prompt, input_string);
    return 0;
}



