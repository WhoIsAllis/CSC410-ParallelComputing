#include "nqueens.h"
#include <time.h>

int main() 
{
    int n = 5;
    int* board = (int*)malloc(n * sizeof(int));
    if (board == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        board[i] = -1;
    }

    clock_t start = clock();
    solveNQueensUtil(board, 0, n);
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Elapsed Time: %.6f seconds\n", elapsed);
    free(board);
    return 0;
}
