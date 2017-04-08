#ifndef __SPAWN_H
#define __SPAWN_H

/**
 * @brief Execute a given command, and return a file descriptor of its output
 *
 * The child process will send a SIGUSR1 if the execvp failed to the parent
 *
 * @param[in] command The command to run
 * @return The file descriptor. -1 if something failed
 */
int spawn(char *const command[]);

#endif
