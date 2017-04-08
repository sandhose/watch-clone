#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "spawn.h"
#include "util.h"

int spawn(char *const command[]) {
  int fildes[2];
  pid_t pid;

  TRY(pipe(fildes));
  TRY(pid = fork());

  if (pid == 0) {
    TRY(close(0));
    TRY(close(1));
    TRY(close(fildes[0]));
    TRY(dup2(fildes[1], 1));
    TRY(close(fildes[1]));
    execvp(command[0], command);
    perror("spawn");
    kill(getppid(), SIGUSR1);
    exit(EXIT_FAILURE);
  }

  TRY(close(fildes[1]));
  return fildes[0];
}
