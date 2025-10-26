#include <stdio.h>
#include <stdlib.h>

typedef double Scalar;
typedef Scalar *Array1d;

static void print_array(double *array, size_t size) {

  unsigned int i;

  for (i = 0; i < size; i++) {

    printf("array[%u] = %8f \n", i, array[i]);
  }

  return;
}

void *new_nd(size_t num_dims, size_t dims[]) {
  if (num_dims < 2)
    return NULL;

  size_t pointer_size = sizeof(void *);
  size_t total_pointers = 0;
  size_t data_elements = 1;

  size_t mult = 1;
  for (size_t i = 0; i < num_dims - 1; i++) {
    mult *= dims[i];
    total_pointers += mult;
  }

  for (size_t i = 0; i < num_dims; i++)
    data_elements *= dims[i];

  size_t pointers_bytes = total_pointers * pointer_size;
  size_t data_bytes = data_elements * sizeof(Scalar);

  unsigned char *raw_bytes = malloc(pointers_bytes + data_bytes);

  if (!raw_bytes)
    return NULL;

  void **layer_ptr = (void **)raw_bytes;
  Scalar *data_ptr = (Scalar *)(raw_bytes + pointers_bytes);

  size_t pointer_index = 0;
  size_t offset = 0;

  for (size_t dim = 0; dim < num_dims - 1; dim++) {
    size_t layer_count = 1;
    for (size_t i = 0; i <= dim; i++)
      layer_count *= dims[i];

    size_t next_layer_size = dims[dim + 1];

    for (size_t j = 0; j < layer_count; j++) {
      if (dim == num_dims - 2)
        layer_ptr[pointer_index++] =
            (void *)(data_ptr + offset + j * next_layer_size);
      else
        layer_ptr[pointer_index++] =
            (void *)(layer_ptr + pointer_index + layer_count - j +
                     j * next_layer_size);
    }
    offset += layer_count * next_layer_size;
  }

  return (void *)layer_ptr;
}

#define INDEX3D(d2, d3, i, j, k) ((i) * (d2) * (d3) + (j) * (d3) + (k))

int main() {
  size_t dims3D[] = {3, 3, 3};

  Scalar ***matriz3d = (Scalar ***)new_nd(3, dims3D);

  if (!matriz3d)
    return 1;

  for (size_t i = 0; i < dims3D[0]; i++)
    for (size_t j = 0; j < dims3D[1]; j++)
      for (size_t k = 0; k < dims3D[2]; k++)
        matriz3d[i][j][k] = (Scalar)INDEX3D(dims3D[1], dims3D[2], i, j, k);

  printf("matriz3d[1][1][1] = %f\n", matriz3d[1][1][1]);
  printf("matriz3d[1][1][1] = %f\n", matriz3d[1][1][1]);
  printf("matriz3d[1][1][1] = %f\n", matriz3d[1][1][1]);

  /* print_array(matriz3d, 3 * 3 * 3); */

  free(matriz3d);

  return 0;
}
