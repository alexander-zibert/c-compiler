.mode list
.headers off

PRAGMA foreign_keys = ON;

CREATE TABLE customer(id INTEGER PRIMARY KEY, name TEXT);
CREATE TABLE [order](
  id INTEGER PRIMARY KEY,
  customer_id INTEGER NOT NULL,
  amount REAL,
  FOREIGN KEY(customer_id) REFERENCES customer(id) ON DELETE CASCADE
);
CREATE TABLE invoice(
  id INTEGER PRIMARY KEY,
  order_id INTEGER,
  paid INTEGER,
  FOREIGN KEY(order_id) REFERENCES [order](id) ON DELETE SET NULL
);

INSERT INTO customer VALUES (1, 'alice'), (2, 'bob');
INSERT INTO [order] VALUES (10, 1, 100.0), (11, 1, 200.0), (12, 2, 50.0);
INSERT INTO invoice VALUES (100, 10, 1), (101, 11, 0), (102, 12, 1);

SELECT '--- initial ---';
SELECT * FROM customer;
SELECT * FROM [order];
SELECT * FROM invoice;

SELECT '--- FK violation: insert orphan ---';
INSERT INTO [order] VALUES (99, 999, 5.0);

SELECT '--- ON DELETE CASCADE: delete customer 1, cascades to orders, sets invoice.order_id NULL ---';
DELETE FROM customer WHERE id = 1;
SELECT '* customers:';
SELECT * FROM customer;
SELECT '* orders:';
SELECT * FROM [order];
SELECT '* invoices:';
SELECT * FROM invoice ORDER BY id;

SELECT '--- pragma foreign_key_check ---';
SELECT * FROM pragma_foreign_key_check;

SELECT '--- disable FKs, allow orphan ---';
PRAGMA foreign_keys = OFF;
INSERT INTO [order] VALUES (98, 999, 1.0);
SELECT '* count after orphan insert with FKs off:';
SELECT COUNT(*) FROM [order];
