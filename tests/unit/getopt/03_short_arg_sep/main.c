// Required-argument short option with the argument in the next argv element.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  char *argv[] = { "prog", "-o", "out.txt", "-n", "42", (void *)0 };
  int argc = 5;
  int c;
  while ((c = getopt(argc, argv, "o:n:")) != -1) {
    printf("opt=%c arg=%s\n", c, optarg);
  }
  printf("optind=%d\n", optind);
  return 0;
}
