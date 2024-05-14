#ifndef UTIL_H
#define UTIL_H

#include "./process.h"

void shell_print_name(char first);
Process **shell_read_commands(int *commands_amount);
void shell_run_processes(Process **processes, int commands_amount);

#endif