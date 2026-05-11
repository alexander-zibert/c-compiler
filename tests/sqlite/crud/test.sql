.mode list
.headers off

CREATE TABLE t(id INTEGER PRIMARY KEY, name TEXT NOT NULL, val INTEGER);

INSERT INTO t(name, val) VALUES ('alpha', 1), ('beta', 2), ('gamma', 3), ('delta', 4);

SELECT '--- initial ---';
SELECT * FROM t ORDER BY id;

SELECT '--- update single ---';
UPDATE t SET val = val * 10 WHERE name = 'beta';
SELECT * FROM t WHERE name = 'beta';

SELECT '--- update many ---';
UPDATE t SET val = val + 100 WHERE val < 5;
SELECT name, val FROM t ORDER BY id;

SELECT '--- delete ---';
DELETE FROM t WHERE name = 'gamma';
SELECT COUNT(*) FROM t;
SELECT * FROM t ORDER BY id;

SELECT '--- insert OR IGNORE on PK conflict ---';
INSERT OR IGNORE INTO t(id, name, val) VALUES (1, 'duplicate', 999);
SELECT * FROM t WHERE id = 1;

SELECT '--- insert OR REPLACE on PK conflict ---';
INSERT OR REPLACE INTO t(id, name, val) VALUES (1, 'replaced', 999);
SELECT * FROM t WHERE id = 1;
