#include <stdio.h>
#include <stdlib.h>

#define MAX_DIM 3

typedef double Scalar;

static void print_array(double *array, size_t size) {
  for (size_t i = 0; i < size; i++)
    printf("array[%zu] = %8f\n", i, array[i]);
}

void *criar_matriz(size_t num_dims, size_t dims[]) {
  if (num_dims < 2 || num_dims > MAX_DIM)
    return NULL;

  size_t total_pointers = 0, elements = 1;

  size_t mult = 1;
  for (size_t i = 0; i < num_dims - 1; ++i) {
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

  void **current_ptr = layers;
  size_t offset_ptr = dims[0];
  size_t stride = 1;

  for (size_t d = 0; d < num_dims - 1; ++d) {
    stride *= dims[d];
    void **next_ptr = layers + offset_ptr;

    for (size_t i = 0; i < stride; ++i) {
      if (d == num_dims - 2)
        current_ptr[i] = data + i * dims[d + 1];
      else
        current_ptr[i] = next_ptr + i * dims[d + 1];
    }

    current_ptr = next_ptr;
    offset_ptr += stride * dims[d + 1];
  }

  return layers;
}

int main() {
  size_t dims3D[MAX_DIM] = {2, 2, 2};

  Scalar ***matriz3d = criar_matriz(3, dims3D);
  if (!matriz3d) {
    printf("Erro ao alocar matriz3d\n");
    return 1;
  }

  for (size_t i = 0; i < dims3D[0]; ++i)
    for (size_t j = 0; j < dims3D[1]; ++j)
      for (size_t k = 0; k < dims3D[2]; ++k)
        matriz3d[i][j][k] = (Scalar)(100 * i + 10 * j + k);

  printf("matriz3d[1][1][1] = %f\n", matriz3d[1][1][1]);

  print_array(**matriz3d, dims3D[0] * dims3D[1] * dims3D[2]);
  free(matriz3d);

  return 0;
}
