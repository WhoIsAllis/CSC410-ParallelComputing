#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100000000

long long sumArray(int arr[], int size)
{
    long long sum = 0;

    for (int i = 0; i < size; i++) 
    {
        sum += arr[i];
    }

    return sum;
}

int main()
{
    int *arr = malloc(SIZE * sizeof(int));
    if (arr == NULL) 
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < SIZE; i++) 
    {
        arr[i] = i + 1;
    }

    struct timespec startTime, endTime;
    clock_gettime(CLOCK_MONOTONIC, &startTime);

    long long totalSum = sumArray(arr, SIZE);

    clock_gettime(CLOCK_MONOTONIC, &endTime);

    double elapsed = (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_nsec - startTime.tv_nsec) / 1000000000.0;

    printf("Total Sum: %lld\n", totalSum);
    printf("Elapsed Time: %.4f seconds\n", elapsed);

    free(arr);
    return 0;
}
