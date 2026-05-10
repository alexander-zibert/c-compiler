// Shape 2: function-end cleanup label. Multiple early-exit gotos all
// target a single label deep inside the function. The natural fall-through
// path past the inner if-stmt must NOT reach the labeled body.
//
// Mirrors machine.c get_file_path: the synthesized skip-label keeps the
// natural !cond=false path from running the labeled return.
#include <stdio.h>

int classify(int has_base, int has_colon, int absolute, int is_null, int extra) {
  if (!has_base) goto done;
  if (has_colon) goto done;
  if (absolute) goto done;
  if (is_null) {
done:
    return 100 + extra;
  }
  return 200 + extra;
}

int main(void) {
  printf("%d\n", classify(1, 0, 0, 0, 5));   /* 205 (no goto, no inner-if body) */
  printf("%d\n", classify(1, 0, 0, 1, 5));   /* 105 (inner-if body via goto done) */
  printf("%d\n", classify(0, 0, 0, 0, 5));   /* 105 (early goto via !has_base) */
  printf("%d\n", classify(1, 1, 0, 0, 5));   /* 105 (early goto via has_colon) */
  printf("%d\n", classify(1, 0, 1, 0, 5));   /* 105 (early goto via absolute) */
  return 0;
}
