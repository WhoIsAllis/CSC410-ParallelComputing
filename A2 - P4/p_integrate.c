#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000000000 // intervals
#define NUM_THREADS 1

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

// use this shared global variable
double total_sum = 0.0;

void *parallel_trapezoidalRule(void *arg) 
{
   
}

int main(int argc, char *argv[]) 
{

    printf("Result of numerical integration: %f\n", total_sum);

    return 0;
}
