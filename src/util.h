#ifndef __UTIL_H
#define __UTIL_H

#define ASSERT(VAL) \
  if ((VAL) == -1) { \
    return -1; \
  }

#define MAX_DATE_SIZE 255

int print_time(const char * format);

#endif
