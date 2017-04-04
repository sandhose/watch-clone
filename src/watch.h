#ifndef __WATCH_H
#define __WATCH_H

#include "buffer.h"

typedef struct s_watcher {
  Buffer last_output;
  int last_status;
  int run_count;
  char ** command;
} * Watcher;

Watcher create_watcher(char * command[]);

int run_watcher(Watcher);

void free_watcher(Watcher);

int run_loop(Watcher watcher, char* format, int interval, int limit, int status);
#endif
