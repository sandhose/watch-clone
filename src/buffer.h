#ifndef __BUFFER_H
#define __BUFFER_H

#define BUF_SIZE 1024

typedef struct s_buffer {
  int size;
  char content[BUF_SIZE];
  struct s_buffer * next;
} * Buffer;

Buffer create_buffer();

void free_buffer(Buffer);

int print_buffer(Buffer);

int compare_buffers(Buffer a, Buffer b);

Buffer read_to_buffer(int fd);

#endif
