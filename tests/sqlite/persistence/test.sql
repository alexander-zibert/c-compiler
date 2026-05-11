.mode list
.headers off

-- Write phase: create db on disk, populate, close
.open /tmp/c-compiler-sqlite-persistence-test.db
DROP TABLE IF EXISTS records;
CREATE TABLE records(id INTEGER PRIMARY KEY, payload TEXT, n INTEGER);

WITH RECURSIVE r(i) AS (SELECT 1 UNION ALL SELECT i+1 FROM r WHERE i < 100)
INSERT INTO records(payload, n) SELECT printf('item_%04d', i), i*i FROM r;

SELECT '--- write-side count ---';
SELECT COUNT(*) FROM records;
SELECT SUM(n) FROM records;

-- Close db by opening another (in-memory)
.open :memory:
SELECT '--- in-memory after switch (records should not exist) ---';
SELECT name FROM sqlite_master WHERE type='table';

-- Re-open the file db
.open /tmp/c-compiler-sqlite-persistence-test.db
SELECT '--- after re-open ---';
SELECT COUNT(*) FROM records;
SELECT SUM(n) FROM records;
SELECT '--- sample rows ---';
SELECT * FROM records WHERE id <= 3;
SELECT * FROM records WHERE id >= 98;
SELECT '--- write again to existing db ---';
INSERT INTO records(payload, n) VALUES ('appended', -1);
SELECT COUNT(*) FROM records;
SELECT * FROM records WHERE n = -1;

-- Cleanup
DROP TABLE records;
.open :memory:
