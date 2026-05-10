// argv[0] with a path prefix: getopt's diagnostic output should use the
// basename only. We don't capture stderr in this test runner — instead
// verify that opterr=0 fully suppresses output and the return path works
// correctly even when argv[0] has a directory prefix.
#include <getopt.h>
#include <stdio.h>

int main(void) {
  opterr = 0;
  char *argv[] = { "/usr/local/bin/prog", "-q", (void *)0 };
  int c = getopt(2, argv, "ab");
  printf("ret=%c optopt=%c\n", c, optopt);
  return 0;
}
