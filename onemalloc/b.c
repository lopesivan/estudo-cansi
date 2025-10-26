#include <stdio.h>
#include <stdlib.h>

static void print_array(double *array, size_t size) {

  unsigned int i;

  for (i = 0; i < size; i++) {

    printf("array[%u] = %8f \n", i, array[i]);
  }

  return;
}

void *criar_array_unico(size_t tamanho_elemento, int num_dim,
                        const size_t dims[]) {
  size_t total_elementos = 1;

  for (int i = 0; i < num_dim; i++)
    total_elementos *= dims[i];

  return malloc(total_elementos * tamanho_elemento);
}

#define INDEX2D(dim2, i, j) ((i) * (dim2) + (j))
#define INDEX3D(dim2, dim3, i, j, k)                                           \
  ((i) * (dim2) * (dim3) + (j) * (dim3) + (k))

int main() {
  size_t dims2D[] = {3, 4};

  double *matriz2d = criar_array_unico(sizeof(double), 2, dims2D);

  for (size_t i = 0; i < dims2D[0]; i++)
    for (size_t j = 0; j < dims2D[1]; j++)
      matriz2d[INDEX2D(dims2D[1], i, j)] = (double)(i * 10 + j);

  printf("Matriz 2D:\n");
  for (size_t i = 0; i < dims2D[0]; i++) {
    for (size_t j = 0; j < dims2D[1]; j++)
      printf("%.1f ", matriz2d[INDEX2D(dims2D[1], i, j)]);
    printf("\n");
  }

  free(matriz2d);

  size_t dims3D[] = {3, 2, 2};

  double *matriz3d = criar_array_unico(sizeof(double), 3, dims3D);

  for (size_t i = 0; i < dims3D[0]; i++)
    for (size_t j = 0; j < dims3D[1]; j++)
      for (size_t k = 0; k < dims3D[2]; k++)
        matriz3d[INDEX3D(dims3D[1], dims3D[2], i, j, k)] =
            (double)(i * 100 + j * 10 + k);

  printf("\nMatriz 3D:\n");
  for (size_t i = 0; i < dims3D[0]; i++) {
    for (size_t j = 0; j < dims3D[1]; j++) {
      for (size_t k = 0; k < dims3D[2]; k++)
        printf("%.1f ", matriz3d[INDEX3D(dims3D[1], dims3D[2], i, j, k)]);
      printf("\n");
    }
    printf("\n");
  }

  print_array(matriz3d, dims3D[0] * dims3D[1] * dims3D[2]);
  printf("matriz3d[1][1][1] = %.1f\n",
         matriz3d[INDEX3D(dims3D[1], dims3D[2], 1, 1, 1)]);
  free(matriz3d);

  return 0;
}
