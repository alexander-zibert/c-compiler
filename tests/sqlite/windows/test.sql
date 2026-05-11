.mode list
.headers off

CREATE TABLE score(student TEXT, subject TEXT, score INTEGER);
INSERT INTO score VALUES
  ('alice','math',95), ('alice','sci',88), ('alice','art',70),
  ('bob','math',60),   ('bob','sci',75),   ('bob','art',92),
  ('carol','math',82), ('carol','sci',90), ('carol','art',85);

SELECT '--- ROW_NUMBER per subject ---';
SELECT subject, student, score,
       ROW_NUMBER() OVER (PARTITION BY subject ORDER BY score DESC) AS rn
FROM score ORDER BY subject, rn;

SELECT '--- RANK / DENSE_RANK with ties ---';
CREATE TABLE pts(player TEXT, points INTEGER);
INSERT INTO pts VALUES ('a',100),('b',90),('c',90),('d',80),('e',70);
SELECT player, points,
       RANK() OVER (ORDER BY points DESC) AS rk,
       DENSE_RANK() OVER (ORDER BY points DESC) AS drk
FROM pts ORDER BY points DESC, player;

SELECT '--- LAG / LEAD ---';
CREATE TABLE ts(t INTEGER, v INTEGER);
INSERT INTO ts VALUES (1,10),(2,15),(3,12),(4,20),(5,18);
SELECT t, v,
       LAG(v) OVER (ORDER BY t) AS prev,
       LEAD(v) OVER (ORDER BY t) AS next,
       v - LAG(v) OVER (ORDER BY t) AS delta
FROM ts ORDER BY t;

SELECT '--- running sum / avg ---';
SELECT t, v,
       SUM(v) OVER (ORDER BY t ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW) AS rsum,
       printf('%.2f', AVG(v) OVER (ORDER BY t ROWS BETWEEN 1 PRECEDING AND CURRENT ROW)) AS roll2
FROM ts ORDER BY t;

SELECT '--- NTILE ---';
SELECT v, NTILE(3) OVER (ORDER BY v) AS bucket FROM ts ORDER BY v;

SELECT '--- FIRST_VALUE / LAST_VALUE ---';
SELECT subject, student, score,
       FIRST_VALUE(student) OVER (PARTITION BY subject ORDER BY score DESC) AS top,
       LAST_VALUE(student)  OVER (PARTITION BY subject ORDER BY score DESC
                                   ROWS BETWEEN UNBOUNDED PRECEDING
                                            AND UNBOUNDED FOLLOWING) AS bottom
FROM score ORDER BY subject, score DESC;
