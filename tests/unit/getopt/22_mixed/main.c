// Short and long options interleaved, including short combined and long
// with =value, plus a positional non-option argument at the end.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  static struct option opts[] = {
    { "name",  required_argument, (void *)0, 'n' },
    { "verbose", no_argument,     (void *)0, 'v' },
    { (void *)0,                0, (void *)0, 0  },
  };
  char *argv[] = { "prog", "-ab", "--name=alice", "-c", "--verbose", "input.txt", (void *)0 };
  int argc = 6;
  int c;
  while ((c = getopt_long(argc, argv, "abcn:v", opts, (void *)0)) != -1) {
    if (optarg) printf("opt=%c arg=%s\n", c, optarg);
    else printf("opt=%c\n", c);
  }
  printf("optind=%d\n", optind);
  if (optind < argc) printf("rest=%s\n", argv[optind]);
  return 0;
}
