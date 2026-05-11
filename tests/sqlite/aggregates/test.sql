.mode list
.headers off

CREATE TABLE sales(region TEXT, product TEXT, qty INTEGER, price REAL);
INSERT INTO sales VALUES
  ('north', 'widget', 10, 2.50),
  ('north', 'gadget',  5, 7.00),
  ('north', 'widget',  3, 2.50),
  ('south', 'widget', 20, 2.50),
  ('south', 'gadget',  7, 7.00),
  ('south', 'thing',   2, 12.00),
  ('east',  'widget',  1, 2.50);

SELECT '--- COUNT, SUM, AVG, MIN, MAX ---';
SELECT COUNT(*), SUM(qty), printf('%.2f', AVG(qty)), MIN(qty), MAX(qty) FROM sales;

SELECT '--- GROUP BY ---';
SELECT region, COUNT(*), SUM(qty), printf('%.2f', SUM(qty * price)) AS revenue
FROM sales GROUP BY region ORDER BY region;

SELECT '--- HAVING ---';
SELECT product, COUNT(*) AS n FROM sales GROUP BY product HAVING COUNT(*) >= 2 ORDER BY n DESC, product;

SELECT '--- DISTINCT ---';
SELECT COUNT(DISTINCT product) FROM sales;
SELECT DISTINCT product FROM sales ORDER BY product;

SELECT '--- aggregate filter (FILTER clause) ---';
SELECT
  SUM(qty) AS total,
  SUM(qty) FILTER (WHERE product = 'widget') AS widgets,
  SUM(qty) FILTER (WHERE region = 'south') AS south
FROM sales;

SELECT '--- group_concat ---';
SELECT region, group_concat(product, ',') FROM sales GROUP BY region ORDER BY region;

SELECT '--- aggregate with CASE ---';
SELECT
  SUM(CASE WHEN price < 5 THEN qty ELSE 0 END) AS cheap,
  SUM(CASE WHEN price >= 5 THEN qty ELSE 0 END) AS expensive
FROM sales;

SELECT '--- empty aggregates ---';
SELECT COUNT(*), SUM(qty), MIN(qty), MAX(qty) FROM sales WHERE region = 'nowhere';
