// Required-argument short option with the argument attached: -ofile.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  char *argv[] = { "prog", "-oOUT.TXT", "-n42", (void *)0 };
  int argc = 3;
  int c;
  while ((c = getopt(argc, argv, "o:n:")) != -1) {
    printf("opt=%c arg=%s\n", c, optarg);
  }
  printf("optind=%d\n", optind);
  return 0;
}
