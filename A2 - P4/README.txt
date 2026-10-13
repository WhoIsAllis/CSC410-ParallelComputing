CSC 410 - Assignment 2 Part 4: Synchronization of Threads
==========================================================

Requirements
------------

- GCC with C11 and pthread support
- Windows PowerShell with WSL, or another Linux terminal

Run all commands below from the "A2 - P4" directory. The -O2 option enables compiler optimization.


Task 1 - Parallel Numerical Integration
---------------------------------------

Build:

wsl --cd "$PWD" gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -pthread p_integrate.c -o p_integrate

Run:

wsl --cd "$PWD" ./p_integrate <threads>

The thread count will be from 1 through 4.

Example:

wsl --cd "$PWD" ./p_integrate 4


Task 2 - Parallel Merge Sort
----------------------------

Build:

wsl --cd "$PWD" gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -pthread main_merge.c p_merge.c -o p_merge

Run:

wsl --cd "$PWD" ./p_merge <approach> <threads>

Approaches:

1 - Segment-based
2 - Recursive thread creation
3 - Thread pool

The thread count will be from 1 through 4.

Examples:

wsl --cd "$PWD" ./p_merge 1 4
wsl --cd "$PWD" ./p_merge 2 4
wsl --cd "$PWD" ./p_merge 3 4