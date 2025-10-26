// g++ -std=c++17 -O2 -Wall -Wextra -pedantic t4.cpp -o app
#include <cstddef>
#include <cstdio>
#include <cstdlib>

constexpr std::size_t MAX_DIM = 2;

template <typename Scalar>
static void print_array(Scalar *array, std::size_t size) {
  for (std::size_t i = 0; i < size; i++)
    std::printf("array[%zu] = %8.4f\n", i, static_cast<double>(array[i]));
}

template <typename Scalar>
void *criar_matriz(std::size_t num_dims, std::size_t dims[]) {
  if (num_dims < 2 || num_dims > MAX_DIM)
    return nullptr;

  std::size_t total_pointers = 0, elements = 1;

  std::size_t mult = 1;
  for (std::size_t i = 0; i < num_dims - 1; ++i) {
    mult *= dims[i];
    total_pointers += mult;
  }

  for (std::size_t i = 0; i < num_dims; ++i)
    elements *= dims[i];

  const std::size_t pointers_size = total_pointers * sizeof(void *);
  const std::size_t data_size = elements * sizeof(Scalar);

  void **layers = static_cast<void **>(std::malloc(pointers_size + data_size));
  if (!layers)
    return nullptr;

  auto *data = reinterpret_cast<Scalar *>(
      reinterpret_cast<unsigned char *>(layers) + pointers_size);

  void **current_ptr = layers;
  std::size_t offset_ptr = dims[0];
  std::size_t stride = 1;

  for (std::size_t d = 0; d < num_dims - 1; ++d) {
    stride *= dims[d];
    void **next_ptr = layers + offset_ptr;

    for (std::size_t i = 0; i < stride; ++i) {
      if (d == num_dims - 2)
        current_ptr[i] = static_cast<void *>(data + i * dims[d + 1]);
      else
        current_ptr[i] = static_cast<void *>(next_ptr + i * dims[d + 1]);
    }

    current_ptr = next_ptr;
    offset_ptr += stride * dims[d + 1];
  }

  return static_cast<void *>(layers);
}

int main() {
  constexpr std::size_t dims2D[MAX_DIM] = {2, 3};

  // 🔹 troque o tipo aqui: int, float, double, long double, etc.
  using Tipo = float;

  auto *matriz2d = static_cast<Tipo **>(
      criar_matriz<Tipo>(2, const_cast<std::size_t *>(dims2D)));
  if (!matriz2d) {
    std::printf("Erro ao alocar matriz2d\n");
    return 1;
  }

  for (std::size_t i = 0; i < dims2D[0]; ++i)
    for (std::size_t j = 0; j < dims2D[1]; ++j)
      matriz2d[i][j] = static_cast<Tipo>(10 * (i + 1) + j);

  std::printf("matriz2d[1][1] = %f\n", static_cast<double>(matriz2d[1][1]));

  print_array(matriz2d[0], dims2D[0] * dims2D[1]);

  std::free(matriz2d);
  return 0;
}
