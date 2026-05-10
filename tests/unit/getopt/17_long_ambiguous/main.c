// Ambiguous prefix between two long options: --rad matches both 'radial'
// and 'radio'. Should report ambiguity and return '?'.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  opterr = 0;
  static struct option opts[] = {
    { "radial", no_argument, (void *)0, 'a' },
    { "radio",  no_argument, (void *)0, 'b' },
    { (void *)0,           0, (void *)0,  0 },
  };
  char *argv[] = { "prog", "--rad", (void *)0 };
  int argc = 2;
  int c = getopt_long(argc, argv, "ab", opts, (void *)0);
  printf("ret=%c\n", c);
  return 0;
}
