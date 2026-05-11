.mode list
.headers off

SELECT '--- NULL semantics ---';
SELECT NULL IS NULL;          -- 1
SELECT NULL = NULL;           -- empty (NULL)
SELECT NULL <> NULL;          -- empty (NULL)
SELECT COALESCE(NULL, NULL, 'fallback');
SELECT IFNULL(NULL, 'b');
SELECT NULLIF(5, 5);          -- empty (NULL)
SELECT NULLIF(5, 6);          -- 5

SELECT '--- string functions ---';
SELECT UPPER('hello'), LOWER('WORLD'), LENGTH('abcdef');
SELECT SUBSTR('abcdef', 2, 3);
SELECT REPLACE('hello world', 'world', 'sqlite');
SELECT TRIM('  spaces  '), LTRIM('  left'), RTRIM('right  ');
SELECT printf('%d, %s, %.2f', 42, 'foo', 3.14159);
SELECT instr('hello', 'l');

SELECT '--- numeric functions ---';
SELECT ABS(-5), ABS(5), ABS(0);
SELECT ROUND(3.5), ROUND(3.45, 1), ROUND(-2.5);
SELECT MIN(1, 2, 3), MAX(1, 2, 3);
SELECT 7 / 2, 7 / 2.0, 7 % 3;

SELECT '--- CAST ---';
SELECT CAST('123' AS INTEGER), CAST('3.14abc' AS REAL), CAST(42 AS TEXT);
SELECT CAST('not_a_number' AS INTEGER);     -- 0
SELECT typeof(1), typeof(1.0), typeof('a'), typeof(NULL), typeof(x'ab');

SELECT '--- BLOB ---';
SELECT hex(x'010203');
SELECT length(x'00000000');
SELECT CAST(x'48656c6c6f' AS TEXT);
SELECT length(zeroblob(4));
SELECT hex(zeroblob(3));

SELECT '--- date/time (deterministic with fixed input) ---';
SELECT date('2020-01-15');
SELECT date('2020-01-15', '+30 days');
SELECT julianday('2020-01-15') - julianday('2020-01-01');
SELECT strftime('%Y/%m/%d', '2020-12-25');

SELECT '--- JSON ---';
SELECT json('{"a":1,"b":[2,3]}');
SELECT json_extract('{"a":1,"b":[2,3]}', '$.a');
SELECT json_extract('{"a":1,"b":[2,3]}', '$.b[1]');
SELECT json_array(1, 2, 'three', NULL);
SELECT json_object('k', 'v', 'n', 42);
SELECT json_type('{"a":1}', '$.a');
SELECT json_valid('{}'), json_valid('not_json');

SELECT '--- collations and comparisons ---';
SELECT 'a' < 'b', 'A' < 'a', 'A' = 'a' COLLATE NOCASE;
SELECT 'abc' GLOB 'a*', 'abc' GLOB 'A*';
SELECT 'abc' LIKE '%b%', 'abc' LIKE '%B%';

SELECT '--- INSERT ... RETURNING ---';
CREATE TABLE r(id INTEGER PRIMARY KEY, v TEXT);
INSERT INTO r(v) VALUES ('first'), ('second') RETURNING id, v;

SELECT '--- WITHOUT ROWID ---';
CREATE TABLE wr(k TEXT PRIMARY KEY, v INTEGER) WITHOUT ROWID;
INSERT INTO wr VALUES ('a', 1), ('b', 2), ('c', 3);
SELECT * FROM wr ORDER BY k;

SELECT '--- generated column ---';
CREATE TABLE g(a INTEGER, b INTEGER, sum INTEGER GENERATED ALWAYS AS (a+b) VIRTUAL);
INSERT INTO g(a, b) VALUES (1, 2), (10, 20), (100, 200);
SELECT a, b, sum FROM g ORDER BY a;
