Aqui estão as formas modernas de somar valores de um array em C++:

## **Opção 1: `std::accumulate` (C++98, mas clássico)**

```cpp
#include <numeric>

template <typename T>
T sum_array(T *array, std::size_t size) {
  return std::accumulate(array, array + size, T{0});
}
```

---

## **Opção 2: `std::reduce` (C++17, paralelo)**

```cpp
#include <numeric>
#include <execution>

template <typename T>
T sum_array(T *array, std::size_t size) {
  return std::reduce(std::execution::par, array, array + size, T{0});
}
```

---

## **Opção 3: Range-based com `std::span` (C++20, **recomendado**)**

```cpp
#include <numeric>
#include <span>

template <typename T>
T sum_array(std::span<T> array) {
  return std::accumulate(array.begin(), array.end(), T{0});
}

// Uso:
int arr[] = {1, 2, 3, 4, 5};
auto total = sum_array(std::span{arr});
```

---

## **Opção 4: Ranges (C++20/23, mais expressivo)**

```cpp
#include <ranges>
#include <numeric>

template <typename T>
T sum_array(T *array, std::size_t size) {
  auto view = std::views::counted(array, size);
  return std::accumulate(view.begin(), view.end(), T{0});
}

// Ou com fold (C++23):
template <std::ranges::range R>
auto sum_range(R&& range) {
  return std::ranges::fold_left(range, 0, std::plus{});
}
```

---

## **Comparação prática:**

```cpp
#include <iostream>
#include <numeric>
#include <span>

template <typename T>
void print_array(T *array, std::size_t size) {
  for (std::size_t i = 0; i < size; i++)
    std::cout << "array[" << i << "] = " << array[i] << '\n';
}

// C++20 moderno
template <typename T>
T sum_array(std::span<T> array) {
  return std::accumulate(array.begin(), array.end(), T{0});
}

int main() {
  int nums[] = {10, 20, 30, 40, 50};
  
  print_array(nums, 5);
  std::cout << "\nSoma total: " << sum_array(std::span{nums}) << '\n';
  
  return 0;
}
```

---

## **Minha recomendação:**

Use **`std::span` com `std::accumulate`** (Opção 3):
- ✅ Type-safe (evita passar tamanho errado)
- ✅ C++20, amplamente suportado
- ✅ Zero overhead
- ✅ Interface limpa

```cpp
template <typename T>
T sum_array(std::span<T> array) {
  return std::accumulate(array.begin(), array.end(), T{0});
}
```
