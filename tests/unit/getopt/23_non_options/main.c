// POSIX-strict (no permute): non-option arg ends scanning. Subsequent
// option-looking args are NOT consumed by getopt.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  char *argv[] = { "prog", "-a", "file1", "-b", "file2", (void *)0 };
  int argc = 5;
  int c;
  while ((c = getopt(argc, argv, "ab")) != -1) {
    printf("opt=%c\n", c);
  }
  printf("optind=%d\n", optind);
  for (int i = optind; i < argc; i++) printf("rest[%d]=%s\n", i, argv[i]);
  return 0;
}
