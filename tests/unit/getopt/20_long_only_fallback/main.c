// In long_only mode, "-h" with no long match falls back to short option 'h'
// (which exists in optstring). Mirrors tinyemu's "-h" / "-m" behavior.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  static struct option opts[] = {
    { "help",   no_argument,       (void *)0, 'H' },
    { (void *)0,                0, (void *)0,  0  },
  };
  // -h matches short, NOT long ("help" doesn't match "h" exactly).
  // -m takes a required arg via short.
  char *argv[] = { "prog", "-h", "-m", "128", (void *)0 };
  int argc = 4;
  int c;
  while ((c = getopt_long_only(argc, argv, "hm:", opts, (void *)0)) != -1) {
    if (optarg) printf("opt=%c arg=%s\n", c, optarg);
    else printf("opt=%c\n", c);
  }
  return 0;
}
