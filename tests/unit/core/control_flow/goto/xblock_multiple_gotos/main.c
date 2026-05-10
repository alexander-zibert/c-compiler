// Multiple distinct gotos targeting the same cross-block label. The
// transform must hoist the label once; all gotos can then reach it.
#include <stdio.h>

int dispatch(int kind) {
  int code = 0;
  if (kind == 1) {
    if (kind & 1) goto out;
    code = 100;
  }
  if (kind == 2) {
    code = 200;
    goto out;
  }
  if (kind == 3) {
    if (kind > 0) {
      goto out;
    }
  }
  if (kind == 4) {
    code = 400;
    if (1) {
      out:
      code += 7;
    }
  }
  return code;
}

int main(void) {
  printf("%d\n", dispatch(1));       /* goto out from kind==1's inner if; code=0+7=7 */
  printf("%d\n", dispatch(2));       /* code=200; goto out; code=207 */
  printf("%d\n", dispatch(3));       /* goto out from kind==3's nested if; code=0+7=7 */
  printf("%d\n", dispatch(4));       /* code=400; natural enter inner-if of kind==4; code=400+7=407 */
  printf("%d\n", dispatch(5));       /* none match; code=0 */
  return 0;
}
