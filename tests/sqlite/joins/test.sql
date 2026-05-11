.mode list
.headers off

CREATE TABLE author(id INTEGER PRIMARY KEY, name TEXT);
CREATE TABLE book(id INTEGER PRIMARY KEY, title TEXT, author_id INTEGER);

INSERT INTO author VALUES (1, 'asimov'), (2, 'tolkien'), (3, 'orwell');
INSERT INTO book VALUES
  (10, 'foundation',         1),
  (11, 'i_robot',             1),
  (12, 'the_hobbit',          2),
  (13, '1984',                3),
  (14, 'animal_farm',         3),
  (15, 'untitled',            NULL),
  (16, 'orphan',              99);

SELECT '--- INNER JOIN ---';
SELECT a.name, b.title
FROM author a INNER JOIN book b ON a.id = b.author_id
ORDER BY a.name, b.title;

SELECT '--- LEFT JOIN (authors with no books vs author with NULL ref) ---';
INSERT INTO author VALUES (4, 'silent');
SELECT a.name, b.title
FROM author a LEFT JOIN book b ON a.id = b.author_id
ORDER BY a.name, b.title NULLS LAST;

SELECT '--- LEFT JOIN with NULL on right ---';
SELECT b.title, a.name
FROM book b LEFT JOIN author a ON a.id = b.author_id
WHERE a.id IS NULL
ORDER BY b.title;

SELECT '--- self join ---';
CREATE TABLE node(id INTEGER PRIMARY KEY, name TEXT, parent_id INTEGER);
INSERT INTO node VALUES (1,'root',NULL), (2,'left',1), (3,'right',1), (4,'leaf',2);
SELECT child.name AS child, parent.name AS parent
FROM node child JOIN node parent ON child.parent_id = parent.id
ORDER BY child.name;

SELECT '--- CROSS JOIN ---';
CREATE TABLE color(c TEXT);
CREATE TABLE size(s TEXT);
INSERT INTO color VALUES ('red'),('blue');
INSERT INTO size VALUES ('S'),('M'),('L');
SELECT c.c || '_' || s.s FROM color c CROSS JOIN size s ORDER BY c.c, s.s;

SELECT '--- 3-way join ---';
CREATE TABLE genre(id INTEGER PRIMARY KEY, name TEXT);
CREATE TABLE book_genre(book_id INTEGER, genre_id INTEGER);
INSERT INTO genre VALUES (100, 'scifi'), (101, 'fantasy'), (102, 'dystopia');
INSERT INTO book_genre VALUES (10,100),(11,100),(12,101),(13,102),(14,102);
SELECT a.name, b.title, g.name
FROM author a JOIN book b ON a.id = b.author_id
              JOIN book_genre bg ON bg.book_id = b.id
              JOIN genre g ON g.id = bg.genre_id
ORDER BY a.name, b.title;
