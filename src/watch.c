#include <stdio.h>
#include "watch.h"

int watch(char * command[], char * format, int interval, int limit, int check_status) {
  printf("calling watch([");
  for (int i = 0; command[i] != NULL; i++) {
    printf("%s, ", command[i]);
  }
  printf("], %s, %d, %d, %d)\n", format, interval, limit, check_status);
  return 0;
}
