#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../include/util.h"

// Global variables: the background processes ids and the amount of background processes registered
__pid_t background_groups_id[MAX_BACKGROUND_GROUPS];
int background_groups_amount = 0;

/**
 * @brief Treats possible signals (SIGTSTP or SIGINT).
 *
 * @param sig The signal received by the shell.
 */
void signal_handler(int sig)
{
    // Treatment for SIGINT
    if (sig == SIGINT)
    {
        // Blocks all signals while SIGINT is treated
        sigset_t mask;
        sigfillset(&mask);
        sigprocmask(SIG_SETMASK, &mask, NULL);

        int flag = 0;

        // Verify from background processes if there are processes still alive
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

        // If there are no processes alive, the shell is finished normally
        if (flag == 0)
        {
            printf(RED "\nNo leftover processes running, exiting...\n" RESET);
            fflush(stdout);
            exit(0);
        }

        else
        {
            // Otherwise, if there are processes alive, verify with user if he wants to proceed
            printf(RED "\nThere are still processes spawned by this shell running.\nAre you sure you want to perform this operation? [yy/nn] " RESET);
            fflush(stdout);
            fflush(stdin);
            char op = 'u';
            getchar(); // Cleans input buffer
            scanf("%c", &op);

            // If true, all processes are finished by sending a SIGKILL to them
            if (op == 'y')
            {
                printf(GREEN "\nOk, cleaning all processes..." PURPLE "\nDone, exiting... Bye!\n" RESET);
                fflush(stdout); // Guarantees that the message gets printed

                for (int i = 0; i < background_groups_amount; i++)
                {
                    if (background_groups_id[i] > 0)
                    {
                        killpg(background_groups_id[i], SIGKILL); // Kills process group
                    }
                }

                // After it, shell finishes itself
                exit(0);
            }

            // If user doesn't want to proceed, a message is shown, stdin and stdout are clened and the shell execution continues
            else
            {
                printf(PURPLE "Ok! Let's keep!\n" RESET);
                fflush(stdout);
                getchar();
                fflush(stdin);
            }
        }
    }

    // Treatmente for SIGTSTP
    if (sig == SIGTSTP)
    {
        // Print a message to warn the user
        printf(RED "\nSIGTSTP received. All processes generated by first shell will be suspended.\n" PURPLE "However, first shell won't be suspended!" RESET "\n");
        shell_print_name('n');
        fflush(stdout);

        // If a SIGTSTP is received, all processes will be suspended with SIGTSTP, except the shell
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
    // Defining a signal handler for SIGINT and SIGTSTP
    struct sigaction sa;
    sa.sa_handler = &signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);

    // Creation of auxiliary variables
    char first = 'y';
    int commands_amount = 0, exit = 1;
    char ***commands;

    // Initializing all backgroup groups ids with 0
    for (int i = 0; i < MAX_BACKGROUND_GROUPS; i++)
    {
        background_groups_id[i] = 0;
    }

    // Main loop
    while (exit)
    {
        commands_amount = 0;

        // Writing prompt to indicate thath shell is waiting commands
        shell_print_name(first);

        // Reading commands from user
        commands = shell_read_commands(first, &commands_amount);

        // Executing commands from user
        exit = execute_processes(commands, commands_amount, background_groups_id, &background_groups_amount, exit);

        // Freeing memory allocated to commands vector
        free_commands(commands);

        // Now shell is not running for the first time
        first = 'n';
    }

    return 0;
}