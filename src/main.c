#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../include/process.h"
#include "../include/util.h"

int main(int argc, char *argv[])
{
    int commands_amount;
    Process **processes;
    char first = 'y';
    int ends = 0;

    while (1)
    {
        // printf("PID: %d\n", getpid());
        shell_print_name(first);
        first = 'n';

        processes = shell_read_commands(&commands_amount);

        // process_print_vector(processes);

        if (!strcmp(process_get_name(processes[0]), "quit"))
        {
            break;
        }

        shell_run_processes(processes, commands_amount);

        process_vector_free(processes);
    }

    process_vector_free(processes);

    return 0;
}