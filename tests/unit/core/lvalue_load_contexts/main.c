// Exercises every parser site where an lvalue is consumed as an rvalue
// (and therefore wrapped in ELValueLoad), plus the no-load contexts
// where the lvalue itself is the result (& / ++ / -- / sizeof / =).

#include <stdio.h>

struct S { int a; int b; };

int side = 0;
int *getp(int *p) { side++; return p; }

int neg(int x) { return -x; }

int main(void) {
  int x = 7;
  int y = 11;
  int arr[3] = {100, 200, 300};
  struct S s = {1, 2};
  struct S *ps = &s;
  int *p = &x;

  // --- LOAD contexts ---
  // Binary operands
  printf("bin: %d\n", x + y);                    // 18
  printf("cmp: %d\n", x < y);                    // 1

  // Unary operand (pos / neg / not / bnot)
  printf("neg: %d\n", -x);                       // -7
  printf("pos: %d\n", +x);                       // 7
  printf("lnot: %d\n", !x);                      // 0
  printf("bnot: %d\n", ~x);                      // -8

  // Subscript index (rvalue) — and array operand decays
  int i = 1;
  printf("sub-idx: %d\n", arr[i]);               // 200

  // Dereference of pointer (load p before deref)
  printf("deref: %d\n", *p);                     // 7

  // Member / arrow (the base address remains lvalue, but reading the
  // member is a load)
  printf("member: %d\n", s.a);                   // 1
  printf("arrow:  %d\n", ps->b);                 // 2

  // Ternary cond + branches
  int z = (x > 0 ? x : y);
  printf("ternary: %d\n", z);                    // 7

  // Cast operand
  long lx = (long) x;
  printf("cast: %ld\n", lx);                     // 7

  // Call argument
  printf("arg: %d\n", neg(x));                   // -7

  // Call callee via function pointer
  int (*fp)(int) = neg;
  printf("fptr-call: %d\n", fp(x));              // -7

  // Initializer rhs
  int q = x;
  printf("init: %d\n", q);                       // 7

  // Assignment rhs
  q = y;
  printf("assign: %d\n", q);                     // 11

  // Return value (verified via the call result above)

  // Conditions: if / while / for / do-while / switch
  if (x) printf("if: yes\n");                    // if: yes
  int n = 0;
  while (n < 2) { printf("while: %d\n", n); n++; } // while: 0/1
  do { printf("do: %d\n", n); n--; } while (n > 0); // do: 2/1
  for (int k = x; k < y; k++) printf("for: %d\n", k); // for: 7..10
  switch (x) {
    case 7:  printf("switch: seven\n"); break;
    default: printf("switch: other\n");
  }

  // --- NO-LOAD contexts ---
  // Address-of: takes the storage location, not the value
  int *pa = &x;
  printf("addr-of: %d\n", *pa);                  // 7

  // Pre/post inc/dec: read+write through the lvalue, not via load
  int u = 5;
  ++u; u++; --u;
  printf("incdec: %d\n", u);                     // 6
  int v = u++;
  printf("post-inc returns old: v=%d u=%d\n", v, u); // 6 7

  // sizeof on an lvalue does NOT load (no side effects)
  side = 0;
  printf("sizeof-noload: %d side=%d\n", (int)sizeof(*getp(p)), side); // 4 0

  // Compound assign: left is the lvalue target (no separate load wrapper)
  int w = 10;
  w += x;
  printf("compound: %d\n", w);                   // 17

  // Assignment LHS is the target (rhs is loaded)
  int t;
  t = x;
  printf("assign-lhs: %d\n", t);                 // 7

  printf("PASS\n");
  return 0;
}
