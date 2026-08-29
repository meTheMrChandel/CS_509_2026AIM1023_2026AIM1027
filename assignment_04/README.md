# CS509 Assignment 4 - K-Means & FastMap (Buddy Tasks)

This repository contains the C++ implementation of the **K-Means Clustering** and **FastMap** algorithms (Buddy Tasks), as defined in CS509 Assignment 4.

## Project Structure

*   [`driver/main.cpp`](driver/main.cpp): Command-line interface driver that handles interactive user prompts for algorithm selection and input filepath.
*   [`include/kmeans.h`](include/kmeans.h) / [`src/kmeans.cpp`](src/kmeans.cpp): Implementation of the K-Means clustering algorithm, point assignment, centroid update, WCSS calculation, and convergence checking.
*   [`include/fastmap.h`](include/fastmap.h) / [`src/fastmap.cpp`](src/fastmap.cpp): Implementation of the FastMap heuristic algorithm, pivot selection, law of cosines coordinate projection, distance matrix deflation, and average distance error calculation.
*   [`include/timer.h`](include/timer.h) / [`src/timer.cpp`](src/timer.cpp): High-resolution timer utility using `std::chrono::high_resolution_clock`.
*   [`Makefile`](Makefile): Compilation configuration.
*   [`tests/`](tests/): Test configuration files for K-Means (`km_example.txt`) and FastMap (`fm_01.txt` to `fm_03.txt`, `fm_example.txt`).

---

## Execution Guide

### Prerequisites
- A C++ compiler supporting C++17 (e.g., GCC / MinGW `g++`).
- GNU Make or MinGW Make (`mingw32-make`).

### 1. Build the Project
Open a terminal in the `assignment_04` directory and execute `make` (or `mingw32-make` on Windows):
```bash
mingw32-make
```
This compiles the C++ files and produces the executable `main.exe` (or `main` on Unix).

To clean build artifacts, run:
```bash
mingw32-make clean
```

### 2. Run the Interactive Executable
Execute the compiled binary:
```bash
.\main.exe
```

When prompted:
1.  **Select the Algorithm**: Enter `3` for K-Means Clustering or `4` for FastMap.
2.  **Select the Input File**: Enter the relative path to the test input file (e.g. `tests/km_example.txt` or `tests/fm_01.txt`).

### 3. Example Interactive Flows

#### K-Means Clustering Example
```text
=========================================
Select the Algorithm:
1. K-Means Clustering
2. FastMap
=========================================
Enter choice (1-2): 1
Enter the path to the input file: tests/km_example.txt

Algorithm: K-Means Clustering
K: 2
Point assignments:
0 0
1 0
2 1
3 1
4 1
5 1
Final centroids:
0: 1.250000 1.500000
1: 4.000000 5.250000
WCSS: 7.875000
Iterations: 3
Converged: true
Execution time: 0.0016 ms
```

#### FastMap Example
```text
=========================================
Select the Algorithm:
1. K-Means Clustering
2. FastMap
=========================================
Enter choice (1-2): 2
Enter the path to the input file: tests/fm_01.txt

Algorithm: FastMap
Target dimensions: 2
Pivots per dimension:
Dim 1: 8 4
Dim 2: 2 7
Object coordinates:
0: 104.929139 66.807972
1: 105.360429 36.229944
2: 66.787748 0.000000
3: 108.052744 43.659572
4: 140.126791 59.093231
5: 72.638029 49.234126
6: 88.424993 70.882808
7: 113.330013 106.293637
8: 0.000000 59.093231
9: 38.833908 91.926319
Average distance error: 29.349451
Execution time: 0.0017 ms
```

---

## Timing and Measurement Strategy

To accurately report algorithm runtime, execution time is measured using `std::chrono::high_resolution_clock`. 

File parsing, input validation, post-algorithm metric computation (such as average pairwise distance error in FastMap or WCSS in K-Means), and terminal formatting/I/O are strictly excluded from the timed section (`start_timer()` / `stop_timer()`). Only the core iterative algorithm loops (pivot selection, law of cosines projections, matrix deflation for FastMap; assignment step, centroid updates, and convergence checks for K-Means) are included inside the timed block.

---

## Validation and Error Handling

Per the assignment specifications (Section 12), the input readers strictly validate input datasets and reject malformed files with clear error messages:

### FastMap Validation
- **Matrix Dimensions**: Rejects non-square matrices or files failing to read $N \times N$ values.
- **Dimensionality**: Rejects target dimensions $K \le 0$ or $K \ge N$.
- **Distance Properties**: Rejects non-symmetric matrices ($d(i,j) \neq d(j,i)$), non-zero diagonal entries ($d(i,i) \neq 0$), and negative distances.

### K-Means Validation
- **Dataset Dimensions**: Rejects non-positive parameters ($N \le 0$, $D \le 0$, $K \le 0$) or cluster counts exceeding point counts ($K > N$).
- **Configuration Parameters**: Rejects missing or non-positive `MAX_ITERATIONS` and `TOLERANCE` parameters.

---

## Experimental Results

The tables below summarize the performance metrics and experimental results for the Buddy Tasks:

### FastMap Results

| File | Objects ($N$) | Target Dim ($K$) | Pivots per Dimension | Avg Distance Error | Execution Time | Status |
| :--- | :---: | :---: | :--- | :---: | :---: | :---: |
| `fm_example.txt` | 5 | 2 | Dim 1: (3, 1)<br>Dim 2: (0, 4) | 0.364803 | 0.0018 ms | Pass |
| `fm_01.txt` | 10 | 2 | Dim 1: (8, 4)<br>Dim 2: (2, 7) | 29.349451 | 0.0017 ms | Pass |
| `fm_02.txt` | 100 | 2 | Dim 1: (30, 31)<br>Dim 2: (53, 0) | 36.550673 | 0.0481 ms | Pass |
| `fm_03.txt` | 1,000 | 3 | Dim 1: (381, 282)<br>Dim 2: (128, 831)<br>Dim 3: (201, 167) | 22.111168 | 7.0234 ms | Pass |

### K-Means Clustering Results

| File | Points ($N$) | Dimensions ($D$) | Clusters ($K$) | WCSS | Iterations | Converged? | Execution Time | Status |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| `km_example.txt` | 6 | 2 | 2 | 7.875000 | 3 | Yes | 0.0016 ms | Pass |
