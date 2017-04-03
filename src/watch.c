#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "buffer.h"
#include "watch.h"
#include "spawn.h"

Watcher create_watcher(char * command[]) {
  Watcher w = malloc(sizeof(struct s_watcher));
  w->command = command;
  w->last_output = create_buffer();
  w->last_status = 0;
  return w;
}

void free_watcher(Watcher w) {
  free_buffer(w->last_output);
  free(w);
}

int run_watcher(Watcher w, int check_status) {
  int fd = spawn(w->command);
  int prev_status = w->last_status;
  int stat;

  Buffer * next_ptr = &w->last_output;
  Buffer bytes_read;

  int diff = 0;

  while((bytes_read = read_to_buffer(fd)) != NULL) {
    if (!diff && !compare_buffers(bytes_read, *next_ptr)) {
      diff = 1;
    }

    if (diff) {
      free_buffer(*next_ptr);
      *next_ptr = bytes_read;
    } else {
      free_buffer(bytes_read);
    }

    next_ptr = &(*next_ptr)->next;
  }

  close(fd);

  wait(&stat);
  if (WIFEXITED(stat)) {
    w->last_status = WEXITSTATUS(stat);
  }

  if (check_status && w->last_status != prev_status) {
    return 1;
  }

  return diff;
}
