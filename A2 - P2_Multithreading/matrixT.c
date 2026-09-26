#include "matrixT.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **A;
int **B;
int **C;
int threadCount = NUM_THREADS;

int **allocateMatrix(void)
{
    int **matrix = malloc(N * sizeof(int *));
    if (matrix == NULL) 
    {
        return NULL;
    }

    for (int i = 0; i < N; i++) 
    {
        matrix[i] = malloc(N * sizeof(int));
        if (matrix[i] == NULL) 
        {
            for (int j = 0; j < i; j++) 
            {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }

    return matrix;
}

void freeMatrix(int **matrix)
{
    for (int i = 0; i < N; i++) 
    {
        free(matrix[i]);
    }
    free(matrix);
}

void *matrixMultiplyThread(void *arg)
{
    int threadId = *(int *)arg;
    int rowsPerThread = N / threadCount;
    int startRow = threadId * rowsPerThread;
    int endRow = (threadId == threadCount - 1) ? N : startRow + rowsPerThread;

    for (int i = startRow; i < endRow; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

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

    A = allocateMatrix();
    B = allocateMatrix();
    C = allocateMatrix();

    if (A == NULL || B == NULL || C == NULL) 
    {
        printf("Memory allocation failed.\n");
        if (A != NULL) freeMatrix(A);
        if (B != NULL) freeMatrix(B);
        if (C != NULL) freeMatrix(C);
        return 1;
    }

    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];
    struct timespec startTime, endTime;

    clock_gettime(CLOCK_MONOTONIC, &startTime);

    for (int i = 0; i < threadCount; i++) 
    {
        threadIds[i] = i;
        if (pthread_create(&threads[i], NULL, matrixMultiplyThread, &threadIds[i]) != 0) 
        {
            printf("Thread creation failed.\n");
            return 1;
        }
    }

    for (int i = 0; i < threadCount; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &endTime);

    double elapsed = (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_nsec - startTime.tv_nsec) / 1000000000.0;

    printf("Matrix multiplication complete.\n");
    printf("Threads: %d\n", threadCount);
    printf("Elapsed Time: %.4f seconds\n", elapsed);

    freeMatrix(A);
    freeMatrix(B);
    freeMatrix(C);

    return 0;
}
