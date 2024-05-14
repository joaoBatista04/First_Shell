#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
    *commands_amount = i;

    return process_vector;
}

void shell_run_processes(Process **commands, int commands_amount)
{
}