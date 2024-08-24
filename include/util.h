#ifndef UTIL_H
#define UTIL_H

void shell_print_name(char first);
char ***shell_read_commands(char first, int *commands_amount);
void free_commands(char ***commands);

int execute_processes(char ***commands, int commands_amount, int exit);

static void execute_process_foreground(char **foreground_process);
static void execute_process_background(char **background_process);

#endif