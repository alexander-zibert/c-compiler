// Goto from outer scope into a label inside a nested block.
// (Outer-encloses-inner case — codegen can't `br` INTO a nested
// wasm block, so the transform must hoist the label out to outer scope.)
#include <stdio.h>

int outer_to_inner(int x) {
  goto target;
  {
    int local_decl_in_block = 999;  /* unrelated decl, not in label tail */
    (void)local_decl_in_block;
target:
    return x * 2;
  }
}

int main(void) {
  printf("%d\n", outer_to_inner(7));   /* 14 */
  printf("%d\n", outer_to_inner(0));   /* 0 */
  return 0;
}
