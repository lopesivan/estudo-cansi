// g++ -std=c++17 -O2 -Wall -Wextra -pedantic t5.cpp -o app
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>

constexpr std::size_t MAX_DIM = 2;

// Alias de ponteiro com deleter 'free'
template <typename T>
using MallocUPtr = std::unique_ptr<T, decltype(&std::free)>;

// Função auxiliar para criar o smart pointer via malloc
template <typename T> MallocUPtr<T> make_malloc_ptr(std::size_t bytes) {
  return {static_cast<T *>(std::malloc(bytes)), &std::free};
}

// Impressão genérica
template <typename Scalar> void print_array(Scalar *array, std::size_t size) {
  for (std::size_t i = 0; i < size; i++)
    std::cout << "array[" << i << "] = " << array[i] << '\n';
}

// Função principal de criação da matriz
template <typename Scalar>
MallocUPtr<void> criar_matriz(std::size_t num_dims, std::size_t dims[]) {
  if (num_dims < 2 || num_dims > MAX_DIM)
    return {nullptr, &std::free};

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

  auto layers = make_malloc_ptr<void>(pointers_size + data_size);
  if (!layers)
    return {nullptr, &std::free};

  auto *data = reinterpret_cast<Scalar *>(
      reinterpret_cast<unsigned char *>(layers.get()) + pointers_size);

  void **current_ptr = static_cast<void **>(layers.get());
  std::size_t offset_ptr = dims[0];
  std::size_t stride = 1;

  for (std::size_t d = 0; d < num_dims - 1; ++d) {
    stride *= dims[d];
    void **next_ptr = static_cast<void **>(layers.get()) + offset_ptr;

    for (std::size_t i = 0; i < stride; ++i) {
      if (d == num_dims - 2)
        current_ptr[i] = static_cast<void *>(data + i * dims[d + 1]);
      else
        current_ptr[i] = static_cast<void *>(next_ptr + i * dims[d + 1]);
    }

    current_ptr = next_ptr;
    offset_ptr += stride * dims[d + 1];
  }

  return layers;
}

int main() {
  constexpr std::size_t dims2D[MAX_DIM] = {3, 3};

  using Tipo = float; // troque aqui: int, float, double, long double etc.

  // RAII: matriz desalocada automaticamente no final
  auto matriz = criar_matriz<Tipo>(2, const_cast<std::size_t *>(dims2D));
  if (!matriz) {
    std::cerr << "Erro ao alocar matriz2d\n";
    return EXIT_FAILURE;
  }

  auto **matriz2d = static_cast<Tipo **>(matriz.get());

  for (std::size_t i = 0; i < dims2D[0]; ++i)
    for (std::size_t j = 0; j < dims2D[1]; ++j)
      matriz2d[i][j] = static_cast<Tipo>(10 * (i + 1) + j);

  std::cout << "matriz2d[1][1] = " << matriz2d[1][1] << '\n';

  print_array(matriz2d[0], dims2D[0] * dims2D[1]);

  // ✅ nada de std::free — liberado automaticamente!
}
