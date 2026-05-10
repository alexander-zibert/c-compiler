// Required long arg missing at end of argv: returns '?', optopt holds 'val'.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  opterr = 0;
  static struct option opts[] = {
    { "name", required_argument, (void *)0, 'n' },
    { (void *)0,              0, (void *)0,  0  },
  };
  char *argv[] = { "prog", "--name", (void *)0 };
  int argc = 2;
  int c = getopt_long(argc, argv, "n:", opts, (void *)0);
  printf("ret=%c optopt=%c\n", c, optopt);
  return 0;
}
