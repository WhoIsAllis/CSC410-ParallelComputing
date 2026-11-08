#include "sorts.h"
#include <time.h>

int main() 
{
    srand(time(NULL));

    // Dynamically allocate arrays
    int *arr1 = (int*)malloc(SIZE * sizeof(int));
    int *arr2 = (int*)malloc(SIZE * sizeof(int));

    if (!arr1 || !arr2) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Populate arrays with random values
    for (int i = 0; i < SIZE; i++) {
        int val = rand() % MAX_VAL;
        arr1[i] = val;
        arr2[i] = val;
    }

    printf("Original array:\n");
    printArray(arr1, SIZE);

    clock_t mergeStart = clock();
    mergeSort(arr1, 0, SIZE - 1);
    clock_t mergeEnd = clock();
    double mergeElapsed = (double)(mergeEnd - mergeStart) / CLOCKS_PER_SEC;
    printf("Merge Sort Elapsed Time: %.6f seconds\n", mergeElapsed);
    printf("Sorted with Merge Sort:\n");
    printArray(arr1, SIZE);

    clock_t bubbleStart = clock();
    bubbleSort(arr2, SIZE);
    clock_t bubbleEnd = clock();
    double bubbleElapsed = (double)(bubbleEnd - bubbleStart) / CLOCKS_PER_SEC;
    printf("Bubble Sort Elapsed Time: %.6f seconds\n", bubbleElapsed);
    printf("Sorted with Bubble Sort:\n");
    printArray(arr2, SIZE);

    free(arr1);
    free(arr2);

    return 0;
}
