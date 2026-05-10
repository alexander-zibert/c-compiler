// `.` on a GC struct ref is rejected — only `->` works (the value is a ref,
// not an aggregate).
__struct Foo { int x; };
int main(void) {
  __struct Foo *p = __new(__struct Foo, 1);
  return p.x;   // should error: use ->
}
