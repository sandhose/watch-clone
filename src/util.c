#include <stdio.h>
#include <time.h>

#include "util.h"

void print_time(const char * format) {
  char buf[MAX_DATE_SIZE];
  time_t cur_time;
  struct tm * tm;

  time(&cur_time);
  tm = localtime(&cur_time);

  strftime(buf, MAX_DATE_SIZE, format, tm);
  puts(buf);
}
