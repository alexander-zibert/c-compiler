.mode list
.headers off

CREATE TABLE acct(id INTEGER PRIMARY KEY, name TEXT, bal INTEGER);
INSERT INTO acct VALUES (1, 'alice', 100), (2, 'bob', 200);

SELECT '--- starting balances ---';
SELECT * FROM acct ORDER BY id;

SELECT '--- commit: transfer 50 from alice to bob ---';
BEGIN;
  UPDATE acct SET bal = bal - 50 WHERE id = 1;
  UPDATE acct SET bal = bal + 50 WHERE id = 2;
COMMIT;
SELECT * FROM acct ORDER BY id;

SELECT '--- rollback: discard transfer ---';
BEGIN;
  UPDATE acct SET bal = bal - 1000 WHERE id = 1;
  SELECT '* during txn: ' || bal FROM acct WHERE id = 1;
ROLLBACK;
SELECT '* after rollback: ' || bal FROM acct WHERE id = 1;

SELECT '--- savepoint partial rollback ---';
BEGIN;
  UPDATE acct SET bal = bal + 1 WHERE id = 1;  -- this should stick
  SAVEPOINT sp1;
    UPDATE acct SET bal = bal + 100 WHERE id = 1;  -- this should NOT stick
    SAVEPOINT sp2;
      UPDATE acct SET bal = bal + 1000 WHERE id = 1;  -- this should NOT stick
    ROLLBACK TO sp2;
  ROLLBACK TO sp1;
  UPDATE acct SET bal = bal + 2 WHERE id = 1;  -- this should stick
COMMIT;
SELECT '* final bal id=1: ' || bal FROM acct WHERE id = 1;  -- 51 + 1 + 2 = 54

SELECT '--- multiple statements per txn ---';
BEGIN;
  INSERT INTO acct VALUES (3, 'carol', 500);
  INSERT INTO acct VALUES (4, 'dave', 1000);
  UPDATE acct SET bal = bal * 2 WHERE name IN ('carol', 'dave');
COMMIT;
SELECT * FROM acct ORDER BY id;

SELECT '--- read-only txn ---';
BEGIN;
  SELECT SUM(bal) FROM acct;
COMMIT;
