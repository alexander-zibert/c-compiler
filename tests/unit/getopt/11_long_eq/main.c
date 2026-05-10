// Long option with --name=value form.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  static struct option opts[] = {
    { "name",   required_argument, (void *)0, 'n' },
    { "output", required_argument, (void *)0, 'o' },
    { (void *)0, 0,                (void *)0,  0  },
  };
  char *argv[] = { "prog", "--name=alice", "--output=/tmp/x", (void *)0 };
  int argc = 3;
  int c;
  while ((c = getopt_long(argc, argv, "n:o:", opts, (void *)0)) != -1) {
    printf("opt=%c arg=%s\n", c, optarg);
  }
  printf("optind=%d\n", optind);
  return 0;
}
