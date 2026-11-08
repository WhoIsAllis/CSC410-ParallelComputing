#include <stdbool.h>
#include <stdlib.h>

bool isSafe(int board[], int row, int col) 
{
    for (int i = 0; i < col; i++) {
        if (board[i] == row || abs(board[i] - row) == abs(i - col)) {
            return false;
        }
    }
    return true;
}

long long solveNQueensUtil(int board[], int col, int n) 
{
    if(col == n)
    {
        return 1;
    }

    long long solutionCount = 0;

    for (int row = 0; row < n; row++)
    {
        if (isSafe(board, row, col))
        {
            board[col] = row;
            solutionCount += solveNQueensUtil(board, col + 1, n);
            board[col] = -1;
        }
    }

    return solutionCount;
}

long long solveNQueens(int n)
{
    long long totalSolutions = 0;

    #pragma omp parallel for reduction(+:totalSolutions) schedule(dynamic)
    for (int row = 0; row < n; row++)
    {
        int board[n];

        for (int i = 0; i < n; i++)
        {
            board[i] = -1;
        }

        board[0] = row;
        totalSolutions += solveNQueensUtil(board, 1, n);
    }

    return totalSolutions;
}
