#include <stdio.h>
#include <stdlib.h>
#include "../include/process.h"

#define MAX_PROCESS_AMOUNT 5

struct Process
{
    char *command;
    char **flags;
    unsigned short flags_amount;
};

/**
 * @brief Memory allocation of a single process
 *
 * @return Process*
 */
Process *process_allocate()
{
    Process *process = (Process *)malloc(sizeof(Process));

    process->flags = (char **)malloc(sizeof(char *));
    process->flags_amount = 0;

    return process;
}

/**
 * @brief Memory allocation of a vector of processes
 *
 * @return Process**
 */
Process **process_vector_allocate()
{
    Process **processes = (Process **)malloc(MAX_PROCESS_AMOUNT * sizeof(Process));

    for (int i = 0; i < MAX_PROCESS_AMOUNT; i++)
    {
        processes[i] = process_allocate();
    }

    return processes;
}

/**
 * @brief Freeing memory of a vector of processes
 *
 * @param process
 */
void process_free(Process *process)
{
    if (process != NULL)
    {
        if (process->flags != NULL)
        {
            for (int i = 0; i < process->flags_amount; i++)
            {
                free(process->flags[i]);
            }

            free(process->flags);
        }

        if (process->command != NULL)
        {
            free(process->command);
        }

        free(process);
    }
}

/**
 * @brief Setting the main command of the process
 *
 * @param process
 * @param command
 */
void process_set_command(Process *process, char *command)
{
    process->command = command;
}

/**
 * @brief Setting a flag to the process
 *
 * @param process
 * @param flag
 */
void process_set_flag(Process *process, char *flag)
{
    if (process->flags_amount > 0)
    {
        process->flags = (char **)realloc(process->flags, (process->flags_amount + 1) * sizeof(char *));
    }

    process->flags[process->flags_amount] = flag;

    process->flags_amount++;
}

/**
 * @brief Printing the vector of processes to see if all is running right
 *
 * @param processes
 */
void process_print_vector(Process **processes)
{
    for (int i = 0; i < MAX_PROCESS_AMOUNT; i++)
    {
        printf("PROCESS %d: %s; FLAGS: ", i, processes[i]->command);

        for (int j = 0; j < processes[i]->flags_amount; j++)
        {
            printf("%s ", processes[i]->flags[j]);
        }

        printf("\n");
    }
}

char *process_get_name(Process *process)
{
    return process->command;
}

void process_vector_free(Process **processes)
{
    if (processes != NULL)
    {
        for (int i = 0; i < MAX_PROCESS_AMOUNT; i++)
        {
            process_free(processes[i]);
        }

        free(processes);
    }
}