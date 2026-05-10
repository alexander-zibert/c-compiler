// getopt_long_only accepts long options spelled with a single dash.
// Mirrors tinyemu's CLI style (-help, -rw, -append=...).
#include <getopt.h>
#include <stdio.h>

int main(void) {
  static struct option opts[] = {
    { "help",   no_argument,       (void *)0, 'h' },
    { "rw",     no_argument,       (void *)0, 'r' },
    { "append", required_argument, (void *)0, 'a' },
    { (void *)0,                 0, (void *)0, 0  },
  };
  char *argv[] = { "prog", "-help", "-rw", "-append=cmdline-text", (void *)0 };
  int argc = 4;
  int c;
  while ((c = getopt_long_only(argc, argv, "", opts, (void *)0)) != -1) {
    if (optarg) printf("opt=%c arg=%s\n", c, optarg);
    else printf("opt=%c\n", c);
  }
  return 0;
}
