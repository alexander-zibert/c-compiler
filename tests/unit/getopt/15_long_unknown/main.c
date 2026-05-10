// Unrecognized long option: returns '?'.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  opterr = 0;
  static struct option opts[] = {
    { "help",  no_argument, (void *)0, 'h' },
    { (void *)0,         0, (void *)0,  0  },
  };
  char *argv[] = { "prog", "--help", "--bogus", "--help", (void *)0 };
  int argc = 4;
  int c;
  while ((c = getopt_long(argc, argv, "h", opts, (void *)0)) != -1) {
    if (c == '?') printf("err\n");
    else printf("opt=%c\n", c);
  }
  printf("optind=%d\n", optind);
  return 0;
}
