#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "spawn.h"
#include "util.h"

int spawn(char * const command[]) {
  int fildes[2];
  pid_t pid;

  ASSERT(pipe(fildes));
  ASSERT(pid = fork());

  if (pid == 0) {
    ASSERT(close(0));
    ASSERT(close(1));
    ASSERT(close(fildes[0]));
    ASSERT(dup2(fildes[1], 1));
    ASSERT(close(fildes[1]));
    execvp(command[0], command);
    perror("spawn");
    kill(getppid(), SIGUSR1);
    exit(EXIT_FAILURE);
  }

  ASSERT(close(fildes[1]));
  return fildes[0];
}
