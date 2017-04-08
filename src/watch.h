#ifndef __WATCH_H
#define __WATCH_H

#include "buffer.h"

typedef struct s_watcher {
  Buffer last_output;
  int last_status;
  int run_count;
  int exec_failure;
  char **command;
} *Watcher;

/**
 * @brief Create a watcher for a given command
 *
 * @param[in] command The command for this watcher
 * @return The newly created watcher. NULL if the allocation failed.
 */
Watcher create_watcher(char *command[]);

/**
 * @brief Run a watcher once, and checks for change
 *
 * @param[in,out] w The watcher to run.
 * @return 1 if something changed, 0 if not. -1 if something failed
 */
int run_watcher(Watcher w);

/**
 * @brief Free a watcher
 *
 * @param w The watcher to free
 */
void free_watcher(Watcher w);

/**
 * @brief Run a watcher in loop
 *
 * @param watcher The watcher to run
 * @param format If set, display the time in this format before each run
 * @param interval The time to wait between each run, in milliseconds
 * @param limit The maximum number of run (0 = unlimited)
 * @param status If true, also check for exit status changes
 *
 * @return -1 if something failed
 */
int run_loop(Watcher watcher, char *format, int interval, int limit,
             int status);
#endif
