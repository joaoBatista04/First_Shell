#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../include/util.h"

int main(int argc, char *argv[])
{
    char first = 'y';
    int commands_amount = 0, exit = 1;
    char ***commands;
    __pid_t background_groups_id[MAX_BACKGROUND_GROUPS];
    int background_groups_amount = 0;

    while (exit)
    {
        commands_amount = 0;

        shell_print_name(first);

        commands = shell_read_commands(first, &commands_amount);

        exit = execute_processes(commands, commands_amount, background_groups_id, &background_groups_amount, exit);

        free_commands(commands);

        first = 'n';
    }

    return 0;
}