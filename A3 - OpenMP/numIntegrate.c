#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef N
#define N 1000000000
#endif

double f(double x) {
    return 4.0 / (1.0 + x * x); // Function to integrate
}

double trapezoidalRule() 
{
    double lowBound = 0.0;
    double highBound = 1.0;
    double width = (highBound - lowBound) / N;
    double sum = 0.5 * (f(lowBound) + f(highBound));

    #pragma omp parallel for reduction(+:sum) schedule(static)

    for (int i = 1; i < N; i++) 
    {
        double x = lowBound + i * width;
        sum += f(x);
    }

    return sum * width;
}

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
        fprintf(stderr, "Please use a positive number of threads!\n");
        return EXIT_FAILURE;
    }

    omp_set_num_threads(threadCount);

    double startTime = omp_get_wtime();
    double pi = trapezoidalRule();
    double endTime = omp_get_wtime();

    printf("Intervals: %d\n", N);
    printf("Threads: %d\n", threadCount);
    printf("Estimated value of pi: %.10f\n", pi);
    printf("Elapsed Time: %.6f seconds\n", endTime - startTime);

    return 0;
}
