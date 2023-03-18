#include <stdio.h>

float dot_product (float *a, float *b, int size);

int main()
{
    float a[] = {1, 2, 3};
    float b[] = {4, 5, 6};
    int size = 3;
    float result;

    result = dot_product (a, b, size);

    printf ("O produto escalar entre os vetores [1, 2, 3] e [4, 5, 6] é: %.2f\n",
            result);

    return 0;
}

float dot_product (float *a, float *b, int size)
{
    float result = 0;
    int i;

    for (i = 0; i < size; i++)
    {
        result += a[i] * b[i];
    }

    return result;
}

