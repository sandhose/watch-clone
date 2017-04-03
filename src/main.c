#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"
#include "util.h"
#include "watch.h"

void usage(char prog_name[]) {
  dprintf(2,
      "Usage: "
    "\n %s [-t format] [-i interval] [-l limit] [-c] command [args...]"
    "\n"
    "\nOptions:"
    "\n  -t format	TODO"
    "\n  -f interval	TODO"
    "\n  -l limit	TODO"
    "\n  -c		TODO"
    "\n", prog_name);
}

int main(int argc, char * argv[]) {
  char * prog_name = argv[0];

  char * opt_format = NULL;
  int opt_interval = 10000;
  int opt_limit = 0;
  int opt_flag_check_status = 0;

  char ch;
  while((ch = getopt(argc, argv, "t:f:l:c")) != -1) {
    switch (ch) {
      case 't':
        opt_format = optarg;
        break;
      case 'f':
        opt_interval = atoi(optarg);
        break;
      case 'l':
        opt_limit = atoi(optarg);
        break;
      case 'c':
        opt_flag_check_status = 1;
        break;
      case 'h':
      default:
        usage(prog_name);
        return 1;
    }
  }

  argc -= optind;
  argv += optind;

  if (argc == 0) {
    usage(prog_name);
    return 1;
  }

  Watcher w = create_watcher(argv);

  int ret1 = run_watcher(w);
  print_buffer(w->last_output);
  int ret2 = run_watcher(w);
  print_buffer(w->last_output);
  int ret3 = run_watcher(w);
  print_buffer(w->last_output);

  printf("%d, %d, %d\n", ret1, ret2, ret3);
  free_watcher(w);
}
