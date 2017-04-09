#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>

#include "buffer.h"
#include "watch.h"
#include "spawn.h"
#include "util.h"

static struct sigaction old_action;
static Watcher installed_watcher = NULL;

// The SIGUSR1 signal is used to report any failure in the child process
void sigusr_handler(int signum) {
  if (signum == SIGUSR1 && installed_watcher != NULL)
    installed_watcher->exec_failure = 1;
}

void install_signal(Watcher w) {
  struct sigaction s;
  s.sa_handler = sigusr_handler;
  sigemptyset(&s.sa_mask);
  s.sa_flags = 0;
  sigaction(SIGUSR1, &s, &old_action);
  installed_watcher = w;
}

void restore_signal(void) {
  installed_watcher = NULL;
  sigaction(SIGUSR1, &old_action, NULL);
}

Watcher create_watcher(char *command[]) {
  Watcher w;
  TRY_ALLOC(w = malloc(sizeof (struct s_watcher)));
  w->last_output = NULL;
  w->last_status = 0;
  w->run_count = 0;
  w->exec_failure = 0;
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
  int diff = 0;

  // This forces to consider the output as different if it is the first run, or
  // if the previous run failed
  if (w->run_count == 0 || w->exec_failure)
    diff = 1;

  // Install the SIGUSR1 signal handler
  install_signal(w);
  w->exec_failure = 0;
  TRY(fd = spawn(w->command));

  // This points where "next buffer" is
  Buffer *next_ptr = &w->last_output;
  Buffer read_buffer;

  // This is a do…while loop to force buffer comparison even if the command
  // doesn't output anything
  do {
    read_buffer = read_to_buffer(fd);
    // The `!diff &&` is here to avoid comparing the buffers if a difference
    // was already detected
    if (!diff && !compare_buffers(read_buffer, *next_ptr)) {
      diff = 1;
    }

    if (diff) {
      free_buffer(*next_ptr);
      *next_ptr = read_buffer;
    }
    else {
      free_buffer(read_buffer);
    }

    if (*next_ptr)
      next_ptr = &((*next_ptr)->next);
  } while (read_buffer != NULL);

  TRY(close(fd));

  // Save the last status code
  TRY(wait(&stat));
  if (WIFEXITED(stat)) {
    w->last_status = WEXITSTATUS(stat);
  }

  restore_signal();

  w->run_count++;

  return diff;
}

int run_loop(Watcher w, char *format, int interval, int limit, int check_status) {
  int prev_status = -1;
  int changed = 0;
  while (limit == 0 || w->run_count < limit) {
    if (format)
      TRY(print_time(format));

    TRY(changed = run_watcher(w));

    // Do not display anything if the command failed to run
    // The child is already displaying an error on stderr
    if (!w->exec_failure) {
      if (changed)
        print_buffer(w->last_output);

      if (check_status && prev_status != w->last_status) {
        printf("exit %d\n", w->last_status);
        // This flush is needed to immediately print the exit status
        fflush(stdout);
      }

      prev_status = w->last_status;
    }

    TRY(usleep(interval * 1000));
  }

  return 1;
}
