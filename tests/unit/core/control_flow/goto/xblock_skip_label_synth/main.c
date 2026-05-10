// Verify the synthesized skip-label correctly bypasses the hoisted body
// on natural fall-through paths. Two cross-block labels in succession;
// each gets its own skip-label and they don't interfere.
#include <stdio.h>

int classify(int kind, int aux) {
  int sum = 0;

  if (kind == 1) goto labelA;
  if (kind == 2) goto labelB;

  if (aux == 100) {
labelA:
    sum += 10;
  }
  if (aux == 200) {
labelB:
    sum += 20;
  }
  return sum;
}

int main(void) {
  printf("%d\n", classify(1, 0));    /* 10 */
  printf("%d\n", classify(2, 0));    /* 20 */
  printf("%d\n", classify(0, 0));    /* 0  */
  printf("%d\n", classify(0, 100));  /* 10 */
  printf("%d\n", classify(0, 200));  /* 20 */
  printf("%d\n", classify(1, 200));  /* 30 */
  return 0;
}
