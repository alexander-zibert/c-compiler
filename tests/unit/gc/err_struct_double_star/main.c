// `__struct Foo **` is not allowed — wasm has no `(ref ref T)`. The first `*`
// converts heap → ref; a second `*` has nowhere to go.
__struct Foo { int x; };
int main(void) {
  __struct Foo **bad;   // should error
  return 0;
}
