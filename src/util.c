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

// char* commands_test[MAX_PROCESS_AMOUNT][MAX_PARAMS_AMOUNT];

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
        commands[i] = (char **)malloc((MAX_PARAMS_AMOUNT + 1) * sizeof(char *)); // pq + 1?
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
            commands[current_command_index][0] = strdup(token);
            // commands[current_command_index][0] = token;
            *commands_amount = *commands_amount + 1;
        }

        if (token[0] == '-')
        {
            if (current_param_index > MAX_PARAMS_AMOUNT)
            {
                printf(RED "Error: You can't type more than two parameters for each command!\nParameter " PURPLE "%s " RED "from command " PURPLE "%s " RED "will be desconsidered!\n" RESET, token, commands[current_command_index][0]);
            }
            commands[current_command_index][current_param_index] = strdup(token);
            // commands[current_command_index][current_param_index] = token;

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

int execute_processes(char ***commands, int commands_amount, __pid_t *background_processes, int *background_processes_amount, int exit)
{
    if (!strcmp("exit", commands[0][0]))
    {
        for (int i = 0; i < *background_processes_amount; i++)
        {
            killpg(background_processes[i], SIGKILL);
        }

        exit = 0;
    }

    else
    {
        if (commands_amount == 1)
        {
            execute_process_foreground(commands[0]);
        }

        else
        {
            execute_process_foreground(commands[0]);
            execute_process_background(commands, commands_amount, background_processes, background_processes_amount);
        }
    }

    return exit;
}

static void execute_process_background(char ***background_process, int commands_amount, __pid_t *background_process_ids, int *background_processes_amount)
{
    pid_t session_leader = fork();

    if (session_leader < 0)
    {
        printf(RED "Error: creation of processes in background!\n" RESET);
        exit(1);
    }

    else if (session_leader == 0)
    {
        setpgid(0, getpid());

        if (commands_amount == 2)
        {
            exec_process_aux(background_process[1]);
        }

        else if (commands_amount == 3)
        {
            exec_process_aux(background_process[1]);
            exec_process_aux(background_process[2]);
        }

        else if (commands_amount == 4)
        {
            exec_process_aux(background_process[1]);
            exec_process_aux(background_process[2]);
            exec_process_aux(background_process[3]);
        }

        else if (commands_amount == 5)
        {
            exec_process_aux(background_process[1]);
            exec_process_aux(background_process[2]);
            exec_process_aux(background_process[3]);
            exec_process_aux(background_process[4]);
        }

        pid_t pid;
        int status;
        while (pid)
        {
            pid = waitpid(0, &status, 0);

            if (WIFSIGNALED(status))
            {
                // printf("FILHO MORREU COM SINAL %d\n", WTERMSIG(status));
                killpg(session_leader, WTERMSIG(status));
            }
        }
    }

    else
    {
        background_process_ids[*background_processes_amount] = session_leader;
        (*background_processes_amount)++;
    }
}

static void exec_process_aux(char **background_process)
{
    pid_t child = fork();

    if (child < 0)
    {
        printf(RED "ERROR ON BACKGROUND PROCESS CREATION!\n" RESET);
    }

    else if (child == 0)
    {
        char path[2000];

        sprintf(path, "/bin/%s", background_process[0]);

        __pid_t consciouness = fork();

        int devnull = open("/dev/null", O_RDWR);
        dup2(devnull, STDIN_FILENO);
        dup2(devnull, STDOUT_FILENO);

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

        printf(RED "ERROR ON FOREGROUND EXECUTION!\n" RESET);
        exit(1);
    }

    int status;
    waitpid(pid, &status, 0);
}

void signal_handler()
{
}