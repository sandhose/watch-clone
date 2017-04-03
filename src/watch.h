#ifndef __WATCH_H
#define __WATCH_H

#include "buffer.h"

typedef struct s_watcher {
  Buffer last_output;
  int last_status;
  char ** command;
} * Watcher;

Watcher create_watcher(char * command[]);

int run_watcher(Watcher);

void free_watcher(Watcher);
#endif
