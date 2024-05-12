#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/util.h"

void shell_print_name(char first)
{
    if (first == 'y')
    {
        system("clear");
    }

    printf("\e[1;92mfsh\e[1;94m> \e[0m");
}

char **shell_read_commands(int *commands_amount)
{
    char line_info[2000];
    char **commands = (char **)malloc(5 * sizeof(char *));
    char *token;
    int i = 0;

    scanf("%[^\n]%*c", line_info);

    token = strtok(line_info, " ");
    while (token != NULL)
    {
        commands[i] = strdup(token);
        token = strtok(NULL, " ");
        i++;

        if (i >= 5)
        {
            break;
        }
    }

    *commands_amount = i;

    return commands;
}