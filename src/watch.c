#include <stdlib.h>
#include <unistd.h>

#include "watch.h"
#include "spawn.h"

Buffer create_buffer() {
  Buffer b = malloc(sizeof(struct s_buffer));
  b->content = calloc(BUF_SIZE, sizeof(char));
  b->next = NULL;
  return b;
}

Watcher create_watcher(char * command[]) {
  Watcher w = malloc(sizeof(struct s_watcher));
  w->command = command;
  w->last_output = create_buffer();
  w->last_status = 0;
  return w;
}

void free_buffer(Buffer buffer) {
  if (buffer->next)
    free_buffer(buffer->next);
  free(buffer->content);
  free(buffer);
}

void free_watcher(Watcher w) {
  free_buffer(w->last_output);
  free(w);
}

int run_watcher(Watcher w) {
  int fd = spawn(w->command);
  char buf[BUF_SIZE];
  int ch;

  while((ch = read(fd, buf, BUF_SIZE)) > 0) {
    write(1, buf, ch);
  }

  return 0;
}
