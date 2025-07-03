# Data-Structures-Implementation

A **modern C++20** playground where classic data-structures are built **from scratch**, each in its own self-contained module and written with today’s best practices (Rule-of-Five, smart pointers, `constexpr`, unit-tested).  Ideal for interview prep, CS coursework reference, or simply sharpening your C++ skills.

> ✨ **Why re-invent the wheel?**  Because writing the wheel teaches you metallurgy, spoke tension, and why rims crack. Likewise, implementing lists, heaps, trees & graphs teaches you memory layout, invariants, complexity trade-offs, and STL internals.

---

## Contents

| Folder | Core files | Highlights |
|--------|------------|-----------|
| **`linked-list/`** | `linked_list.cpp`<br>`doubly_linked_list.cpp` | Rule-of-Five, copy-and-swap |
| **`queue/`** | `circular_queue.cpp` | Array-backed ring buffer, `std::optional` pop |
| **`stack/`** | `stack.cpp` | Dynamic array version |
| **`binary-tree/`** | WIP | Recursive + iterative insert/erase, self-balancing AVL variant |
| **`heap/`** | WIP | Min/Max template, `std::vector` backing, `heapify` & `buildHeap` |
| **`graph/`** | WIP | Weighted/unweighted BFS & DFS, Dijkstra shortest path |

*(New folders auto-discovered by CMake)*

---
