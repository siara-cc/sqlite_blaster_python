import sys

sys.path.insert(0, '.')
sys.path.insert(0, 'src')

import sqlite_blaster_python as m

def test_basic():
    col_names = "key, value"
    sqib = m.sqlite_index_blaster(2, 1, col_names, "imain", 4096, 40, "kv_idx.db")
    sqib.put_string("hello", "world")
    assert sqib.get_string("hello", "not_found") == "world"
    assert sqib.get_string("hello world", "not_found") == "not_found"
    sqib.close()

def test_table():
    import sqlite_blaster_python
    col_names = "student_name, age, maths_marks, physics_marks, chemistry_marks, average_marks"
    sqib = sqlite_blaster_python.sqlite_index_blaster(6, 2, col_names, "student_marks", 4096, 40, "student_marks.db")
    test_rec1 = ["Robert", 19, 80, 69, 98, round((80+69+98)/3, 2)]
    sqib.put_rec(test_rec1)
    test_rec2 = ["Barry", 20, 82, 99, 83, round((82+99+83)/3, 2)]
    sqib.put_rec(test_rec2)
    test_rec3 = ["Elizabeth", 23, 84, 89, 74, round((84+89+74)/3, 2)]
    sqib.put_rec(test_rec3)
    assert sqib.get_rec(["Elizabeth", 23]) == test_rec3
    assert sqib.get_rec(["Barry", 20]) == test_rec2
    assert sqib.get_rec(["Robert", 19]) == test_rec1
    assert sqib.get_rec(["Robert"]) == []
    sqib.close()

def test_main():
    assert m.__version__ == "0.0.5"
