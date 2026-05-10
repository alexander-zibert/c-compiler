// Hoisting a label OUT of a loop body would change iteration semantics.
// The transform must refuse this case and let codegen surface its
// own diagnostic (target label not in scope).
int main(void) {
  goto inside;
  for (int i = 0; i < 10; i++) {
    inside:
    return i;
  }
  return -1;
}
