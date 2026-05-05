// __array_copy requires identical element types.
int main(void) {
  __array(int) a = __array_of(int, 1, 2, 3);
  __array(double) b = __array_new(double, 3);
  __array_copy(b, 0, a, 0, 3);
  return 0;
}
