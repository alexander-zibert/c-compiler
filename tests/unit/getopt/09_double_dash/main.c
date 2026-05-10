// "--" terminates option processing. Following args are non-options.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  char *argv[] = { "prog", "-a", "--", "-b", "file", (void *)0 };
  int argc = 5;
  int c;
  while ((c = getopt(argc, argv, "ab")) != -1) {
    printf("opt=%c\n", c);
  }
  printf("optind=%d\n", optind);
  for (int i = optind; i < argc; i++) printf("rest[%d]=%s\n", i, argv[i]);
  return 0;
}
