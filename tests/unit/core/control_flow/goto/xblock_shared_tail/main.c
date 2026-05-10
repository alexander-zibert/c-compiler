// Shape 1: shared tail across switch cases. Multiple gotos from inside a
// switch (in if-then-branch) target a label in the if-else-branch. The
// label's body runs once per goto and once per natural else fall-through.
#include <stdio.h>

int parse_escape(int c, int taken_branch) {
  int q = 0;
  if (taken_branch) {
    switch (c) {
      case 'n': c = '\n'; goto add;
      case 't': c = '\t'; goto add;
      case 'r': c = '\r'; goto add;
      default: return -1;
    }
  } else {
    add:
    q = c;
  }
  return q;
}

int main(void) {
  printf("%d\n", parse_escape('n', 1));   /* 10 */
  printf("%d\n", parse_escape('t', 1));   /* 9  */
  printf("%d\n", parse_escape('r', 1));   /* 13 */
  printf("%d\n", parse_escape('x', 1));   /* -1 (default returns) */
  printf("%d\n", parse_escape('a', 0));   /* 97 (else branch: q = 'a') */
  return 0;
}
