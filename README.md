# CS509 Laboratory Repository

## Repository Overview

This repository contains buddy (pair) assignments for CS509 . Assignment 01 implements BFS, DFS, and SSSP (Dijkstra) on graphs stored in CSR (Compressed Sparse Row) format.

## Student / Pair Details

- **Student 1:** Vishal Chandel (2026AIM1023)
- **Student 2:** ----------------------------
- **Program:** M.Tech Artificial Intelligence, IIT Ropar

## Language and Environment

- **Language:** C++17
- **Compiler:** MinGW g++ (GCC)
- **Build Tool:** mingw32-make
- **Timing Method:** `std::chrono::high_resolution_clock`, reported in milliseconds (ms)
- **Platform:** Windows 10/11

## Directory Structure

```
CS_509_2026AIM1023_2026AIM1023/
|-- README.md
|-- common_wrapper/
|   |-- wrapper.cpp
|   `-- Makefile
|-- assignment_01/
    |-- driver/
    |   `-- driver.cpp
    |-- include/
    |   |-- bfs.h
    |   |-- csr.h
    |   |-- dfs.h
    |   |-- graph.h
    |   |-- sssp.h
    |   `-- timer.h
    |-- src/
    |   |-- bfs.cpp
    |   |-- csr.cpp
    |   |-- dfs.cpp
    |   |-- graph.cpp
    |   |-- sssp.cpp
    |   `-- timer.cpp
    |-- tests/
    |   |-- bfs_10.txt
    |   |-- bfs_100.txt
    |   |-- bfs_10000.txt
    |   |-- bfs_50000.txt
    |   |-- bfs_100000.txt
    |   |-- dfs_10.txt
    |   |-- dfs_100.txt
    |   |-- dfs_10000.txt
    |   |-- dfs_50000.txt
    |   |-- dfs_100000.txt
    |   |-- sssp_10.txt
    |   |-- sssp_100.txt
    |   |-- sssp_10000.txt
    |   |-- sssp_50000.txt
    |   `-- sssp_100000.txt
    `-- Makefile
```

## Common Wrapper: Build and Usage

The wrapper acts as a main program interface.

```powershell
cd common_wrapper
mingw32-make
.\wrapper.exe
```

The wrapper provides:
- Assignment selection menu
- Compile Assignment 01
- Run one selected test file
- Run all tests for one algorithm
- Run all algorithms on all test files

## General Conventions

- Graph input uses adjacency-list format with a `SOURCE s` configuration line.
- Adjacency-list-to-CSR conversion is done during preprocessing and is excluded from timing metrics.
- Timing includes only algorithm execution time.
- Vertex numbering uses 0 to V-1.
- Variables use type-prefixed snake_case naming style.

---

## Assignment 01 - BFS, DFS, and SSSP

### Assignment Mode

Double / Buddy

### Objective

Implement graph traversal (BFS, DFS) and shortest-path (SSSP) algorithms using CSR representation.

### Algorithm / Approach

- **CSR Conversion:** Converts adjacency representation to flat `v_row_ptr`, `v_col_index`, and `v_weights` vectors.
- **BFS:** Queue-based level-order traversal with unweighted edge-count distance calculations.
- **DFS:** Iterative depth-first traversal. Uses a heap-allocated `std::vector` stack instead of deep call recursion frames to avoid Windows thread stack limit constraints (stack overflows on V=100,000 scale).
- **SSSP:** Dijkstra's shortest-path algorithm using a binary min-priority queue (positive weights only).

### Input Format

Unweighted (BFS/DFS):
```
V E
u degree neighbor1 neighbor2 ...
...
SOURCE s
```

Weighted (SSSP):
```
V E
u degree neighbor1 weight1 neighbor2 weight2 ...
...
SOURCE s
```

### Helper Functions / CSR Conversion

- `read_unweighted_graph()` / `read_weighted_graph()` in `src/graph.cpp`
- `convert_to_csr()` in `src/csr.cpp`
- Preprocessing time is excluded from reported algorithm runtimes.

### File Structure

| File | Purpose |
|------|---------|
| `driver/driver.cpp` | Command-line parsing, CSR conversion preprocessing, algorithm dispatch, formatted console logging. |
| `src/graph.cpp` | Adjacency list graph reader helper. |
| `src/csr.cpp` | Conversion utilities to CSR format. |
| `src/bfs.cpp` | BFS level-order queue search. |
| `src/dfs.cpp` | DFS iterative stack traversal. |
| `src/sssp.cpp` | Dijkstra min-priority queue shortest path search. |

### Compilation

```powershell
cd assignment_01
mingw32-make
```

### Execution

```powershell
# Run a specific algorithm on a test file
.\graph_simulator.exe bfs tests/bfs_10.txt
.\graph_simulator.exe dfs tests/dfs_10.txt
.\graph_simulator.exe sssp tests/sssp_10.txt

# Run all tests for a specific algorithm
.\graph_simulator.exe --all-tests bfs

# Run all algorithms on all scales (10, 100, 10000, 50000, 100000)
.\graph_simulator.exe --all
```

### Test Cases and Result Table

All benchmarks were run locally on the target platform.

| Algorithm | Test File | Vertices | Edges | Input Type | Source | Expected Output | Actual Output | Time (ms) | Status |
|-----------|-----------|----------|-------|------------|--------|-----------------|---------------|------|--------|
| BFS | bfs_10.txt | 10 | 14 | Unweighted adjacency list (CSR) | 0 | Traversal + distances | Matches expected | 0.000 ms | Pass |
| BFS | bfs_100.txt | 100 | 200 | Unweighted adjacency list (CSR) | 0 | Traversal + distances | Matches expected | 0.000 ms | Pass |
| BFS | bfs_10000.txt | 10,000 | 20,000 | Unweighted adjacency list (CSR) | 0 | Traversal + distances | Matches expected | 1.994 ms | Pass |
| BFS | bfs_50000.txt | 50,000 | 100,000 | Unweighted adjacency list (CSR) | 0 | Traversal + distances | Matches expected | 13.832 ms | Pass |
| BFS | bfs_100000.txt | 100,000 | 200,000 | Unweighted adjacency list (CSR) | 0 | Traversal + distances | Matches expected | 21.210 ms | Pass |
| DFS | dfs_10.txt | 10 | 14 | Unweighted adjacency list (CSR) | 0 | Valid DFS traversal | Matches expected | 0.000 ms | Pass |
| DFS | dfs_100.txt | 100 | 200 | Unweighted adjacency list (CSR) | 0 | Valid DFS traversal | Matches expected | 0.000 ms | Pass |
| DFS | dfs_10000.txt | 10,000 | 20,000 | Unweighted adjacency list (CSR) | 0 | Valid DFS traversal | Matches expected | 3.007 ms | Pass |
| DFS | dfs_50000.txt | 50,000 | 100,000 | Unweighted adjacency list (CSR) | 0 | Valid DFS traversal | Matches expected | 15.383 ms | Pass |
| DFS | dfs_100000.txt | 100,000 | 200,000 | Unweighted adjacency list (CSR) | 0 | Valid DFS traversal | Matches expected | 91.566 ms | Pass |
| SSSP | sssp_10.txt | 10 | 16 | Positive weighted adjacency list (CSR) | 0 | Shortest distances | Matches expected | 0.000 ms | Pass |
| SSSP | sssp_100.txt | 100 | 200 | Positive weighted adjacency list (CSR) | 0 | Shortest distances | Matches expected | 0.000 ms | Pass |
| SSSP | sssp_10000.txt | 10,000 | 20,000 | Positive weighted adjacency list (CSR) | 0 | Shortest distances | Matches expected | 23.589 ms | Pass |
| SSSP | sssp_50000.txt | 50,000 | 100,000 | Positive weighted adjacency list (CSR) | 0 | Shortest distances | Matches expected | 136.978 ms | Pass |
| SSSP | sssp_100000.txt | 100,000 | 200,000 | Positive weighted adjacency list (CSR) | 0 | Shortest distances | Matches expected | 319.434 ms | Pass |

### Complexity

- **CSR Conversion:** Time complexity $O(V + E)$, Space complexity $O(V + E)$
- **BFS / DFS:** Time complexity $O(V + E)$, Space complexity $O(V + E)$
- **Dijkstra (SSSP):** Time complexity $O((V + E) \log V)$ with binary heap, Space complexity $O(V + E)$

### References

- CS509 Assignment 1 Specification
- CS509 Lab Work Guidelines
