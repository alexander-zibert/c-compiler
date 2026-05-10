// Long option with non-NULL flag: getopt_long stores 'val' through flag and
// returns 0 (the option-recognized signal).
#include <getopt.h>
#include <stdio.h>

int main(void) {
  int verbose_flag = 0;
  int debug_flag = 0;
  static struct option opts[] = {
    { "verbose", no_argument, (void *)0,           1 },  // val placeholders, real wiring below
    { "debug",   no_argument, (void *)0,           2 },
    { (void *)0,           0, (void *)0,           0 },
  };
  // Wire the flag pointers at runtime (initializers can't reference locals).
  opts[0].flag = &verbose_flag;
  opts[0].val = 99;
  opts[1].flag = &debug_flag;
  opts[1].val = 77;

  char *argv[] = { "prog", "--verbose", "--debug", (void *)0 };
  int argc = 3;
  int c;
  while ((c = getopt_long(argc, argv, "", opts, (void *)0)) != -1) {
    printf("ret=%d\n", c);
  }
  printf("verbose=%d debug=%d\n", verbose_flag, debug_flag);
  return 0;
}
