#ifndef __UTIL_H
#define __UTIL_H

#define TRY(VAL) \
  if ((VAL) == -1) { \
    if (!errsrc) errsrc = #VAL; \
    return -1; \
  }

#define MAX_DATE_SIZE 255

extern char *errsrc;

int print_time(const char *format);

char *geterr(void);

#endif
