#include <stdio.h>
#include <time.h>

#include "util.h"

char *errsrc = NULL;

int print_time(const char *format) {
  char buf[MAX_DATE_SIZE];
  time_t cur_time;
  struct tm *tm;

  TRY(time(&cur_time));
  tm = localtime(&cur_time);

  strftime(buf, MAX_DATE_SIZE, format, tm);
  TRY(puts(buf));
  return 0;
}

char *geterr(void) {
  char *e = errsrc;
  errsrc = NULL;
  return e;
}
