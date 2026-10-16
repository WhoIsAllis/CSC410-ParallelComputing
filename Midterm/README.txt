CSC 410 - Midterm Part 2: Parallel Computing
=============================================

Requirements
------------

- GCC with pthread and OpenMP support
- Windows PowerShell with MSYS2 GCC, or another compatible terminal

Run all commands below from the "Midterm" directory. The -O2 option enables compiler optimization.


Task 1 - Sobel Edge Detection
-----------------------------

Build the sequential version:
gcc -std=gnu11 -Wall -Wextra -Wpedantic -O2 sobel.c -lm -o sobel

Build the pthread version:
gcc -std=gnu11 -Wall -Wextra -Wpedantic -O2 -pthread sobel_pthread.c -lm -o sobel_pthread

Build the OpenMP version:
gcc -std=gnu11 -Wall -Wextra -Wpedantic -O2 -fopenmp sobel_openmp.c -lm -o sobel_openmp

Run:

.\sobel.exe <image>
.\sobel_pthread.exe <image> <threads>
.\sobel_openmp.exe <image> <threads>

Images:

NGGYU.jpg
NGGYU2.jpg

The parallel thread counts are 1, 2, 3, 4, and 8.

Examples:

.\sobel.exe NGGYU.jpg
.\sobel_pthread.exe NGGYU.jpg 4
.\sobel_openmp.exe NGGYU.jpg 4

Task 2 - Custom Reader-Writer Lock
----------------------------------

Build:
gcc -std=gnu11 -Wall -Wextra -Wpedantic -O2 -pthread rw.c -o rw

Run:
.\rw.exe