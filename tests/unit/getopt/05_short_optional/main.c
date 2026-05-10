// Optional-argument short options: "x::". Argument must be ATTACHED (-xVAL),
// not in the next argv element.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  char *argv[] = { "prog", "-xfoo", "-y", "-z", "notarg", (void *)0 };
  int argc = 5;
  int c;
  while ((c = getopt(argc, argv, "x::y::z::")) != -1) {
    if (optarg) printf("opt=%c arg=%s\n", c, optarg);
    else printf("opt=%c (no arg)\n", c);
  }
  printf("optind=%d\n", optind);
  if (optind < argc) printf("rest=%s\n", argv[optind]);
  return 0;
}
