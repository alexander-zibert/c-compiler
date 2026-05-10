// Two no-argument long options with distinct return values via 'val'.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  static struct option opts[] = {
    { "help",    no_argument, (void *)0, 'h' },
    { "verbose", no_argument, (void *)0, 'v' },
    { (void *)0,           0, (void *)0,  0  },
  };
  char *argv[] = { "prog", "--help", "--verbose", (void *)0 };
  int argc = 3;
  int c;
  while ((c = getopt_long(argc, argv, "hv", opts, (void *)0)) != -1) {
    printf("opt=%c\n", c);
  }
  printf("optind=%d\n", optind);
  return 0;
}
