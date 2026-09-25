// Parallel array sum using pthreads

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100000000
#define NUM_THREADS 4

int *array;
int threadCount = NUM_THREADS;
long long partialSums[NUM_THREADS];

void *sumArrayThread(void *arg)
{
    int threadId = *(int *)arg;
    int itemsPerThread = SIZE / threadCount;
    int start = threadId * itemsPerThread;
    int end = (threadId == threadCount - 1) ? SIZE : start + itemsPerThread;
    long long sum = 0;

    for (int i = start; i < end; i++) 
    {
        sum += array[i];
    }

    partialSums[threadId] = sum;
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc > 2) 
    {
        printf("Usage: %s [number of threads: 1-%d]\n", argv[0], NUM_THREADS);
        return 1;
    }

    if (argc == 2) 
    {
        threadCount = atoi(argv[1]);
    }

    if (threadCount < 1 || threadCount > NUM_THREADS) 
    {
        printf("Usage: %s [Number of Threads: 1-%d]\n", argv[0], NUM_THREADS);
        return 1;
    }

    array = malloc(SIZE * sizeof(int));
    if (array == NULL) 
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < SIZE; i++) 
    {
        array[i] = i + 1;
    }

    pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];
    struct timespec startTime, endTime;

    clock_gettime(CLOCK_MONOTONIC, &startTime);

    for (int i = 0; i < threadCount; i++) 
    {
        threadIds[i] = i;
        if (pthread_create(&threads[i], NULL, sumArrayThread, &threadIds[i]) != 0) 
        {
            printf("Thread creation failed.\n");
            return 1;
        }
    }

    for (int i = 0; i < threadCount; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    long long totalSum = 0;
    for (int i = 0; i < threadCount; i++) 
    {
        totalSum += partialSums[i];
    }

    clock_gettime(CLOCK_MONOTONIC, &endTime);

    double elapsed = (endTime.tv_sec - startTime.tv_sec) +
                     (endTime.tv_nsec - startTime.tv_nsec) / 1000000000.0;

    printf("Total Sum: %lld\n", totalSum);
    printf("Threads: %d\n", threadCount);
    printf("Elapsed Time: %.4f seconds\n", elapsed);

    free(array);
    return 0;
}
