// Unrecognized short option: getopt returns '?' and sets optopt.
// opterr=0 suppresses stderr output (so test stdout is deterministic).
#include <getopt.h>
#include <stdio.h>

int main(void) {
  opterr = 0;
  char *argv[] = { "prog", "-a", "-q", "-b", (void *)0 };
  int argc = 4;
  int c;
  while ((c = getopt(argc, argv, "ab")) != -1) {
    if (c == '?') printf("err optopt=%c\n", optopt);
    else printf("opt=%c\n", c);
  }
  return 0;
}
