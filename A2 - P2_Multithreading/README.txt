CSC 410 - Assignment 2 Part 2: Multithreading with pthreads
==============================================================

Requirements
------------

- GCC with C11 and pthread support
- Windows PowerShell or another terminal capable of running GCC

All commands below should be run from the "A2 - P2_Multithreading"
directory. The -O2 option enables compiler optimization for the timing
runs. The remaining options select C11 and link
the pthread library.

Parallel Array Sum - Task 1
---------------------------

Build:

gcc -O2 -std=c11 -Wall -Wextra -Wpedantic -pthread .\sum_T.c -o .\sum_T.exe

Run:

.\sum_T.exe <threads>

Examples:

.\sum_T.exe 1
.\sum_T.exe 2
.\sum_T.exe 3
.\sum_T.exe 4

Parallel Matrix Multiplication - Task 2
---------------------------------------

Build:

gcc -O2 -std=c11 -Wall -Wextra -Wpedantic -pthread .\matrixT.c -o .\matrixT.exe

Run:

.\matrixT.exe <threads>

Examples:

.\matrixT.exe 1
.\matrixT.exe 2
.\matrixT.exe 3
.\matrixT.exe 4