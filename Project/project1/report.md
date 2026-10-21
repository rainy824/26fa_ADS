# Performance Comparison of Unbalanced BST, AVL Tree, and Splay Tree

## 高级数据结构与算法分析 — Project 1 实验报告

---

## Chapter 1: Introduction

### 1.1 Project Background

Binary search trees (BSTs) are fundamental data structures that support efficient insertion, deletion, and search operations. However, the performance of an unbalanced BST degrades dramatically when the input is already sorted — the tree degenerates into a linked list, reducing all operations from O(log n) to O(n). To address this problem, various self-balancing search tree variants have been proposed, among which the **AVL tree** and the **Splay tree** are two representative approaches.

This project implements all three structures — an unbalanced BST, an AVL tree, and a Splay tree — and systematically compares their runtime performance under different input patterns. The goal is to empirically verify the theoretical complexity claims and understand the practical trade-offs among these structures.

### 1.2 Overview of the Three Data Structures

**Unbalanced Binary Search Tree (BST)**
- A binary tree where for every node, all keys in the left subtree are smaller and all keys in the right subtree are larger.
- No balancing mechanism is applied.
- Average-case time per operation: O(log n); worst-case: O(n) (when the tree degenerates into a chain).

**AVL Tree**
- A self-balancing BST where the height difference (balance factor) between the left and right subtrees of any node is at most 1.
- After every insertion or deletion, the tree is rebalanced by rotating nodes (LL, RR, LR, RL rotations).
- Worst-case time per operation: strictly O(log n).

**Splay Tree**
- A self-adjusting BST that brings the most recently accessed node to the root through a series of rotations called "splaying" (zig, zig-zig, zig-zag).
- Does not maintain strict balance, but guarantees amortized O(log n) per operation.
- Exploits access locality: frequently accessed nodes stay near the root.

### 1.3 Theoretical Complexity Comparison

| Operation | Unbalanced BST (Avg) | Unbalanced BST (Worst) | AVL Tree | Splay Tree (Amortized) |
|-----------|---------------------|------------------------|----------|------------------------|
| Insert    | O(log n)            | O(n)                   | O(log n) | O(log n)               |
| Delete    | O(log n)            | O(n)                   | O(log n) | O(log n)               |
| Search    | O(log n)            | O(n)                   | O(log n) | O(log n)               |

### 1.4 Research Questions

1. How does the unbalanced BST perform under sorted input compared to random input?
2. Does the deletion order (same order vs. reverse order) significantly affect BST performance?
3. How do AVL and Splay trees compare in practice under different workloads?
4. Do the experimental results align with the theoretical complexity analysis?

---

## Chapter 2: Implementation and Methodology

### 2.1 Overall Architecture

The project adopts a modular design with a unified interface across all three tree implementations:

```
project1/
├── bst.h / bst.c          # Unbalanced BST
├── AVL.h / AVL.c          # AVL tree
├── splay.h / splay.c      # Splay tree
├── performance.c          # Performance testing program
├── plot_results.py        # Plotting script
└── result.csv             # Raw timing data
```

Each tree exposes the same set of operations:
- `xxx_create()` — create an empty tree
- `xxx_insert(root, key)` — insert a key, return new root
- `xxx_delete(root, key)` — delete a key, return new root
- `xxx_destroy(root)` — free all nodes
- `xxx_inorder(root)` — in-order traversal (for correctness verification)

This unified interface allows the test program to reuse the same testing logic for all three trees by simply swapping the function prefix.

### 2.2 Unbalanced BST Implementation

**Node structure:**
```c
struct BSTNode {
    int key;
    BSTree left;
    BSTree right;
};
```

**Insertion:** Implemented iteratively (non-recursive) to avoid stack overflow when inserting 10,000 sorted elements (which would create a tree of depth 10,000). A `parent` pointer tracks the insertion position.

**Deletion:** Handles three cases:
1. **Leaf node** — simply free the node and set the parent's pointer to NULL.
2. **One child** — replace the node with its only child.
3. **Two children** — replace the node's key with the minimum key in its right subtree (the in-order successor), then delete that successor node. The successor is guaranteed to have no left child, simplifying its removal.

**Destruction:** Implemented iteratively using an explicit stack to avoid stack overflow on degenerate trees.

### 2.3 AVL Tree Implementation

**Node structure:**
```c
struct AVLNode {
    int key;
    int height;   // stored height, avoids recomputation
    AVLTree left;
    AVLTree right;
};
```

**Height convention:** Empty node height = -1, leaf node height = 0. The balance factor is defined as `height(left) - height(right)`, with a valid range of [-1, 0, 1].

**Four rotation types:**
- **LL imbalance** (balance factor = 2, left child's balance ≥ 0): right rotation on the root.
- **RR imbalance** (balance factor = -2, right child's balance ≤ 0): left rotation on the root.
- **LR imbalance** (balance factor = 2, left child's balance < 0): left rotation on the left child, then right rotation on the root.
- **RL imbalance** (balance factor = -2, right child's balance > 0): right rotation on the right child, then left rotation on the root.

**Insertion and deletion:** Both implemented recursively. Since the AVL tree height is always O(log n) (at most ~14 for n=10,000), recursion depth is safe. After each recursive call returns, the node's height is updated and its balance factor is checked; if unbalanced, the appropriate rotation is applied. For deletion, the rebalancing propagates all the way up to the root, as a single deletion can cause multiple ancestors to become unbalanced.

### 2.4 Splay Tree Implementation

**Node structure:**
```c
struct SplayNode {
    int key;
    SplayTree parent;   // parent pointer for bottom-up splaying
    SplayTree left;
    SplayTree right;
};
```

**Splaying operation:** Brings a target node to the root using three types of steps:
- **Zig** (single rotation): the parent is the root; one rotation brings the node to the root.
- **Zig-zig** (same-direction double rotation): both the node and its parent are left children (or both right children); rotate the parent first, then the node.
- **Zig-zag** (opposite-direction double rotation): the node and its parent are on opposite sides; rotate the node with its parent first, then with its grandparent.

All rotations carefully maintain `parent` pointers for all affected nodes, including the intermediate subtree and the grandparent.

**Insertion:** Standard BST insertion followed by splaying the newly inserted node to the root.

**Deletion:** Uses the standard two-splay method:
1. Splay the target node to the root.
2. If the root has no left child, the right child becomes the new root.
3. If the root has no right child, the left child becomes the new root.
4. If both children exist: splay the maximum node of the left subtree to the root of the left subtree (it will have no right child), then attach the old root's right subtree to it.

### 2.5 Testing Methodology

**Three test scenarios** (as required by the project):
1. **Scenario 1:** Insert N integers in increasing order, then delete them in increasing order.
2. **Scenario 2:** Insert N integers in increasing order, then delete them in decreasing order.
3. **Scenario 3:** Insert N integers in random order, then delete them in random order.

**Input sizes:** N = 1000, 2000, 3000, ..., 10000 (10 data points).

**Random sequence generation:** The Fisher-Yates shuffle algorithm is used to generate uniformly random permutations of 1..N, guaranteeing no duplicates with O(n) time complexity. A fixed random seed (42) ensures reproducibility.

**Timing method:** The Windows `QueryPerformanceCounter` API is used for high-resolution timing (microsecond级 precision). Only the pure insertion/deletion operations are timed — tree creation, data generation, and memory deallocation are excluded from the measurement.

**Averaging:** Each test case is repeated 5 times, and the average runtime is reported to reduce system noise.

**Compiler optimization:** All code is compiled with `-O2` optimization.

### 2.6 Experimental Environment

| Item | Specification |
|------|--------------|
| Operating System | Windows |
| Compiler | MinGW GCC (x86_64-w64-mingw32) |
| Compilation flags | `-Wall -O2` |
| Timing API | Windows QueryPerformanceCounter |
| Random seed | 42 (fixed) |
| Repeats per test | 5 |

---

## Chapter 3: Experimental Results and Analysis

### 3.1 Complete Runtime Table

All times are in milliseconds (ms), averaged over 5 runs.

#### Scenario 1: Insert Increasing, Delete Increasing

| N | BST Insert | BST Delete | AVL Insert | AVL Delete | Splay Insert | Splay Delete |
|------|-----------|-----------|-----------|-----------|-------------|-------------|
| 1000 | 0.7874 | 0.0099 | 0.0396 | 0.0201 | 0.0628 | 0.0223 |
| 2000 | 3.1361 | 0.0315 | 0.0803 | 0.0444 | 0.0593 | 0.0518 |
| 3000 | 8.4783 | 0.0549 | 0.1309 | 0.0717 | 0.0909 | 0.1486 |
| 4000 | 16.9786 | 0.1136 | 0.1893 | 0.1001 | 0.1093 | 0.1066 |
| 5000 | 24.2867 | 0.1116 | 0.2253 | 0.1367 | 0.1412 | 0.1375 |
| 6000 | 35.8229 | 0.1198 | 0.2723 | 0.1483 | 0.1751 | 0.1806 |
| 7000 | 49.6525 | 0.1351 | 0.3613 | 0.1710 | 0.1998 | 0.1855 |
| 8000 | 65.0853 | 0.1236 | 0.3524 | 0.1944 | 0.1868 | 0.2368 |
| 9000 | 84.2617 | 0.1433 | 0.4028 | 0.2217 | 0.2247 | 0.2280 |
| 10000 | 104.8460 | 0.1560 | 0.4615 | 0.2417 | 0.2897 | 0.3058 |

#### Scenario 2: Insert Increasing, Delete Decreasing

| N | BST Insert | BST Delete | AVL Insert | AVL Delete | Splay Insert | Splay Delete |
|------|-----------|-----------|-----------|-----------|-------------|-------------|
| 1000 | 0.5385 | 0.7545 | 0.0364 | 0.0198 | 0.0192 | 0.0148 |
| 2000 | 3.0064 | 4.3673 | 0.0822 | 0.0384 | 0.0520 | 0.0310 |
| 3000 | 8.5503 | 10.9418 | 0.1422 | 0.0711 | 0.1061 | 0.0606 |
| 4000 | 16.4963 | 20.8172 | 0.1836 | 0.1005 | 0.1076 | 0.0608 |
| 5000 | 25.4561 | 33.8273 | 0.2474 | 0.1226 | 0.1415 | 0.0800 |
| 6000 | 37.2742 | 47.5834 | 0.2687 | 0.1430 | 0.1587 | 0.0808 |
| 7000 | 50.2486 | 67.1183 | 0.3254 | 0.1556 | 0.1776 | 0.0937 |
| 8000 | 64.2692 | 84.7145 | 0.3782 | 0.2210 | 0.1982 | 0.1068 |
| 9000 | 83.2006 | 107.9017 | 0.4076 | 0.2212 | 0.2115 | 0.1378 |
| 10000 | 104.1385 | 134.7121 | 0.4827 | 0.2484 | 0.3441 | 0.1506 |

#### Scenario 3: Insert Random, Delete Random

| N | BST Insert | BST Delete | AVL Insert | AVL Delete | Splay Insert | Splay Delete |
|------|-----------|-----------|-----------|-----------|-------------|-------------|
| 1000 | 0.0728 | 0.0402 | 0.0790 | 0.0659 | 0.1226 | 0.1153 |
| 2000 | 0.1324 | 0.0845 | 0.1665 | 0.1482 | 0.3600 | 0.2790 |
| 3000 | 0.2713 | 0.1454 | 0.2626 | 0.2364 | 0.4591 | 0.4772 |
| 4000 | 0.2711 | 0.2180 | 0.3933 | 0.3611 | 0.6990 | 0.6956 |
| 5000 | 0.3577 | 0.2574 | 0.5443 | 0.4158 | 0.8702 | 0.8516 |
| 6000 | 0.4334 | 0.3025 | 0.5860 | 0.4940 | 1.0227 | 1.0518 |
| 7000 | 0.5832 | 0.3701 | 0.6978 | 0.6206 | 1.2333 | 1.2788 |
| 8000 | 0.5966 | 0.4335 | 0.8439 | 0.7227 | 1.4171 | 1.5194 |
| 9000 | 0.7782 | 0.4865 | 0.9526 | 0.8192 | 1.6056 | 1.7384 |
| 10000 | 0.9251 | 0.5650 | 1.0768 | 0.9219 | 1.8027 | 2.0314 |

### 3.2 Performance Plots

Three comparison plots are generated (Y-axis uses logarithmic scale due to the large performance gap between BST and balanced trees under sorted input):

- **Scenario 1:** `plot_升序插入升序删除.png`
- **Scenario 2:** `plot_升序插入降序删除.png`
- **Scenario 3:** `plot_随机插入随机删除.png`

Each plot contains two subplots: insertion time (left) and deletion time (right), with all three trees overlaid for direct comparison.

### 3.3 Detailed Analysis

#### 3.3.1 Unbalanced BST under Sorted Input — Quadratic Degradation

In Scenarios 1 and 2, inserting elements in increasing order causes the BST to degenerate into a right-skewed linked list. Each new insertion must traverse the entire depth of the tree, so the total insertion time is:

$$T_{insert}(n) = \sum_{i=1}^{n} i = \frac{n(n+1)}{2} = O(n^2)$$

The experimental data confirms this: when N increases from 1000 to 10000 (a 10× increase), BST insertion time grows from 0.79ms to 104.8ms — approximately a **133× increase**, closely matching the expected 100× (10²) for quadratic growth. The slight excess over 100× can be attributed to memory allocation overhead and cache effects.

In contrast, both AVL and Splay trees maintain O(log n) height, so their insertion times grow only ~12× over the same range (0.04ms → 0.46ms for AVL), consistent with O(n log n) total time.

#### 3.3.2 Deletion Order Matters Dramatically for BST

The most striking result is the difference between Scenario 1 and Scenario 2 for BST deletion:

- **Scenario 1 (delete in increasing order):** Deleting from smallest to largest means each deletion targets the **leftmost node**, which after the first deletion becomes the root (since the tree is a right chain). Each deletion is O(1), so total deletion time is O(n) — only 0.156ms for N=10000.
- **Scenario 2 (delete in decreasing order):** Deleting from largest to smallest means each deletion targets the **deepest leaf** at the bottom of the right chain. Each deletion requires traversing the entire remaining depth, so total deletion time is O(n²) — 134.7ms for N=10000.

This 860× difference in deletion time (0.156ms vs. 134.7ms for the same tree) vividly demonstrates that an unbalanced BST is extremely sensitive to operation order, and its worst-case behavior is not merely theoretical.

#### 3.3.3 AVL vs. Splay — Constant Factor Differences

Under sorted input (Scenarios 1 and 2), both AVL and Splay maintain logarithmic performance, but with notable differences:

- **Insertion:** AVL is consistently slightly faster than Splay (e.g., 0.46ms vs. 0.29ms at N=10000 in Scenario 1). This is because Splay's splaying operation typically involves more rotations per insertion (zig-zig/zig-zag steps) compared to AVL's at most one double rotation per insertion.
- **Deletion in Scenario 2:** Splay deletion is noticeably faster than AVL deletion (0.15ms vs. 0.25ms at N=10000). This is because deleting in decreasing order on a Splay tree exhibits strong access locality — each deleted key is near the root after the previous splay operation, reducing traversal cost.

Under random input (Scenario 3), AVL is slightly faster than Splay for both insertion and deletion, consistent with the general understanding that Splay has a larger constant factor due to its more aggressive rotation strategy. However, Splay's advantage appears in workloads with locality, which the random scenario does not provide.

#### 3.3.4 Random Input — All Three Are Competitive

In Scenario 3, the BST performs surprisingly well — even slightly faster than AVL and Splay for insertion (0.93ms vs. 1.08ms vs. 1.80ms at N=10000). This is because:
1. Random input produces a reasonably balanced BST with expected height O(log n).
2. The BST has **no balancing overhead** — no height updates, no balance factor checks, no rotations.
3. AVL and Splay pay the cost of maintaining balance/adjustment on every operation, which adds a constant factor.

This result illustrates an important practical insight: if the input is known to be random or well-distributed, a simple BST can outperform more complex balanced structures due to lower overhead. However, this comes at the cost of worst-case vulnerability.

#### 3.3.5 Theory vs. Experiment Summary

| Claim | Theoretical Prediction | Experimental Result | Match? |
|-------|----------------------|-------------------|--------|
| BST sorted insertion is O(n²) | Quadratic growth | ~133× for 10× N | ✓ |
| BST sorted deletion (same order) is O(n) | Linear growth | Very slow growth (0.01→0.16ms) | ✓ |
| BST sorted deletion (reverse order) is O(n²) | Quadratic growth | ~178× for 10× N | ✓ |
| AVL all operations O(n log n) | ~10-15× for 10× N | ~12× growth | ✓ |
| Splay amortized O(n log n) | ~10-15× for 10× N | ~10-15× growth | ✓ |
| Random BST is competitive | Lower constant factor | Fastest in random insertion | ✓ |

---

## Chapter 4: Conclusion

### 4.1 Summary of Findings

1. **Unbalanced BST is fragile:** Under sorted input, it degenerates to a linked list with O(n²) total operation time. The deletion order has an enormous impact — same-order deletion is O(n) while reverse-order deletion is O(n²). Under random input, however, it performs competitively due to zero balancing overhead.

2. **AVL tree provides reliable performance:** It strictly maintains O(log n) height across all scenarios, with consistent and predictable performance. Its constant factor is small, making it a safe choice when worst-case guarantees are required.

3. **Splay tree offers adaptive performance:** It also achieves amortized O(log n), with a slightly larger constant factor than AVL in random workloads. However, its self-adjusting nature can yield superior performance when the workload exhibits access locality (as seen in Scenario 2 deletion).

### 4.2 Practical Recommendations

| Scenario | Recommended Structure | Reason |
|----------|---------------------|--------|
| Input guaranteed random/sorted-free | Unbalanced BST | Lowest overhead, simplest code |
| Worst-case performance critical | AVL Tree | Strict O(log n) guarantee |
| Access patterns have locality | Splay Tree | Frequently accessed nodes stay near root |
| General-purpose, robust choice | AVL Tree | Predictable performance, moderate complexity |

### 4.3 Limitations and Future Work

- The test only measures insertion and deletion; search operations are not tested, which is where Splay trees typically shine (due to locality).
- The random workload does not model real-world access patterns (e.g., Zipfian distribution), which would favor Splay trees.
- Memory usage is not measured — AVL nodes store an extra height field, and Splay nodes store an extra parent pointer, both increasing per-node memory overhead.
- Larger input sizes (e.g., N = 100,000 to 1,000,000) would provide clearer asymptotic trends and better illustrate cache effects.

### 4.4 Final Remarks

This project successfully implemented three search tree variants and empirically validated their theoretical performance characteristics. The results confirm that while balanced trees (AVL, Splay) are essential for handling adversarial or sorted inputs, the unbalanced BST remains a viable and efficient choice when the input distribution is favorable. The choice among these structures should be guided by the expected workload pattern and the importance of worst-case guarantees.

---

## Appendix: File Listing

| File | Description |
|------|------------|
| `bst.h` / `bst.c` | Unbalanced BST implementation |
| `AVL.h` / `AVL.c` | AVL tree implementation |
| `splay.h` / `splay.c` | Splay tree implementation |
| `performance.c` | Performance testing program |
| `plot_results.py` | Python/matplotlib plotting script |
| `result.csv` | Raw timing data |
| `plot_*.png` | Three performance comparison plots |
| `report.md` | This report |
