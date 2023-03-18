#include <stdio.h>

void cross_product(float *a, float *b, float *result);

int main() {
    float a[] = {1, 2, 3};
    float b[] = {4, 5, 6};
    float result[3];

    cross_product(a, b, result);

    printf("O produto vetorial entre os vetores [1, 2, 3] e [4, 5, 6] é: [%.2f, %.2f, %.2f]\n", result[0], result[1], result[2]);

    return 0;
}

void cross_product(float *a, float *b, float *result) {
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

