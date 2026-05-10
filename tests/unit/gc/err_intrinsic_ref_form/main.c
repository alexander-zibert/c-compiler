// Intrinsic type-args take the heap form for GC structs; the ref-form
// spelling `__struct Foo *` is rejected. Mirrors wasm: `ref.test`/`ref.cast`
// take a heap type, not a ref type.
__struct Foo { int x; };
int main(void) {
  __struct Foo *p = __new(__struct Foo, 1);
  return __ref_test(__struct Foo *, p);   // should error: use heap form
}
