#include <stdio.h>
#include <time.h>

#include "util.h"

int print_time(const char * format) {
  char buf[MAX_DATE_SIZE];
  time_t cur_time;
  struct tm * tm;

  ASSERT(time(&cur_time));
  tm = localtime(&cur_time);

  strftime(buf, MAX_DATE_SIZE, format, tm);
  ASSERT(puts(buf));
  return 0;
}
