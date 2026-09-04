#include <stdio.h>

#define N 100000 // intervals

double f(double x) {
    return 4.0 / (1.0 + x * x); // Function to integrate
}

double trapezoidalRule() 
{
    // complete this
}

int main() {
    double pi = trapezoidalRule();
    printf("Estimated value of π: %f\n", pi);
    return 0;
}
