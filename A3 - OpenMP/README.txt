CSC 410 A3 - OpenMP
--------------------------------------

Requirements
--------------------------------------

- GCC with OpenMP support
- C11 support
- PowerShell or another command-line terminal

Building Note:  The -O2 option enables compiler optimization and -fopenmp enables OpenMP.


Task 1 - Array Sum:
--------------------------------------

Build:

gcc -O2 -std=c11 -Wall -Wextra -Wpedantic -fopenmp .\arraysum.c -o .\arraysum.exe

Run with a selected number of threads:

.\arraysum.exe 1
.\arraysum.exe 2
.\arraysum.exe 5
.\arraysum.exe 7
.\arraysum.exe 9

Task 2 - Matrix Multiplication
--------------------------------------

Build:

gcc -O2 -std=c11 -Wall -Wextra -Wpedantic -fopenmp .\matrixMul\mainM.c -o .\matrixMul\matrixMul.exe

Run:

.\matrixMul\matrixMul.exe 1
.\matrixMul\matrixMul.exe 2
.\matrixMul\matrixMul.exe 5
.\matrixMul\matrixMul.exe 7
.\matrixMul\matrixMul.exe 9


Task 3 - Trapezoidal Rule Integration
--------------------------------------

Build:

gcc -O2 -std=c11 -Wall -Wextra -Wpedantic -fopenmp .\numIntegrate.c -o .\numIntegrate.exe

Run:

.\numIntegrate.exe 1
.\numIntegrate.exe 2
.\numIntegrate.exe 5
.\numIntegrate.exe 7
.\numIntegrate.exe 9


Task 4 - N-Queens
--------------------------------------

Build:

gcc -O2 -std=c11 -Wall -Wextra -Wpedantic -fopenmp .\Nqueens\mainQ.c -o .\Nqueens\nqueens.exe

Run:

.\Nqueens\nqueens.exe 1
.\Nqueens\nqueens.exe 2
.\Nqueens\nqueens.exe 5
.\Nqueens\nqueens.exe 7
.\Nqueens\nqueens.exe 9