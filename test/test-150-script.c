#include <unistd.h>
#include <time.h>

/**
 * This test writes a text in two parts, and forces the kernel to flush its
 * internal buffer before writing the second part.
 * This forces to fill up the Buffer structure with two read calls
 */
int main(void)
{
  int rand = time(NULL) % 20; // Not the best random source, but HEH, it works.
  char to_write[] = "some long string that will be written in two parts, with a fsync in between";
  write(1, to_write, 10 + rand);
  fsync(1);
  sleep(1);
  write(1, to_write + 10 + rand, sizeof(to_write) - 10 - rand);
}
