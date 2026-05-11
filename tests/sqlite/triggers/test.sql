.mode list
.headers off

CREATE TABLE inventory(id INTEGER PRIMARY KEY, item TEXT, qty INTEGER);
CREATE TABLE log(action TEXT, info TEXT);

INSERT INTO inventory VALUES (1, 'apples', 100), (2, 'bread', 20), (3, 'milk', 15);

CREATE TRIGGER t_after_insert AFTER INSERT ON inventory
BEGIN
  INSERT INTO log VALUES ('insert', 'id=' || NEW.id || ' item=' || NEW.item || ' qty=' || NEW.qty);
END;

CREATE TRIGGER t_after_update AFTER UPDATE OF qty ON inventory
BEGIN
  INSERT INTO log VALUES ('update', 'id=' || NEW.id || ' qty_old=' || OLD.qty || ' qty_new=' || NEW.qty);
END;

CREATE TRIGGER t_before_delete BEFORE DELETE ON inventory
BEGIN
  INSERT INTO log VALUES ('delete', 'id=' || OLD.id || ' item=' || OLD.item);
END;

SELECT '--- exercise triggers ---';
INSERT INTO inventory VALUES (4, 'cheese', 8);
UPDATE inventory SET qty = qty - 5 WHERE item = 'apples';
DELETE FROM inventory WHERE item = 'milk';

SELECT '--- log contents ---';
SELECT * FROM log;

SELECT '--- conditional trigger (WHEN clause) ---';
CREATE TABLE big_log(info TEXT);
CREATE TRIGGER t_big AFTER UPDATE OF qty ON inventory
WHEN NEW.qty > 50
BEGIN
  INSERT INTO big_log VALUES ('big_qty: id=' || NEW.id || ' qty=' || NEW.qty);
END;
UPDATE inventory SET qty = 200 WHERE id = 4;
UPDATE inventory SET qty = 5 WHERE id = 2;
SELECT * FROM big_log;

SELECT '--- INSTEAD OF on a view ---';
CREATE VIEW v_inv AS SELECT id, item, qty FROM inventory;
CREATE TABLE view_log(msg TEXT);
CREATE TRIGGER t_view INSTEAD OF UPDATE ON v_inv
BEGIN
  INSERT INTO view_log VALUES ('view-update id=' || NEW.id || ' new_qty=' || NEW.qty);
END;
UPDATE v_inv SET qty = 999 WHERE id = 1;
SELECT * FROM view_log;
-- underlying table should be unchanged
SELECT qty FROM inventory WHERE id = 1;
