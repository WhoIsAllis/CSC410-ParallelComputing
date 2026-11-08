#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef SIZE
#define SIZE 1000000000LL
#endif

static long long sumArray(const int arr[], long long size)
{
    long long sum = 0;

    #pragma omp parallel for reduction(+:sum) schedule(static)
    for (long long i = 0; i < size; i++)
    {
        sum += arr[i];
    }

    return sum;
}

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
        fprintf(stderr, "Please use a positive value of threads!\n");
        return EXIT_FAILURE;
    }

    int *arr = malloc((size_t)SIZE * sizeof *arr);
    if (arr == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    for (long long i = 0; i < SIZE; i++)
    {
        arr[i] = (int)(i + 1);
    }

    omp_set_num_threads(threadCount);

    double startTime = omp_get_wtime();
    long long totalSum = sumArray(arr, SIZE);
    double endTime = omp_get_wtime();

    long long expectedSum = SIZE * (SIZE + 1LL) / 2LL;

    printf("Array Size: %lld\n", (long long)SIZE);
    printf("Threads: %d\n", threadCount);
    printf("Total Sum: %lld\n", totalSum);
    printf("Expected Sum: %lld\n", expectedSum);
    printf("Result: %s\n", totalSum == expectedSum ? "correct" : "incorrect");
    printf("Elapsed Time: %.6f seconds\n", endTime - startTime);

    free(arr);
    return totalSum == expectedSum ? EXIT_SUCCESS : EXIT_FAILURE;
}
