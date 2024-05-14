#include <stdio.h>
#include <unistd.h>
#include "../include/process.h"
#include "../include/util.h"

int main(int argc, char *argv[])
{
    int commands_amount;
    Process **processes;
    char first = 'y';

    while (1)
    {
        shell_print_name(first);
        first = 'n';

        processes = shell_read_commands(&commands_amount);

        process_print_vector(processes);

        shell_run_processes(processes, commands_amount);
    }

    return 0;
}