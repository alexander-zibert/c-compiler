__struct A { int x; };
int main(void) {
  __struct A *a = __new(__struct A);
  __struct A *b = __new(__struct A);
  return a < b;  // relational on refs is meaningless — should error
}
