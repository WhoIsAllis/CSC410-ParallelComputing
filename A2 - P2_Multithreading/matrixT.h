#ifndef MATRIX_T_H
#define MATRIX_T_H

#define N 1000
#define NUM_THREADS 4

int **allocateMatrix(void);
void freeMatrix(int **matrix);
void *matrixMultiplyThread(void *arg);

#endif
