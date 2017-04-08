#ifndef __BUFFER_H
#define __BUFFER_H

#define BUF_SIZE 1024

typedef struct s_buffer {
  int size;
  char content[BUF_SIZE];
  struct s_buffer *next;
} *Buffer;

/**
 * @brief Allocate a new buffer structure
 *
 * @return The newly created buffer. NULL if the creation failed
 */
Buffer create_buffer(void);

/**
 * @brief Recursivly free a buffer
 */
void free_buffer(Buffer);

/**
 * @brief Print a buffer to stdout
 *
 * @return -1 if `write` failed`
 */
int print_buffer(Buffer);

/**
 * @brief Compare two buffers
 *
 * Note: the comparaison is not recursivly done
 *
 * @param[in] a The first buffer
 * @param[in] b The second buffer
 * @return 1 if the buffers are equal, 0 otherwise
 */
int compare_buffers(Buffer a, Buffer b);

/**
 * @brief Read from a file descriptor to a buffer
 *
 * @return The buffer. NULL if nothing was read.
 */
Buffer read_to_buffer(int fd);

#endif
