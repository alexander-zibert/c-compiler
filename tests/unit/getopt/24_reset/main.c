// Setting optind=0 (or 1) before a fresh getopt loop resets internal state,
// allowing reuse on a different argv.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  char *argv1[] = { "prog", "-a", "-b", (void *)0 };
  int c;
  while ((c = getopt(3, argv1, "abc")) != -1) printf("first opt=%c\n", c);
  printf("first optind=%d\n", optind);

  // Reset for a second pass over a different argv.
  optind = 0;

  char *argv2[] = { "prog", "-c", (void *)0 };
  while ((c = getopt(2, argv2, "abc")) != -1) printf("second opt=%c\n", c);
  printf("second optind=%d\n", optind);
  return 0;
}
