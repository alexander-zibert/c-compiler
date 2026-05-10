// Required argument missing at end of argv: returns '?', optopt set.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  opterr = 0;
  char *argv[] = { "prog", "-a", "-o", (void *)0 };
  int argc = 3;
  int c;
  while ((c = getopt(argc, argv, "ao:")) != -1) {
    if (c == '?') printf("err optopt=%c\n", optopt);
    else if (optarg) printf("opt=%c arg=%s\n", c, optarg);
    else printf("opt=%c\n", c);
  }
  return 0;
}
