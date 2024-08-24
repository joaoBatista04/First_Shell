#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../include/util.h"

#define GREEN "\033[1;32m"
#define PURPLE "\033[35m"
#define RESET "\033[0m"
#define RED "\033[0;31m"

#define MAX_PROCESS_AMOUNT 5
#define MAX_PARAMS_AMOUNT 2

/**
 * @brief Always executed to print the shell info
 *
 * @param first
 */
void shell_print_name(char first)
{
    // If shell execution is first, terminal is cleared
    if (first == 'y')
    {
        system("clear");
    }

    // Print buffer info
    printf(GREEN "fsh" PURPLE "> " RESET);
}

char ***shell_read_commands(char first, int *commands_amount)
{
    char line[2000];
    char ***commands = (char ***)malloc(MAX_PROCESS_AMOUNT * sizeof(char **));

    for (int i = 0; i < MAX_PROCESS_AMOUNT; i++)
    {
        commands[i] = (char **)malloc((MAX_PARAMS_AMOUNT + 1) * sizeof(char *));
    }

    if (first == 'y')
    {
        scanf("%[^\n]", line);
    }
    else
    {
        scanf("%*c%[^\n]", line);
    }

    int current_command_index = 0;
    int current_param_index = 1;
    char *token = strtok(line, " ");

    while (token != NULL)
    {
        if (token[0] != '-' && token[0] != '#' && current_command_index < MAX_PROCESS_AMOUNT)
        {
            commands[current_command_index][0] = token;
            *commands_amount = *commands_amount + 1;
        }

        if (token[0] == '-')
        {
            if (current_param_index > MAX_PARAMS_AMOUNT)
            {
                printf(RED "Error: You can't type more than two parameters for each command!\nParameter " PURPLE "%s " RED "from command " PURPLE "%s " RED "will be desconsidered!\n" RESET, token, commands[current_command_index][0]);
            }
            commands[current_command_index][current_param_index] = token;
            current_param_index++;
        }

        if (token[0] == '#')
        {
            current_command_index++;

            if (current_command_index >= MAX_PROCESS_AMOUNT)
            {
                printf(RED "Error: You can't type more than five commands at once!\n" RESET);
                perror(RED "Aborting: more than five commands called!\n" RESET);
                abort();
            }
            current_param_index = 1;
        }

        token = strtok(NULL, " ");
    }

    printf("COMMANDS AMOUNT: %d\n", *commands_amount);

    return commands;
}

void free_commands(char ***commands)
{
    for (int i = 0; i < MAX_PROCESS_AMOUNT; i++)
    {
        free(commands[i]);
    }
    free(commands);
}

int execute_processes(char ***commands, int commands_amount, int exit)
{
    for (int i = 0; i < commands_amount; i++)
    {
        if (commands[i][0] != NULL)
        {
            if (!strcmp(commands[i][0], "exit"))
            {
                exit = 0;
                break;
            }

            if (i == 0)
            {
                execute_process_foreground(commands[i]);
            }

            else
            {
                execute_process_background(commands[i]);
            }
        }
    }

    return exit;
}

static void execute_process_background(char **background_process)
{
    char path[2000];

    sprintf(path, "/bin/%s", background_process[0]);
    __pid_t pid = fork();

    if (pid == 0)
    {
        int devNull = open("/dev/null", O_WRONLY);
        int devNull2 = open("/dev/null", O_WRONLY);
        dup2(devNull, STDOUT_FILENO);
        dup2(devNull2, STDIN_FILENO);
        setpgid(0, pid);
        execv(path, background_process);
    }
}

static void execute_process_foreground(char **foreground_process)
{
    char path[2000];

    sprintf(path, "/bin/%s", foreground_process[0]);
    __pid_t pid = fork();

    if (pid == 0)
    {
        execv(path, foreground_process);
    }

    int status;
    waitpid(pid, &status, 0);
}