#include <stdio.h>
/**/
/* Para calcular o determinante de uma matriz de ordem n x n, você pode
 * implementar o algoritmo de eliminação de Gauss para reduzir a matriz a uma
 * forma triangular superior e, em seguida, calcular o determinante como o
 * produto dos elementos na diagonal principal. Segue um exemplo de função em
 * C que calcula o determinante de uma matriz quadrada de ordem n x n
 * utilizando o algoritmo de eliminação de Gauss : */
/**/

float determinant(float mat[][10], int n);

int main()
{
    int n = 4;
    float mat[][10] = {
        // clang-format off
        {2,  3,  1,  5},
        {4,  1,  3,  2},
        {7,  0,  2,  6},
        {1,  5,  4,  3} // clang-format on
    };

    float det = determinant(mat, n);

    printf("O determinante da matriz é: %.2f\n", det);

    return 0;
}

float determinant(float mat[][10], int n)
{
    float det = 1;
    int i, j, k;

    // Eliminação de Gauss
    for (i = 0; i < n - 1; i++)
    {
        // Pivô
        if (mat[i][i] == 0)
        {
            int c = 1;
            while ((i + c) < n && mat[i + c][i] == 0)
            {
                c++;
            }
            if ((i + c) == n)
            {
                return 0;
            }
            for (j = i; j < n; j++)
            {
                float temp = mat[i][j];
                mat[i][j] = mat[i + c][j];
                mat[i + c][j] = temp;
            }
            det *= -1;
        }

        for (j = i + 1; j < n; j++)
        {
            float factor = mat[j][i] / mat[i][i];
            for (k = i; k < n; k++)
            {
                mat[j][k] -= factor * mat[i][k];
            }
        }
    }

    // Cálculo do determinante
    for (i = 0; i < n; i++)
    {
        det *= mat[i][i];
    }

    return det;
}
