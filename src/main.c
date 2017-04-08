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
          "\n  -t format	if set, display the time using the given format before each run"
          "\n  -i interval	milliseconds to wait between updates"
          "\n  -l limit	maximum numbers of runs (0 means unlimitted)"
          "\n  -c		also check for exit status code changes"
          "\n\n  -h		display this help and exit\n", prog_name);
}

int main(int argc, char *argv[]) {
  char *prog_name = argv[0];

  char *opt_format = NULL;
  int opt_interval = 10000;
  int opt_limit = 0;
  int opt_flag_check_status = 0;

  char ch;
  while ((ch = getopt(argc, argv, "+t:i:l:c")) != -1) {
    switch (ch) {
      case 't':
        opt_format = optarg;
        break;
      case 'i':
        if ((opt_interval = atoi(optarg)) <= 0) {
          usage(prog_name);
          return 1;
        }
        break;
      case 'l':
        if ((opt_limit = atoi(optarg)) <= 0) {
          usage(prog_name);
          return 1;
        }
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
  if (run_loop(w, opt_format, opt_interval,
               opt_limit, opt_flag_check_status) == -1) {
    perror(geterr());
  }
  free_watcher(w);

  return 0;
}
