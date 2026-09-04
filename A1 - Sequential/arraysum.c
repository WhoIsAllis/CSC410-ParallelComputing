#include <stdio.h>
#include <time.h>

#define SIZE 60

int sumArray(int arr[], int size) 
{
    int sum = 0;
    for (int i = 0; i < size; i++) 
    {
        sum += arr[i];
    }
    return sum;
}

int main() 
{
    int arr[SIZE];
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1; 
    }

    clock_t start = clock();
    int totalSum = sumArray(arr, SIZE);
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Total Sum: %d\n", totalSum);
    printf("Elapsed Time: %.6f seconds\n", elapsed);

    return 0;
}
