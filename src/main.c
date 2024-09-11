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

/**
 * @brief Treats possible signals (SIGTSTP or SIGINT).
 *
 * @param sig The signal received by the shell.
 */
void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        sigset_t mask;
        sigfillset(&mask);
        sigprocmask(SIG_SETMASK, &mask, NULL);

        int flag = 0;

        for (int i = 0; i < background_groups_amount; i++)
        {
            pid_t group_id = background_groups_id[i];

            if (group_id != 0)
            {
                pid_t pid = waitpid(background_groups_id[i], NULL, WNOHANG);

                if (pid == 0)
                {
                    flag = 1;
                }
            }
        }

        if (flag == 0)
        {
            printf(RED "\nNo leftover processes running, exiting...\n" RESET);
            fflush(stdout);
            exit(0);
        }

        else
        {
            printf(RED "\nThere are still processes spawned by this shell running.\nAre you sure you want to perform this operation? [yy/nn] " RESET);
            fflush(stdout);
            fflush(stdin);
            char op = 'u';
            getchar(); // Limpa o buffer de entrada (sem isso tava dando errado n sei pq)
            scanf("%c", &op);

            if (op == 'y')
            {
                printf(GREEN "\nOk, cleaning all processes..." PURPLE "\nDone, exiting... Bye!\n" RESET);
                fflush(stdout); // Garante que a mensagem seja impressa

                for (int i = 0; i < background_groups_amount; i++)
                {
                    if (background_groups_id[i] > 0)
                    {
                        killpg(background_groups_id[i], SIGKILL); // Mata o grupo de processos
                    }
                }

                exit(0);
            }

            else
            {
                printf(PURPLE "Ok! Let's keep!\n" RESET);
                fflush(stdout);
                getchar();
                fflush(stdin);
            }
        }
    }

    if (sig == SIGTSTP)
    {
        printf(RED "\nSIGTSTP received. All processes generated by first shell will be suspended.\n" PURPLE "However, first shell won't be suspended!" RESET "\n");
        shell_print_name('n');
        fflush(stdout);
        for (int i = 0; i < background_groups_amount; i++)
        {
            if (background_groups_id[i] > 0)
            {
                killpg(background_groups_id[i], SIGSTOP);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = &signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);

    char first = 'y';
    int commands_amount = 0, exit = 1;
    char ***commands;

    for (int i = 0; i < MAX_BACKGROUND_GROUPS; i++)
    {
        background_groups_id[i] = 0;
    }

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