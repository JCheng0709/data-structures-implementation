# LinkedList — A Simple Singly-Linked List (Rule-of-Five Ready)

This repository contains a **minimal yet modern C++ implementation of a singly-linked list** that demonstrates:

- Manual memory management with `new`/`delete`
- Complete **Rule of Five** support (copy / move ctor & assignment, destructor)
- Basic operations: `insert`, `insertFront`, `remove`, `find`, `size`, `empty`, `print`
- Internal **deep-copy helper** and **copy-and-swap** idiom for strong exception-safety

> 📚 Perfect as a study sample or as a lightweight container in small projects.

---
A
## File structure

| File              | Purpose                                                |
| ----------------- | ------------------------------------------------------ |
| `linked_list.cpp` | Implementation + test `main()` (single TU for clarity) |
| `README.md`       | This document                                          |

You can later split class declaration / definition into separate headers & sources if you prefer.

---
