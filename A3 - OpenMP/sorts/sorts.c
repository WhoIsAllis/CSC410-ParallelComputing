#include "sorts.h"

// Merge Sort 
static void merge(int arr[], int left, int mid, int right) 
{
    int len = right - left + 1;
    int *tmp = malloc(len * sizeof *tmp);
    int i = left;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= right) 
    {
        if (arr[i] <= arr[j]) 
        {
            tmp[k] = arr[i];
            i++;
        } 
        
        else 
        {
            tmp[k] = arr[j];
            j++;
        }

        k++;
    }

    while (i <= mid) 
    {
        tmp[k] = arr[i];
        i++;
        k++;
    }

    while (j <= right) 
    {
        tmp[k] = arr[j];
        j++;
        k++;
    }

    for (int index = 0; index < len; index++) 
    {
        arr[left + index] = tmp[index];
    }

    free(tmp);
}

void mergeSort(int arr[], int left, int right) 
{
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Bubble Sort
void bubbleSort(int arr[], int n) 
{
    for (int i = 0; i < n - 1; i++) 
    {
        int swap = 0;

        for (int j = 0; j < n - i - 1; j++) 
        {
            if (arr[j] > arr[j + 1]) 
            {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                swap = 1;
            }
        }

        if (!swap) 
        {
            break;
        }
    }
}

void printArray(int arr[], int n) 
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}