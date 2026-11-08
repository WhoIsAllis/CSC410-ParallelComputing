#include <stdio.h>
#include <time.h>

#define N 1000000000 // intervals

double f(double x) {
    return 4.0 / (1.0 + x * x); // Function to integrate
}

double trapezoidalRule() 
{
    double lowBound = 0.0;
    double highBound = 1.0;
    double width = (highBound - lowBound) / N;
    double sum = 0.5 * (f(lowBound) + f(highBound));
    double answer = 0.0;

    for (int i = 1; i < N; i++) {
        double x = lowBound + i * width;
        sum += f(x);
    }

    answer = sum * width;
    return answer;
}

int main() {
    clock_t start = clock();
    double pi = trapezoidalRule();
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Elapsed Time: %.6f seconds\n", elapsed);
    printf("Estimated value of π: %f\n", pi);
    return 0;
}
