// goto into a nested block. Standard C allows this; the compiler's
// goto normalizer pass hoists the label so the wasm codegen can
// reach it.
#include <stdio.h>

int direct_jump(void) {
  goto inside;
  {
inside:
    return 42;
  }
}

int via_branch(int x) {
  if (x) goto target;
  return 100;
target:
  return 200;
}

int into_nested_if(int x) {
  if (x == 1) goto deep;
  if (x == 2) {
    int y = 10;
    if (y > 0) {
deep:
      return 999;
    }
  }
  return -1;
}

int main(void) {
  printf("%d\n", direct_jump());        /* 42 */
  printf("%d\n", via_branch(1));        /* 200 */
  printf("%d\n", via_branch(0));        /* 100 */
  printf("%d\n", into_nested_if(1));    /* 999 (early goto) */
  printf("%d\n", into_nested_if(2));    /* 999 (natural path through nested if) */
  printf("%d\n", into_nested_if(99));   /* -1 (neither matches) */
  return 0;
}
