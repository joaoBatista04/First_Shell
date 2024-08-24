#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../include/util.h"

int main(int argc, char *argv[])
{
    char first = 'y';
    int commands_amount = 0, exit = 1;
    char ***commands;

    while (exit)
    {
        commands_amount = 0;

        shell_print_name(first);

        commands = shell_read_commands(first, &commands_amount);

        exit = execute_processes(commands, commands_amount, exit);

        free_commands(commands);
        
        first = 'n';
    }

    return 0;
}