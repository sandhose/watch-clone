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
  w->last_output = create_buffer();
  w->last_status = 0;
  w->run_count = 0;
  w->command = command;
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

  close(fd);

  wait(&stat);
  if (WIFEXITED(stat)) {
    w->last_status = WEXITSTATUS(stat);
  }

  w->run_count++;

  if (check_status && w->last_status != prev_status) {
    return 1;
  }

  return diff;
}

int run_loop(Watcher w, char* format, int interval, int limit, int status) {
  while(limit == 0 || w->run_count < limit){
    if(format)
      print_time(format);
    if(run_watcher(w, status)) {
      print_buffer(w->last_output);
      if (status) {
        printf("exit %d\n", w->last_status);
        fflush(stdout);
      }
    }
    usleep(interval * 1000);
  }
  return 0;
}
