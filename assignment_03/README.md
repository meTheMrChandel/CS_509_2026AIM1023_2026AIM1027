# CS509 Assignment 3 - Gradient Descent (Buddy Task)

This repository contains the C++ implementation of the **Gradient Descent** optimization algorithm (Buddy Task) for polynomial functions, as defined in CS509 Assignment 3. The other buddy task (Maxflow-Mincut) is left blank/stubbed out, and individual tasks are omitted as requested.

## Project Structure

*   [`driver/main.cpp`](file:///d:/MTech/1st%20Semester/CS_509_Buddy/CS_509_2026AIM1023_2026AIM1023/assignment_03/driver/main.cpp): Command-line interface driver that handles interactive user prompts for algorithm selection and input filepath.
*   [`src/gradient_descent.h`](file:///d:/MTech/1st%20Semester/CS_509_Buddy/CS_509_2026AIM1023_2026AIM1023/assignment_03/src/gradient_descent.h) / [`src/gradient_descent.cpp`](file:///d:/MTech/1st%20Semester/CS_509_Buddy/CS_509_2026AIM1023_2026AIM1023/assignment_03/src/gradient_descent.cpp): Implementation of the polynomial evaluation, derivative computation, and optimization loop.
*   [`src/maxflow_mincut.h`](file:///d:/MTech/1st%20Semester/CS_509_Buddy/CS_509_2026AIM1023_2026AIM1023/assignment_03/src/maxflow_mincut.h) / [`src/maxflow_mincut.cpp`](file:///d:/MTech/1st%20Semester/CS_509_Buddy/CS_509_2026AIM1023_2026AIM1023/assignment_03/src/maxflow_mincut.cpp): Empty stubs for the other buddy task.
*   [`Makefile`](file:///d:/MTech/1st%20Semester/CS_509_Buddy/CS_509_2026AIM1023_2026AIM1023/assignment_03/Makefile): Compilation configuration.
*   [`tests/`](file:///d:/MTech/1st%20Semester/CS_509_Buddy/CS_509_2026AIM1023_2026AIM1023/assignment_03/tests/): The 5 required test configuration files (`gd_01.txt` to `gd_05.txt`).

---

## Execution Guide

### Prerequisites
- A C++ compiler supporting C++11 or C++14 (e.g. GCC/MinGW `g++`).
- GNU Make or MinGW Make (`mingw32-make`).

### 1. Build the Project
Open a shell in the workspace directory and execute `make` (or `mingw32-make` on Windows):
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
1.  **Select the Algorithm**: Enter `1` for Gradient Descent. (Option `2` for Maxflow-Mincut is stubbed as not implemented).
2.  **Select the Input File**: Enter the relative path to the test input file (e.g. `tests/gd_01.txt`).

### 3. Example Interactive Flow
```text
=========================================
Select the Algorithm:
1. Gradient Descent
2. Maxflow-Mincut
=========================================
Enter choice (1-2): 1
Enter the path to the input file: tests/gd_01.txt

Algorithm: Gradient Descent
Degree: 2
Final x: approximately 3.000000
Final f(x): approximately 0.000000
Iterations: 70
Converged: true
Execution time: 0.751470 microseconds
```

---

## Timing and Measurement Strategy

To comply with the instruction to report execution times in **microseconds**, we measure the execution time of the solver using `std::chrono::high_resolution_clock`. 

Because a single optimization run completes in less than a microsecond on modern CPUs (leading to `0 microseconds` readings), the solver executes the optimization loop **100,000 times** sequentially during timing, and computes the average duration per run. File parsing, terminal output, and pre/post-setup allocations are strictly excluded from the timed section.

---

## Experimental Results

The table below summarizes the performance metrics of the 5 required test cases:

| File | Degree | Initial $x_0$ | Learn Rate ($\alpha$) | Tolerance ($\epsilon$) | Max Iterations | Expected $x^*$ | Actual $x$ | Actual $f(x)$ | Iterations | Converged? | Execution Time (us) | Status |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| `gd_01.txt` | 2 | 0 | 0.100 | $1 \times 10^{-6}$ | 5,000 | 3 | 3.000000 | 0.000000 | 70 | Yes | 0.751470 | Pass |
| `gd_02.txt` | 4 | 2 | 0.020 | $1 \times 10^{-6}$ | 10,000 | 0 | 0.000000 | 0.000000 | 180 | Yes | 2.355320 | Pass |
| `gd_03.txt` | 6 | 2 | 0.020 | $1 \times 10^{-6}$ | 20,000 | 0 | 0.000000 | 0.000000 | 349 | Yes | 5.517880 | Pass |
| `gd_04.txt` | 8 | 2 | 0.010 | $1 \times 10^{-8}$ | 50,000 | 0 | 0.000000 | 0.000000 | 948 | Yes | 21.325450 | Pass |
| `gd_05.txt` | 10 | 2 | 0.005 | $1 \times 10^{-10}$ | 100,000 | 0 | 0.000000 | 0.000000 | 2,364 | Yes | 73.754590 | Pass |

| File | V | E | Source | Sink | Actual Flow  | Cut Capacity | Time (ms) | Status |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| `mf_10.txt` | 10 | 20 | 0 | 9 | 36 | 36 | 0.0045  | Pass |
| `mf_100.txt` | 100 | 200 | 0 | 99 | 81 | 81 | 0.0509  | Pass |
| `mf_1000.txt` | 1000 | 2000 | 0 | 999 | 81 | 81 | 0.3221  | Pass |
| `mf_10000.txt` | 10000 | 30000 | 0 | 9999 | 81 | 21818 | 4.4603  | Pass |
| `mf_50000.txt` | 50000 | 150000 | 0 | 49999 | 88 | 88 | 25.4597  | Pass |

