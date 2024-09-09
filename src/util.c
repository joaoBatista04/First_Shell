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
#define MAX_COMMANDS 3

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
        commands[i] = (char **)malloc((MAX_PARAMS_AMOUNT + 1) * sizeof(char *));
    
    if (first == 'y')
        scanf("%[^\n]", line);
    
    else
        scanf("%*c%[^\n]", line);
    

    char* commands_aux[MAX_PROCESS_AMOUNT];
    char* command = strtok(line, "#");
    int count_proc = 0;

    while (command != NULL) 
    {

        if (count_proc >= MAX_PROCESS_AMOUNT)
        {
            printf(RED "Error: You can't type more than five commands at once!\n" RESET);
            perror(RED "Aborting: more than five commands called!\n" RESET);
            abort();
        }

        // Remove espaços extras no começo do comando
        while (*command == ' ') command++;

        //printf("Comando: %s\n", command);
        commands_aux[count_proc] = strdup(command);
        
        // Avançar para o próximo comando separado por '#'
        command = strtok(NULL, "#");
        count_proc++;
    }

    
    for (int i = 0; i < count_proc; i++) 
    {
        // Agora separamos os parâmetros com ' '
        char* dup = strdup(commands_aux[i]);
        char* parameter = strtok(dup, " ");
        
        // O primeiro parametro é o comando principal, armazenamos ele na primeira posição
        if (parameter != NULL) 
        {
            //printf("Comando principal: %s\n", parameter);
            commands[i][0] = strdup(parameter);
        }
        
        int count_param = 1;
        
        // Pegamos os parâmetros com ' '
        parameter = strtok(NULL, " ");

        while (parameter != NULL) {
            //printf("Parâmetro: %s\n", parameter);

            if (count_param > MAX_PARAMS_AMOUNT)
                printf(RED "Error: You can't type more than two parameters for each command!\nParameter " PURPLE "%s " RED "from command " PURPLE "%s " RED "will be desconsidered!\n" RESET, parameter, commands[i][0]);
            
            else commands[i][count_param] = strdup(parameter);

            parameter = strtok(NULL, " ");
            count_param++;
        }
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
    if (!strcmp("die", commands[0][0]))
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