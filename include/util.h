#ifndef UTIL_H
#define UTIL_H

#define MAX_BACKGROUND_GROUPS 1000

#define GREEN "\033[1;32m"
#define PURPLE "\033[35m"
#define RESET "\033[0m"
#define RED "\033[0;31m"

void shell_print_name(char first);
char ***shell_read_commands(char first, int *commands_amount);
void free_commands(char ***commands);

int execute_processes(char ***commands, int commands_amount, __pid_t *background_processes, int *background_processes_amount, int exit);
static void execute_process_foreground(char **foreground_process, pid_t group_id, __pid_t *background_processes_ids, int *background_processes_amount);
pid_t execute_process_background(char ***background_process, int commands_amount, __pid_t *background_process_ids, int *background_processes_amount);
static void exec_process_aux(char **background_process);

void signal_prevent();

#endif