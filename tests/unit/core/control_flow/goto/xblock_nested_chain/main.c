// Label deeply nested in if-else-if-else chain. Goto from one branch's
// switch jumps to label in the final else. Mirrors json.c parse_string's
// add_char pattern.
#include <stdio.h>

int classify(int kind, int c) {
  int q = 0;
  if (kind == 1) {
    return -1;
  } else if (kind == 2) {
    return -2;
  } else if (kind == 3) {
    /* the "process escape" path */
    switch (c) {
      case 'n': c = 10; goto add;
      case 't': c = 9;  goto add;
      case 'x': c = 0xff; goto add;
      default: return -3;
    }
  } else {
    /* default character path — label here */
    add:
    q = c + 1000;
  }
  return q;
}

int main(void) {
  printf("%d\n", classify(1, 0));      /* -1 */
  printf("%d\n", classify(2, 0));      /* -2 */
  printf("%d\n", classify(3, 'n'));    /* 1010 (goto via case 'n') */
  printf("%d\n", classify(3, 't'));    /* 1009 */
  printf("%d\n", classify(3, 'q'));    /* -3 (default returns) */
  printf("%d\n", classify(99, 'A'));   /* 1065 (else branch) */
  return 0;
}
