// Bare `__struct Foo` is the heap form — not a value type. Variables, fields,
// params, and returns must use the ref form `__struct Foo *`.
__struct Foo { int x; };
int main(void) {
  __struct Foo bad;   // should error: heap form in value position
  return 0;
}
