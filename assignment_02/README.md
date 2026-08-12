# CS509 Laboratory Repository - Assignment 02

## Repository Overview

This repository contains buddy (pair) assignments for CS509. Assignment 02 implements Graph Analytics algorithms (Triangle Counting and Connected Components) on undirected graphs stored in standard Adjacency List format.

## Student / Pair Details

- **Student 1:** Vishal Chandel (2026AIM1023)
- **Student 2:** Kuber Pathak (2026AIM1027)
- **Program:** M.Tech Artificial Intelligence, IIT Ropar

## Language and Environment

- **Language:** C++17
- **Compiler:** MinGW g++ (GCC)
- **Build Tool:** mingw32-make
- **Timing Method:** `std::chrono::high_resolution_clock`, reported in microseconds (us)
- **Platform:** Windows 10/11

## Directory Structure

```
CS_509_2026AIM1023_2026AIM1023/
|-- README.md
|-- common_wrapper/
|   |-- wrapper.cpp
|   `-- Makefile
|-- assignment_02/
    |-- driver/
    |   `-- driver.cpp
    |-- include/
    |   |-- betweenness_centrality.h
    |   |-- connected_components.h
    |   |-- graph.h
    |   |-- timer.h
    |   `-- triangle_counting.h
    |-- src/
    |   |-- betweenness_centrality.cpp 
    |   |-- connected_components.cpp
    |   |-- graph.cpp
    |   |-- timer.cpp
    |   `-- triangle_counting.cpp
    |-- tests/
    |   |-- tc_10.txt
    |   |-- tc_100.txt
    |   |-- tc_10,000.txt
    |   |-- tc_50,000.txt
    |   |-- tc_100,000.txt
    |   |-- cc_10.txt
    |   |-- cc_100.txt
    |   |-- cc_10,000.txt
    |   |-- cc_50,000.txt
    |   `-- cc_100,000.txt
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
- Assignment selection menu (Assignment 01 or 02)
- Compile selected assignment
- Run selected assignment (Interactive mode - select algorithm and enter file name manually)

---

## Assignment 02 - Graph Analytics

### Assignment Mode

Double / Buddy

### Objective

Implement undirected graph analytics (Triangle Counting and Connected Components) directly using Adjacency List representation.

### Algorithm / Approach

- **Graph Loader:** Reads and validates space-separated undirected adjacency list input. Checks for invalid degree values and rejects negative edge weights. Sorts neighbors of each vertex to optimize adjacency scans.
- **Triangle Counting:** Implements an optimized ordered search ($u < v < w$). For each vertex $u$, iterates through its neighbors $v > u$, then through $w > v$. A binary search is performed on $v$'s neighbor list to check if edge $(v, w)$ exists. This prevents triple counting and avoids divisions. For $V \le 100$, lists all unique triangles in sorted lexicographical order.
- **Connected Components:** Implements component finding via repeated Breadth-First Search (BFS) directly on the Adjacency List. Component IDs are assigned from 0 in the order they are first discovered.
- **Betweenness Centrality:** Stub files are created and left blank for your buddy to implement.

### Input Format

Undirected Unweighted Adjacency List:
```
V E
u degree neighbor1 neighbor2 ...
...
```

Undirected Weighted Adjacency List (Weights must be non-negative):
```
V E
u degree neighbor1 weight1 neighbor2 weight2 ...
...
```

### Compilation

```powershell
cd assignment_02
mingw32-make
```

### Execution

```powershell
# Run a specific algorithm on a test file
# Prompts for algorithm and file if no command line arguments are provided:
.\graph_simulator.exe

# Command line execution:
.\graph_simulator.exe tc tests/tc_10.txt
.\graph_simulator.exe cc tests/cc_10.txt
```

### Test Cases and Result Table

All benchmarks were run locally on the target platform. Timings are reported in microseconds (us) to measure execution times with high precision.

| Algorithm | Test File | Vertices | Edges | Expected Output | Actual Output | Time (us) | Status |
|-----------|-----------|----------|-------|-----------------|---------------|-----------|--------|
| Triangle Counting | tc_10.txt | 10 | 35 | 55 Triangles | Matches expected | < 1 us | Pass |
| Triangle Counting | tc_100.txt | 100 | 350 | 55 Triangles | Matches expected | < 1 us | Pass |
| Triangle Counting | tc_10000.txt | 10,000 | 35,000 | 64 Triangles | Matches expected | 9,001 us | Pass |
| Triangle Counting | tc_50000.txt | 50,000 | 175,000 | 49 Triangles | Matches expected | 49,002 us | Pass |
| Triangle Counting | tc_100000.txt | 100,000 | 350,000 | 54 Triangles | Matches expected | 124,239 us | Pass |
| Connected Components | cc_10.txt | 10 | 25 | 1 Component | Matches expected | < 1 us | Pass |
| Connected Components | cc_100.txt | 100 | 250 | 1 Component | Matches expected | < 1 us | Pass |
| Connected Components | cc_10000.txt | 10,000 | 25,000 | 1 Component | Matches expected | 2,004 us | Pass |
| Connected Components | cc_50000.txt | 50,000 | 125,000 | 1 Component | Matches expected | 12,994 us | Pass |
| Connected Components | cc_100000.txt | 100,000 | 250,000 | 1 Component | Matches expected | 35,203 us | Pass |
| Betweenness Centrality | - | - | - | - | Stub for Buddy | - | - |

### Complexity

- **Graph Loader / Sorting:** Time complexity $O(V + E \log(d_{max}))$, Space complexity $O(V + E)$
- **Triangle Counting:** Time complexity $O(\sum_{u} d_u^2 \log(d_{max}))$, Space complexity $O(V + E)$
- **Connected Components:** Time complexity $O(V + E)$, Space complexity $O(V + E)$

### References

- CS509 Assignment 2 Specification
- CS509 Lab Work Guidelines
