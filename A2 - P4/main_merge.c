#include "p_merge.h"

int main(int argc, char* argv[])
{
    int approach = 1;
    int threadCount = NUM_THREADS;

    if (argc > 3)
    {
        printf("Usage: %s [approach: 1-3] [threads: 1-%d]\n", argv[0], NUM_THREADS);
        return 1;
    }

    if (argc >= 2)
    {
        approach = atoi(argv[1]);
    }

    if (argc == 3)
    {
        threadCount = atoi(argv[2]);
    }

    if (approach < 1 || approach > 3 || threadCount < 1 || threadCount > NUM_THREADS)
    {
        printf("Usage: %s [approach: 1-3] [threads: 1-%d]\n", argv[0], NUM_THREADS);
        return 1;
    }

    int* arr1 = (int*)malloc(SIZE * sizeof(int));
    int* arr2 = (int*)malloc(SIZE * sizeof(int));

    if (arr1 == NULL || arr2 == NULL)
    {
        printf("Memory allocation failed.\n");
        free(arr1);
        free(arr2);
        return 1;
    }

    for (int i = 0; i < SIZE; i++) 
    {
        int val = rand() % MAX_VAL;
        arr1[i] = val;
        arr2[i] = val;
    }

    merge_sort_seq(arr1, 0, SIZE - 1);

    if (approach == 1)
    {
        mergeSortSegment(arr2, 0, SIZE - 1, threadCount);
    }

    else if (approach == 2)
    {
        mergeSortRecursive(arr2, 0, SIZE - 1, threadCount);
    }
    
    else
    {
        mergeSortPool(arr2, 0, SIZE - 1, threadCount);
    }

    printf("Approach %d with %d thread(s)\n", approach, threadCount);

    int correct = 1;
    for (int i = 0; i < SIZE; i++)
    {
        if (arr1[i] != arr2[i])
        {
            correct = 0;
            break;
        }
    }
    printf("Correctly sorted: %s\n", correct ? "Yes" : "No");

    free(arr1);
    free(arr2);

    return 0;
}
