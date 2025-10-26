#include <stdio.h>
#include <stdlib.h>

#define MAX_DIM 8

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
  size_t dims8D[MAX_DIM] = {2, 2, 2, 2, 2, 2, 2, 2};
  size_t total_elements = 256; // 2^8

  Scalar ********matriz8d = criar_matriz(8, dims8D);
  if (!matriz8d) {
    printf("Erro ao alocar matriz8d\n");
    return 1;
  }

  for (size_t i = 0; i < dims8D[0]; ++i)
    for (size_t j = 0; j < dims8D[1]; ++j)
      for (size_t k = 0; k < dims8D[2]; ++k)
        for (size_t l = 0; l < dims8D[3]; ++l)
          for (size_t m = 0; m < dims8D[4]; ++m)
            for (size_t n = 0; n < dims8D[5]; ++n)
              for (size_t o = 0; o < dims8D[6]; ++o)
                for (size_t p = 0; p < dims8D[7]; ++p)
                  matriz8d[i][j][k][l][m][n][o][p] =
                      (Scalar)(i + j + k + l + m + n + o + p);

  printf("matriz8d[1][1][1][1][1][1][1][1] = %f\n",
         matriz8d[1][1][1][1][1][1][1][1]);

  print_array(*******matriz8d, total_elements);
  free(matriz8d);

  return 0;
}
