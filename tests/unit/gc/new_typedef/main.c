// __new accepts any type expression that resolves to a GC struct,
// including typedefs (with or without trailing *).
#include <stdio.h>

typedef __struct Point { int x; int y; } PointT;
typedef __struct Point *PointRef;

int main(void) {
    auto a = __new(__struct Point, 1, 2);
    auto b = __new(__struct Point *, 3, 4);
    auto c = __new(PointT, 5, 6);
    auto d = __new(PointRef, 7, 8);
    auto e = __struct_new(PointT, 9, 10);

    printf("%d %d\n", a->x, a->y);
    printf("%d %d\n", b->x, b->y);
    printf("%d %d\n", c->x, c->y);
    printf("%d %d\n", d->x, d->y);
    printf("%d %d\n", e->x, e->y);
    return 0;
}
