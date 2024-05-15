#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/util.h"
#include "../include/process.h"

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
    printf("\e[1;92mfsh\e[1;94m> \e[0m");
}

/**
 * @brief Reading of the buffer and tokenization of the commands and flags
 *
 * @param commands_amount
 * @return Process**
 */
Process **shell_read_commands(int *commands_amount)
{
    Process **process_vector = process_vector_allocate();
    char line_info[2000];
    char *token;
    int i = 0;

    // Buffer reading
    scanf("%[^\n]%*c", line_info);

    // Reading of the first command
    token = strtok(line_info, " ");
    while (token != NULL)
    {
        // If the token doesn't start with '-', then it is a command
        if (token[0] != '-')
        {
            process_set_command(process_vector[i], strdup(token));
        }

        // If the token starts with '-', then it is a flag
        else
        {
            process_set_flag(process_vector[i], strdup(token));
        }

        // Reading of the next token
        token = strtok(NULL, " ");

        // If the next token doesn't start with '-', then it will be a new command
        if (token)
        {
            if (token[0] != '-')
            {
                i++;
            }
        }

        // If the number of processes exceeds the maximum number of processes allowed, then the process registration ends
        if (i >= 5)
        {
            break;
        }
    }

    // The number of process is recorded
    *commands_amount = i + 1;

    return process_vector;
}

void shell_run_processes(Process **processes, int commands_amount)
{
    char base[2000] = "/bin/";
    char path[2000];

    if (commands_amount > 0)
    {
        sprintf(path, "%s%s", base, process_get_name(processes[0]));

        __pid_t pid1 = fork();

        if (pid1 == 0)
        {
            execv(path, process_get_flags(processes[0]));
            exit(1);
        }

        int status;
        waitpid(pid1, &status, 0);
    }

    if (commands_amount > 1)
    {
        sprintf(path, "%s%s", base, process_get_name(processes[1]));

        __pid_t pid2 = fork();

        if (pid2 == 0)
        {
            execv(path, process_get_flags(processes[1]));
            exit(1);
        }

        int status;
        waitpid(pid2, &status, 0);

        /*__pid_t pid2 = fork();

        if (pid2 == 0)
        {
            execv(path, process_get_flags(processes[1]));
        }*/
    }

    if (commands_amount > 2)
    {
        sprintf(path, "%s%s", base, process_get_name(processes[2]));

        /*__pid_t pid3 = fork();

        if (pid3 == 0)
        {
            execv(path, process_get_flags(processes[2]));
        }*/
    }

    if (commands_amount > 3)
    {
        sprintf(path, "%s%s", base, process_get_name(processes[3]));

        /*__pid_t pid4 = fork();

        if (pid4 == 0)
        {
            execv(path, process_get_flags(processes[3]));
        }*/
    }

    if (commands_amount > 4)
    {
        sprintf(path, "%s%s", base, process_get_name(processes[4]));

        /*__pid_t pid5 = fork();

        if (pid5 == 0)
        {
            execv(path, process_get_flags(processes[4]));
        }*/
    }
}

void shell_execute_process(Process *process)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Error on fork!\n");
        exit(-1);
    }

    if (pid == 0)
    {
        execvp(process_get_name(process), process_get_flags(process));
        perror("Exec error!\n");
        exit(EXIT_FAILURE);
    }

    else
    {
        int status;
        waitpid(pid, &status, 0);

        if (WIFSIGNALED(status))
        {
            int signal = WTERMSIG(status);

            if (signal == SIGUSR1)
            {
                kill(getppid(), SIGUSR1);
            }
        }

        exit(EXIT_SUCCESS);
    }
}