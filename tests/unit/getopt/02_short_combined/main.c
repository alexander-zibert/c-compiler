// Three short options combined into one argv element.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  char *argv[] = { "prog", "-abc", (void *)0 };
  int argc = 2;
  int c;
  while ((c = getopt(argc, argv, "abc")) != -1) {
    printf("opt=%c\n", c);
  }
  printf("optind=%d\n", optind);
  return 0;
}
