# Data-Structures-Implementation

A **modern C++11** playground where classic data-structures are built **from scratch**, each in its own self-contained module and written with today’s best practices.  

> ✨ **Why re-invent the wheel?** Writing the wheel teaches you metallurgy, spoke tension, and why rims crack. Likewise, implementing lists, heaps, trees & graphs teaches you memory layout, invariants, complexity trade-offs, and STL internals.

---

## Contents

| Folder | Core files | Highlights |
|--------|------------|------------|
| **`linked-list/`** | `linked_list.cpp`<br>`doubly_linked_list.cpp` | Rule-of-Five, copy-and-swap |
| **`queue/`** | `queue.cpp`<br>`circular_queue.cpp` | Array-backed ring buffer, strong exception-safety, automatic growth |
| **`stack/`** | `stack.cpp` | Auto-resizing array, `noexcept` move ops |
| **`binary-tree/`** | `binaryTree.cpp`<br>`AVL_tree.cpp` | Basic BST + self-balancing AVL with rotations |
| **`hash-table/`** | `hash_table.cpp` |
| **`heap/`** | *(WIP)* | Min/Max template, `heapify`, `buildHeap` |
| **`graph/`** | *(WIP)* | Adjacency-list BFS / DFS, Dijkstra shortest path |
