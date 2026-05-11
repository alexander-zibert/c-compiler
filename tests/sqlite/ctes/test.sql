.mode list
.headers off

SELECT '--- simple CTE ---';
CREATE TABLE vals(id INTEGER, val INTEGER);
INSERT INTO vals VALUES (1, 100), (2, 200), (3, 300);
WITH high_value AS (SELECT * FROM vals WHERE val > 150)
SELECT * FROM high_value ORDER BY id;

SELECT '--- multi-CTE chain ---';
WITH
  evens AS (SELECT value FROM generate_series(1, 10) WHERE value % 2 = 0),
  doubled AS (SELECT value * 2 AS v FROM evens)
SELECT v FROM doubled ORDER BY v;

SELECT '--- recursive: count to 5 ---';
WITH RECURSIVE c(n) AS (
  SELECT 1 UNION ALL SELECT n+1 FROM c WHERE n < 5
)
SELECT n FROM c;

SELECT '--- recursive: factorial ---';
WITH RECURSIVE fact(n, f) AS (
  SELECT 0, 1
  UNION ALL
  SELECT n+1, (n+1)*f FROM fact WHERE n < 7
)
SELECT n, f FROM fact;

SELECT '--- recursive: tree traversal ---';
CREATE TABLE tree(id INTEGER PRIMARY KEY, parent_id INTEGER, name TEXT);
INSERT INTO tree VALUES
  (1, NULL, 'root'),
  (2, 1,    'child_a'),
  (3, 1,    'child_b'),
  (4, 2,    'grand_aa'),
  (5, 2,    'grand_ab'),
  (6, 3,    'grand_ba'),
  (7, 5,    'great_aba');

WITH RECURSIVE descendants(id, name, depth, path) AS (
  SELECT id, name, 0, name FROM tree WHERE id = 1
  UNION ALL
  SELECT t.id, t.name, d.depth + 1, d.path || '/' || t.name
  FROM tree t JOIN descendants d ON t.parent_id = d.id
)
SELECT printf('%s%s [d=%d]', substr('                ', 1, depth*2), name, depth) FROM descendants ORDER BY path;

SELECT '--- recursive: Fibonacci ---';
WITH RECURSIVE fib(n, a, b) AS (
  SELECT 1, 0, 1
  UNION ALL
  SELECT n+1, b, a+b FROM fib WHERE n < 10
)
SELECT n, a FROM fib;
