#ifndef __WATCH_H
#define __WATCH_H

#define BUF_SIZE 1024

typedef struct s_buffer {
  char * content;
  struct s_buffer * next;
} * Buffer;

typedef struct s_watcher {
  Buffer last_output;
  int last_status;
  char ** command;
} * Watcher;

Watcher create_watcher(char * command[]);

int run_watcher(Watcher);

void free_watcher(Watcher);

#endif
