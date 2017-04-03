#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

int compare_buffers(Buffer a, Buffer b) {
  if (a == b) return 1;
  else if (a == NULL || b == NULL) return 0;
  else if (a->size != b->size) return 0;
  else if (memcmp(a->content, b->content, a->size) == 0) return 1;
  else return 0;
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

  return diff;
}
