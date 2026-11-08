#include "nqueens.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef N
#define N 15
#endif

int main(int argc, char *argv[]) 
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <positive number of threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int threadCount = atoi(argv[1]);
    if (threadCount < 1)
    {
        fprintf(stderr, "Please have a positive amount of threads!\n");
        return EXIT_FAILURE;
    }

    omp_set_num_threads(threadCount);

    double startTime = omp_get_wtime();
    long long solutionCount = solveNQueens(N);
    double endTime = omp_get_wtime();

    printf("Board Size: %d\n", N);
    printf("Threads: %d\n", threadCount);
    printf("Solutions: %lld\n", solutionCount);
    printf("Elapsed Time: %.6f seconds\n", endTime - startTime);

    return 0;
}
