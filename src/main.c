#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../include/util.h"

__pid_t background_groups_id[MAX_BACKGROUND_GROUPS];
int background_groups_amount = 0;
__pid_t fsh;

void test(int sig)
{
    printf("TESTE\n");
}

void sigint_handler(int sig)
{
    int flag = 0;
    for (int i = 0; i < background_groups_amount; i++)
    {

        __pid_t group_id = background_groups_id[i];

        if (group_id != 0)
        {
            pid_t pid = waitpid(background_groups_id[i], NULL, WNOHANG);

            if (pid == 0)
            {
                flag = 1;
            }
        }
    }

    if (flag == 1)
    {
        printf(RED "There are still processes spawned by this shell running.\nAre you sure you want to perform this operation? [y/n]\n" RESET);
        char op = 'u';
        // scanf("%c", &op);

        if (op == 'y')
        {
            printf(RED "Ok, exiting..." RESET);
            for (int i = 0; i < background_groups_amount; i++)
            {
                killpg(background_groups_id[i], SIGKILL);
            }
        }
    }

    else
    {
        printf(RED "Ok, exiting..." RESET);
        for (int i = 0; i < background_groups_amount; i++)
        {
            if (background_groups_id[i] != 0)
            {
                killpg(background_groups_id[i], SIGKILL);
            }
        }
        raise(SIGKILL);
    }
}

int main(int argc, char *argv[])
{
    fsh = getpid();

    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);

    char first = 'y';
    int commands_amount = 0, exit = 1;
    char ***commands;

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