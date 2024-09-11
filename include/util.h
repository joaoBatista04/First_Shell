#ifndef UTIL_H
#define UTIL_H

#define MAX_BACKGROUND_GROUPS 1000

#define GREEN "\033[1;32m"
#define PURPLE "\033[35m"
#define RESET "\033[0m"
#define RED "\033[0;31m"

/**
 * @brief Treats errors on signal receptors.
 */
void signal_prevent();

/**
 * @brief Prints the shell's basic structure.
 *
 * @param first A flag that indicates if it's the first time this function is executed.
 */
void shell_print_name(char first);

/**
 * @brief Destroys the vector that storages the shell commands.
 *
 * @param commands The vector that storages the shell commands.
 */
void free_commands(char ***commands);

/**
 * @brief Creates the environment required and executes one background process.
 *
 * @param brackground_process The process to be executed in background.
 */
static void exec_process_aux(char **background_process);

/**
 * @brief Reads and stores the shell commands.
 *
 * @param first A flag that indicates if it's the first time this function is executed.
 * @param commands_amount The amount of commands written.
 * 
 * @returns The vector that storages the shell commands.
 */
char ***shell_read_commands(char first, int *commands_amount);

/**
 * @brief Kills all living descendants and ends shell operation.
 *
 * @param background_processes The background processes pid's vector.
 * @param background_processes_amount The number of living descendants processes.
 */
void die(pid_t *background_processes, int *background_processes_amount);

/**
 * @brief Frees all zombie descendants.
 *
 * @param background_processes The background processes pid's vector.
 * @param background_processes_amount The number of zombie descendants processes.
 */
void waitall(pid_t *background_processes, int *background_processes_amount);

/**
 * @brief Executes the commands entered by the user.
 *
 * @param commands The vector of commands entered by the user.
 * @param commands_amount The amount of commands entered by the user.
 * @param background_processes The background processes pid's vector.
 * @param background_processes_amount The number of zombie descendants processes.
 * @param exit A flag that indicates if shell operation should continue or not.
 * 
 * @returns A flag that indicates if shell operation should continue or not.
 */
int execute_processes(char ***commands, int commands_amount, __pid_t *background_processes, int *background_processes_amount, int exit);

/**
 * @brief Creates the environment required and executes background processes.
 *
 * @param brackground_process The vector that storages the processes to be executed in background.
 * @param commands_amount The amount of commands to be executed in background.
 * @param background_process_ids The background processes id's.
 * @param background_processes_amount The number of background processes to be executed.
 * 
 * @returns 
 */
pid_t execute_process_background(char ***background_process, int commands_amount, __pid_t *background_process_ids, int *background_processes_amount);

/**
 * @brief Creates the environment required and executes foreground processes.
 *
 * @param foreground_process The vector that storages the processes to be executed in foreground.
 * @param group_id The id from the processes group.
 * @param background_processes_ids The background processes id's.
 * @param background_processes_amount The number of background processes to be executed.
 */
static void execute_process_foreground(char **foreground_process, pid_t group_id, __pid_t *background_processes_ids, int *background_processes_amount);

#endif