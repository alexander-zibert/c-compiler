// __new accepts any type expression that resolves to a GC struct heap form,
// including typedefs of the heap form. The ref-form spelling (`PointRef`
// = `__struct Point *`) is NOT a valid type-arg — __new takes a heap type.
#include <stdio.h>

typedef __struct Point { int x; int y; } PointT;

int main(void) {
    auto a = __new(__struct Point, 1, 2);
    auto b = __new(__struct Point, 3, 4);
    auto c = __new(PointT, 5, 6);
    auto e = __struct_new(PointT, 9, 10);

    printf("%d %d\n", a->x, a->y);
    printf("%d %d\n", b->x, b->y);
    printf("%d %d\n", c->x, c->y);
    printf("%d %d\n", e->x, e->y);
    return 0;
}
