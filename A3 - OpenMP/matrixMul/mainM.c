#include "matrixMul.h"
#include <omp.h>

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

    // Dynamically allocate memory for the matrices
    int** A = (int**)malloc(N * sizeof(int*));
    int** B = (int**)malloc(N * sizeof(int*));
    int** C = (int**)malloc(N * sizeof(int*));
    
    for (int i = 0; i < N; ++i) {
        A[i] = (int*)malloc(N * sizeof(int));
        B[i] = (int*)malloc(N * sizeof(int));
        C[i] = (int*)malloc(N * sizeof(int));
    }

    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    printf("Matrices allocated successfully.\n");

    // Initialize matrices A and B
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    printf("Matrices initialized successfully.\n");

    omp_set_num_threads(threadCount);

    double startTime = omp_get_wtime();
    matrixMultiply(A, B, C, N);
    double endTime = omp_get_wtime();

    int resultCorrect = C[0][0] == N;

    printf("Matrix multiplication complete!\n");
    printf("Threads: %d\n", threadCount);
    printf("Result check: C[0][0] = %d\n", C[0][0]);
    printf("Result: %s\n", resultCorrect ? "correct" : "incorrect");
    printf("Elapsed Time: %.6f seconds\n", endTime - startTime);

    // Free dynamically allocated memory
    for (int i = 0; i < N; ++i) 
    {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    
    free(A);
    free(B);
    free(C);

    return resultCorrect ? EXIT_SUCCESS : EXIT_FAILURE;
}
