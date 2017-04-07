#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "buffer.h"
#include "util.h"

Buffer create_buffer() {
  Buffer b = malloc(sizeof (struct s_buffer));
  if (b == NULL)
    return NULL;
  b->size = 0;
  b->next = NULL;
  return b;
}

void free_buffer(Buffer buffer) {
  if (!buffer)
    return;
  if (buffer->next)
    free_buffer(buffer->next);
  free(buffer);
}

int compare_buffers(Buffer a, Buffer b) {
  if (a == b)
    return 1;
  else if (a == NULL || b == NULL)
    return 0;
  else if (a->size != b->size)
    return 0;
  else if (memcmp(a->content, b->content, a->size) == 0)
    return 1;
  else
    return 0;
}

int print_buffer(Buffer b) {
  if (b == NULL)
    return 0;
  ASSERT(write(1, b->content, b->size));
  return print_buffer(b->next);
}

Buffer read_to_buffer(int fd) {
  Buffer b = create_buffer();
  if (b == NULL)
    return NULL;
  b->size = read(fd, b->content, BUF_SIZE);

  if (b->size == 0) {
    free_buffer(b);
    return NULL;
  }

  return b;
}
