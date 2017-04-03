#include <unistd.h>

#include "spawn.h"

int spawn(char * const command[]) {
  int fildes[2];
  pid_t pid;

  pipe(fildes);
  pid = fork();

  if (pid == 0) {
    close(fildes[0]);
    dup2(fildes[1], 1);
    execvp(command[0], command);
  }

  close(fildes[1]);
  return fildes[0];
}
