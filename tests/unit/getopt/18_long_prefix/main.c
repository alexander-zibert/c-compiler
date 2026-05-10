// Unambiguous prefix abbreviation: --ver matches 'verbose' uniquely.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  static struct option opts[] = {
    { "verbose", no_argument, (void *)0, 'v' },
    { "help",    no_argument, (void *)0, 'h' },
    { (void *)0,           0, (void *)0,  0  },
  };
  char *argv[] = { "prog", "--ver", "--he", (void *)0 };
  int argc = 3;
  int c;
  while ((c = getopt_long(argc, argv, "vh", opts, (void *)0)) != -1) {
    printf("opt=%c\n", c);
  }
  return 0;
}
