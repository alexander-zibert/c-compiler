// Three short options, none take arguments.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  char *argv[] = { "prog", "-a", "-b", "-c", (void *)0 };
  int argc = 4;
  int c;
  while ((c = getopt(argc, argv, "abc")) != -1) {
    printf("opt=%c\n", c);
  }
  printf("optind=%d\n", optind);
  return 0;
}
