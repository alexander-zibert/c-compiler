// Long option with optional argument: --name=value (with arg)
// vs --name (without arg). Bare --name does NOT consume the next argv.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  static struct option opts[] = {
    { "color", optional_argument, (void *)0, 'c' },
    { (void *)0,                0, (void *)0,  0  },
  };
  char *argv[] = { "prog", "--color=red", "--color", "next-arg", (void *)0 };
  int argc = 4;
  int c;
  while ((c = getopt_long(argc, argv, "c::", opts, (void *)0)) != -1) {
    if (optarg) printf("opt=%c arg=%s\n", c, optarg);
    else printf("opt=%c (no arg)\n", c);
  }
  printf("optind=%d\n", optind);
  if (optind < argc) printf("rest=%s\n", argv[optind]);
  return 0;
}
