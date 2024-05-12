#include <stdio.h>
#include <unistd.h>
#include "../include/util.h"

int main(int argc, char *argv[])
{
    int commands_amount;
    char **commands;
    char first = 'y';

    while (1)
    {
        shell_print_name(first);
        first = 'n';
        commands = shell_read_commands(&commands_amount);
    }

    return 0;
}