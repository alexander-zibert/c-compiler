// longindex output: the index of the matched longopts entry.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  static struct option opts[] = {
    { "alpha", no_argument, (void *)0, 'a' },
    { "beta",  no_argument, (void *)0, 'b' },
    { "gamma", no_argument, (void *)0, 'g' },
    { (void *)0,         0, (void *)0,  0  },
  };
  char *argv[] = { "prog", "--gamma", "--alpha", "--beta", (void *)0 };
  int argc = 4;
  int c;
  int idx = -1;
  while ((c = getopt_long(argc, argv, "abg", opts, &idx)) != -1) {
    printf("opt=%c idx=%d name=%s\n", c, idx, opts[idx].name);
  }
  return 0;
}
