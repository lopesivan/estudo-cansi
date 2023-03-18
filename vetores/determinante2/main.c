
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 1000

void decompose_lu (double **a, int n, int *indx, double *d)
{
    int i, imax, j, k;
    double big, dum, sum, temp;
    double *vv;

    vv = (double *)malloc ((n + 1) * sizeof (double));

    *d = 1.0;
    for (i = 1; i <= n; i++)
    {
        big = 0.0;
        for (j = 1; j <= n; j++)
            if ((temp = fabs (a[i][j])) > big) big = temp;
        if (big == 0.0)
        {
            printf ("Singular matrix in routine ludcmp\n");
            exit (1);
        }
        vv[i] = 1.0 / big;
    }
    for (j = 1; j <= n; j++)
    {
        for (i = 1; i < j; i++)
        {
            sum = a[i][j];
            for (k = 1; k < i; k++) sum -= a[i][k] * a[k][j];
            a[i][j] = sum;
        }
        big = 0.0;
        for (i = j; i <= n; i++)
        {
            sum = a[i][j];
            for (k = 1; k < j; k++)
                sum -= a[i][k] * a[k][j];
            a[i][j] = sum;
            if ((dum = vv[i] * fabs (sum)) >= big)
            {
                big = dum;
                imax = i;
            }
        }
        if (j != imax)
        {
            for (k = 1; k <= n; k++)
            {
                dum = a[imax][k];
                a[imax][k] = a[j][k];
                a[j][k] = dum;
            }
            *d = - (*d);
            vv[imax] = vv[j];
        }
        indx[j] = imax;
        if (a[j][j] == 0.0) a[j][j] = 1.0e-20;
        if (j != n)
        {
            dum = 1.0 / (a[j][j]);
            for (i = j + 1; i <= n; i++) a[i][j] *= dum;
        }
    }
    free (vv);
}

double det (double **a, int n)
{
    int i, j, *indx;
    double d, *vv;

    indx = (int *)malloc ((n + 1) * sizeof (int));


    vv = (double *)malloc ((n + 1) * sizeof (double));

    d = 1.0;
    decompose_lu (a, n, indx, &d);

    for (i = 1; i <= n; i++)
        d *= a[i][i];

    free (indx);
    free (vv);

    return d;

}

int main()
{
    int i, j;
    double **a, result;
    clock_t start, end;


    // Alocando espaço para a matriz
    a = (double **)malloc ((N + 1) * sizeof (double *));
    for (i = 1; i <= N; i++)
    {
        a[i] = (double *)malloc ((N + 1) * sizeof (double));
    }

    // Inicializando a matriz com valores aleatórios entre -1 e 1
    for (i = 1; i <= N; i++)
    {
        for (j = 1; j <= N; j++)
        {
            a[i][j] = 2.0 * rand() / RAND_MAX - 1.0;
        }
    }

    // Calculando o determinante da matriz
    start = clock();
    result = det (a, N);
    end = clock();

    printf ("Determinant: %f\n", result);
    printf ("Time: %f seconds\n", (double) (end - start) / CLOCKS_PER_SEC);

    // Liberando a memória alocada para a matriz
    for (i = 1; i <= N; i++)
    {
        free (a[i]);
    }
    free (a);

    return 0;
}

