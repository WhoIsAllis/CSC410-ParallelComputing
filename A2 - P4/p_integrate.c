#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 1000000000 // intervals
#define NUM_THREADS 4

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

// use this shared global variable
double totalSum = 0.0;
int threadCount = NUM_THREADS;
pthread_mutex_t sumMutex = PTHREAD_MUTEX_INITIALIZER;

void *parallelTrapezoidalRule(void *arg)
{
    int threadId = *(int *)arg;
    int valuesPerThread = (N - 1) / threadCount;
    int start = 1 + threadId * valuesPerThread;
    int end = (threadId == threadCount - 1) ? N : start + valuesPerThread;
    double step = 1.0 / N;
    double localSum = 0.0;

    for (int i = start; i < end; i++)
    {
        localSum += f(i * step);
    }

    pthread_mutex_lock(&sumMutex);
    totalSum += localSum;
    pthread_mutex_unlock(&sumMutex);

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
        printf("Usage: %s [number of threads: 1-%d]\n", argv[0], NUM_THREADS);
        return 1;
    }

    pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];
    struct timespec startTime, endTime;

    clock_gettime(CLOCK_MONOTONIC, &startTime);

    for (int i = 0; i < threadCount; i++)
    {
        threadIds[i] = i;

        if (pthread_create(&threads[i], NULL, parallelTrapezoidalRule, &threadIds[i]) != 0)
        {
            printf("Thread creation failed.\n");
            return 1;
        }
    }

    for (int i = 0; i < threadCount; i++)
    {
        pthread_join(threads[i], NULL);
    }

    double step = 1.0 / N;
    totalSum = step * (0.5 * f(0.0) + totalSum + 0.5 * f(1.0));

    clock_gettime(CLOCK_MONOTONIC, &endTime);

    double elapsed = (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_nsec - startTime.tv_nsec) / 1000000000.0;

    printf("Result of numerical integration: %f\n", totalSum);
    printf("Threads: %d\n", threadCount);
    printf("Elapsed Time: %.4f seconds\n", elapsed);

    pthread_mutex_destroy(&sumMutex);

    return 0;
}
