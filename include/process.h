#ifndef PROCESS_H
#define PROCESS_H

typedef struct Process Process;

Process *process_allocate();
Process **process_vector_allocate();
void process_set_command(Process *process, char *command);
void process_set_flag(Process *process, char *flag);
void process_print_vector(Process **processes);
void process_free(Process *process);

#endif