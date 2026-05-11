/* Duff's device and nested-case-label patterns. These force the
 * compiler down the loop-switch lowering fallback because the case
 * labels live inside an inner SCompound (or a loop body), not at the
 * switch body's top level. Structured wasm control flow can't dispatch
 * directly into the middle of an inner block; the lowering pass
 * handles it by giving each SCase its own segment in a state machine.
 *
 * The simple `case OP_X: { ...; case OP_Y: ...; }` shape comes from
 * SQLite's VDBE opcode dispatcher; the classic copy-unrolling form
 * comes from Tom Duff's xterm patch. Both are valid C and both must
 * produce the same observable behavior as the equivalent structured
 * code.
 */
#include <stdio.h>

/* SQLite-VDBE style: case label buried inside an inner brace block.
 * Tests that dispatching to `case 2:` from the switch dispatch lands
 * INSIDE the case-1 brace block, not at its start. */
static int sqlite_style(int op) {
  int r = 0;
  switch (op) {
    case 1: {
      int x;
      r = 100;
      x = 1;
      r += x;
      goto done;
    case 2:
      r = 200;
      r += 5;
      goto done;
    }
    case 3:
      r = 300;
      goto done;
  }
done:
  r += 1;
  return r;
}

/* Classic Duff's device: case labels interleaved with a do-while loop
 * body. After dispatching into the middle of the loop, the loop
 * continues iterating, hitting every case statement on each pass. */
static void duffs_copy(int *dst, const int *src, int count) {
  int n = (count + 7) / 8;
  int rem = count % 8;
  switch (rem) {
    case 0: do { *dst++ = *src++;
    case 7:      *dst++ = *src++;
    case 6:      *dst++ = *src++;
    case 5:      *dst++ = *src++;
    case 4:      *dst++ = *src++;
    case 3:      *dst++ = *src++;
    case 2:      *dst++ = *src++;
    case 1:      *dst++ = *src++;
            } while (--n > 0);
  }
}

/* Variant: deeply nested case (case inside compound inside compound)
 * with a goto landing on a forward label past the switch. */
static int nested_two_deep(int op) {
  int r = 0;
  switch (op) {
    case 1: {
      r = 10;
      {
    case 2:
        r += 20;
        goto out;
      }
    }
    case 3:
      r = 30;
      goto out;
    default:
      r = -1;
  }
out:
  return r;
}

int main(void) {
  /* SQLite-style nested case */
  printf("sqlite_style(1) = %d\n", sqlite_style(1));  /* 100+1+1 = 102 */
  printf("sqlite_style(2) = %d\n", sqlite_style(2));  /* 200+5+1 = 206 */
  printf("sqlite_style(3) = %d\n", sqlite_style(3));  /* 300+1   = 301 */

  /* Duff's device: copy 13 ints (rem=5, n=2). All 13 values must come
   * through unchanged regardless of where dispatch lands. */
  int src[13] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 };
  int dst[13] = { 0 };
  duffs_copy(dst, src, 13);
  for (int i = 0; i < 13; i++) printf("dst[%d]=%d\n", i, dst[i]);

  /* Try a copy of exactly 8 (rem=0, n=1) and 1 (rem=1, n=1) to hit the
   * other dispatch entries. */
  int small_src[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
  int small_dst[8] = { 0 };
  duffs_copy(small_dst, small_src, 8);
  for (int i = 0; i < 8; i++) printf("small_dst[%d]=%d\n", i, small_dst[i]);

  int one_src[1] = { 42 };
  int one_dst[1] = { 0 };
  duffs_copy(one_dst, one_src, 1);
  printf("one_dst[0]=%d\n", one_dst[0]);

  /* Two-deep nesting */
  printf("nested_two_deep(1) = %d\n", nested_two_deep(1));  /* 10+20 = 30 */
  printf("nested_two_deep(2) = %d\n", nested_two_deep(2));  /* 0+20  = 20 */
  printf("nested_two_deep(3) = %d\n", nested_two_deep(3));  /* 30 */
  printf("nested_two_deep(9) = %d\n", nested_two_deep(9));  /* -1 (default) */

  return 0;
}
