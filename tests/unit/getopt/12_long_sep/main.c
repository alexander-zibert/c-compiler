// Long option with argument in the next argv element: --name value.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  static struct option opts[] = {
    { "count",   required_argument, (void *)0, 'c' },
    { (void *)0,                 0, (void *)0,  0  },
  };
  char *argv[] = { "prog", "--count", "42", (void *)0 };
  int argc = 3;
  int c;
  while ((c = getopt_long(argc, argv, "c:", opts, (void *)0)) != -1) {
    printf("opt=%c arg=%s\n", c, optarg);
  }
  printf("optind=%d\n", optind);
  return 0;
}
