#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "buffer.h"

Buffer create_buffer() {
  Buffer b = malloc(sizeof(struct s_buffer));
  b->size = 0;
  b->content = calloc(BUF_SIZE, sizeof(char));
  b->next = NULL;
  return b;
}

void free_buffer(Buffer buffer) {
  if (buffer->next)
    free_buffer(buffer->next);
  free(buffer->content);
  free(buffer);
}

int compare_buffers(Buffer a, Buffer b) {
  if (a == b) return 1;
  else if (a == NULL || b == NULL) return 0;
  else if (a->size != b->size) return 0;
  else if (memcmp(a->content, b->content, a->size) == 0) return 1;
  else return 0;
}

void print_buffer(Buffer b) {
  if (b == NULL) return;
  write(1, b->content, b->size);
  print_buffer(b->next);
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
