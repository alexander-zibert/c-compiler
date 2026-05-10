// Optstring beginning with ':' is silent mode: missing required arg returns
// ':' (not '?'). The character is set in optopt regardless.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  char *argv[] = { "prog", "-o", (void *)0 };
  int argc = 2;
  int c = getopt(argc, argv, ":o:");
  printf("ret=%c optopt=%c\n", c, optopt);
  return 0;
}
