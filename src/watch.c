#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#include "buffer.h"
#include "watch.h"
#include "spawn.h"
#include "util.h"

Watcher create_watcher(char * command[]) {
  Watcher w = malloc(sizeof(struct s_watcher));
  if (w == NULL) return NULL;
  w->last_output = NULL;
  w->last_status = 0;
  w->run_count = 0;
  w->command = command;
  return w;
}

void free_watcher(Watcher w) {
  free_buffer(w->last_output);
  free(w);
}

int run_watcher(Watcher w) {
  int fd;
  int stat;

  ASSERT(fd = spawn(w->command));

  Buffer * next_ptr = &w->last_output;
  Buffer bytes_read;

  int diff = 0;

  if (w->run_count == 0) diff = 1;

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

  ASSERT(close(fd));

  ASSERT(wait(&stat));
  if (WIFEXITED(stat)) {
    w->last_status = WEXITSTATUS(stat);
  }

  w->run_count++;

  return diff;
}

int run_loop(Watcher w, char* format, int interval, int limit, int check_status) {
  int prev_status = -1;
  int changed = 0;
  while(limit == 0 || w->run_count < limit){
    if(format)
      ASSERT(print_time(format));

    ASSERT(changed = run_watcher(w));
    if (changed)
      print_buffer(w->last_output);

    if(check_status && prev_status != w->last_status) {
      printf("exit %d\n", w->last_status);
      fflush(stdout);
    }

    prev_status = w->last_status;

    ASSERT(usleep(interval * 1000));
  }
  return 0;
}
