#include <stdio.h>
#include <stdlib.h>

typedef double Scalar;

static void print_array(double *array, size_t size) {

  unsigned int i;

  for (i = 0; i < size; i++) {

    printf("array[%u] = %8f \n", i, array[i]);
  }

  return;
}

void *criar_matriz(size_t num_dims, size_t dims[]) {
  if (num_dims < 2)
    return NULL;

  size_t total_pointers = 0;
  size_t elements = 1;

  for (size_t i = 0, mult = 1; i < num_dims - 1; ++i) {
    mult *= dims[i];
    total_pointers += mult;
  }

  for (size_t i = 0; i < num_dims; ++i)
    elements *= dims[i];

  size_t pointers_size = total_pointers * sizeof(void *);
  size_t data_size = elements * sizeof(Scalar);

  void **layers = malloc(pointers_size + data_size);
  if (!layers)
    return NULL;

  Scalar *data = (Scalar *)((char *)layers + pointers_size);

  void **ptr = layers;
  size_t next_ptr_offset = dims[0];
  size_t data_offset = 0;

  if (num_dims == 2) {
    Scalar **mat = (Scalar **)layers;
    for (size_t i = 0; i < dims[0]; ++i)
      mat[i] = data + i * dims[1];
  } else if (num_dims == 3) {
    Scalar ***mat = (Scalar ***)layers;
    Scalar **ptr2 = (Scalar **)(layers + dims[0]);

    for (size_t i = 0; i < dims[0]; ++i) {
      mat[i] = ptr2 + i * dims[1];
      for (size_t j = 0; j < dims[1]; ++j) {
        mat[i][j] = data + (i * dims[1] * dims[2]) + (j * dims[2]);
      }
    }
  } else {
    free(layers);
    return NULL;
  }

  return layers;
}

int main() {
  size_t dims3D[] = {3, 3, 3};

  Scalar ***matriz3d = criar_matriz(3, dims3D);
  if (!matriz3d) {
    printf("Erro ao alocar matriz3d\n");
    return 1;
  }

  for (size_t i = 0; i < dims3D[0]; ++i)
    for (size_t j = 0; j < dims3D[1]; ++j)
      for (size_t k = 0; k < dims3D[2]; ++k)
        matriz3d[i][j][k] = (Scalar)(i * 100 + j * 10 + k);

  printf("matriz3d[1][1][1] = %f\n",
         matriz3d[1][1][1]); // Deve imprimir 111.000000

  for (size_t i = 0; i < dims3D[0]; ++i)
    for (size_t j = 0; j < dims3D[1]; ++j)
      for (size_t k = 0; k < dims3D[2]; ++k)
        printf("matriz3d[%lu][%lu][%lu] = %f\n", i, j, k, matriz3d[i][j][k]);

  print_array(**matriz3d, dims3D[0] * dims3D[1] * dims3D[2]);
  free(matriz3d);

  return 0;
}
