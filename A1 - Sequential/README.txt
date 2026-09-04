CSC 410 - Assignment 1: Sequential Programs
=============================================

Requirements
------------

- GCC with C11 support
- Windows PowerShell or another terminal capable of running GCC

All commands below should be run from the "A1 - Sequential" directory.
The -O2 option enables compiler optimization for the timing runs. The
remaining options select C11 and enable useful compiler warnings.

Array Sum
---------

Build:

gcc -O2 -std=c11 -Wall -Wextra -Wpedantic .\arraysum.c -o .\arraysum.exe

Run:

.\arraysum.exe

Matrix Multiplication
---------------------
Build:

gcc -O2 -std=c11 -Wall -Wextra -Wpedantic .\matrixMul\mainM.c -o .\matrixMul\matrixMul.exe

Run:

.\matrixMul\matrixMul.exe

N-Queens
--------
Build:

gcc -O2 -std=c11 -Wall -Wextra -Wpedantic .\Nqueens\mainQ.c -o .\Nqueens\nqueens.exe

Run:

.\Nqueens\nqueens.exe

Merge Sort and Bubble Sort
--------------------------
Build:

gcc -O2 -std=c11 -Wall -Wextra -Wpedantic .\sorts\mainSorts.c .\sorts\sorts.c -o .\sorts\sorts.exe

Run:

.\sorts\sorts.exe

Numerical Integration
---------------------
Build:

gcc -O2 -std=c11 -Wall -Wextra -Wpedantic .\numIntegrate.c -o .\numIntegrate.exe

Run:

.\numIntegrate.exe

Input Sizes Used for Timing
---------------------------

- Array Sum: SIZE = 500000
- Matrix Multiplication: N = 1000
- N-Queens: n = 13
- Merge Sort and Bubble Sort: SIZE = 100000
- Numerical Integration: N = 1000000000

Large arrays, matrices, and individual N-Queens solutions were not printed
during the timing runs so that terminal output was not included in the
measured execution time.
