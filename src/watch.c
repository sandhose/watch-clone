#include <stdlib.h>
#include <unistd.h>

#include "watch.h"
#include "spawn.h"

Buffer create_buffer() {
  Buffer b = malloc(sizeof(struct s_buffer));
  b->size = 0;
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

void putb(Buffer b) {
  if (b == NULL) return;
  write(1, b->content, b->size);
  putb(b->next);
}

Buffer read_to_buffer(int fd) {
  Buffer b = create_buffer();
  b->size = read(fd, b->content, BUF_SIZE);

  if (b->size == 0) {
    free_buffer(b);
    return NULL;
  }

  return b;
}

int run_watcher(Watcher w) {
  int fd = spawn(w->command);

  Buffer * next_ptr = &w->last_output;

  while((*next_ptr = read_to_buffer(fd)) != NULL) {
    next_ptr = &((*next_ptr)->next);
  }

  return 0;
}
