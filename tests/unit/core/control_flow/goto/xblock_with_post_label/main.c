// Label has post-label statements in its containing compound. The tail
// includes ALL of them; they execute on every path that reaches the
// hoisted label.
#include <stdio.h>

int compute(int branch, int v) {
  int total = 0;
  if (branch) {
    if (v == 1) goto mid;
    if (v == 2) goto mid;
    return -100;
  } else {
    total = v;
    mid:
    total += 1;       /* post-label stmt #1 */
    total *= 2;       /* post-label stmt #2 */
  }
  return total;
}

int main(void) {
  /* branch=0, v=5: total=5, mid: total=6, total=12. Return 12. */
  printf("%d\n", compute(0, 5));        /* 12 */
  /* branch=1, v=1: goto mid. total=0+1=1, total=2. Return 2. */
  printf("%d\n", compute(1, 1));        /* 2 */
  /* branch=1, v=2: goto mid. total=0+1=1, total=2. Return 2. */
  printf("%d\n", compute(1, 2));        /* 2 */
  /* branch=1, v=3: no goto, return -100. */
  printf("%d\n", compute(1, 3));        /* -100 */
  return 0;
}
