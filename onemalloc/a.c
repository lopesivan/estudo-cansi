#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void* criar_array(size_t tamanho_elemento, int num_dim, ...) {
    va_list args;
    va_start(args, num_dim);

    size_t total_elementos = 1;
    size_t *dims = malloc(num_dim * sizeof(size_t));
    for(int i = 0; i < num_dim; i++){
        dims[i] = va_arg(args, size_t);
        total_elementos *= dims[i];
    }
    va_end(args);

    size_t total_tamanho = total_elementos * tamanho_elemento;

    void* bloco = malloc(total_tamanho);
    free(dims);
    return bloco;
}

#define INDEX2D(dim2, i, j) ((i)*(dim2) + (j))
#define INDEX3D(dim2, dim3, i, j, k) ((i)*(dim2)*(dim3) + (j)*(dim3) + (k))

int main() {
    size_t linhas = 3;
    size_t colunas = 4;

    double *matriz2d = criar_array(sizeof(double), 2, linhas, colunas);

    for (size_t i = 0; i < linhas; i++)
        for (size_t j = 0; j < colunas; j++)
            matriz2d[INDEX2D(colunas, i, j)] = (double)(i*10 + j);

    printf("Matriz 2D:\n");
    for (size_t i = 0; i < linhas; i++) {
        for (size_t j = 0; j < colunas; j++)
            printf("%.1f ", matriz2d[INDEX2D(colunas, i, j)]);
        printf("\n");
    }

    free(matriz2d);

    size_t d1=3, d2=2, d3=2;

    double *matriz3d = criar_array(sizeof(double), 3, d1, d2, d3);

    for (size_t i = 0; i < d1; i++)
        for (size_t j = 0; j < d2; j++)
            for (size_t k = 0; k < d3; k++)
                matriz3d[INDEX3D(d2, d3, i, j, k)] = (double)(i*100 + j*10 + k);

    printf("\nMatriz 3D:\n");
    for (size_t i = 0; i < d1; i++) {
        for (size_t j = 0; j < d2; j++) {
            for (size_t k = 0; k < d3; k++)
                printf("%.1f ", matriz3d[INDEX3D(d2, d3, i, j, k)]);
            printf("\n");
        }
        printf("\n");
    }

    free(matriz3d);

    return 0;
}

