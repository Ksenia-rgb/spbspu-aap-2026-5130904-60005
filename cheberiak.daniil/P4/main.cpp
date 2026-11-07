#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <limits>

namespace cheberiak {
  static int has_rep(const char *const text, const std::size_t length)
  {
    if (length == 0) {
      return 0;
    }
    for (std::size_t i = 0; i < length; ++i) {
      for (std::size_t j = i + 1; j < length; ++j) {
        if (text[i] == text[j]) {
          return 1;
        }
      }
    }
    return 0;
  }

  static int seq_sym(const char *const text, const std::size_t length)
  {
    if (length == 0) {
      return 0;
    }
    for (std::size_t i = 0; i < length - 1; ++i) {
      if (text[i] == text[i + 1]) {
        return 1;
      }
    }
    return 0;
  }

}

int main()
{
  char c = 0;
  constexpr int exit_code_success = 0;
  constexpr int exit_code_memory_error = 1;
  constexpr int exit_code_input_error = 2;
  constexpr std::size_t initial_capacity = 4;
  constexpr std::size_t growth_factor = 2;
  std::size_t capacity = initial_capacity;
  std::size_t length = 0;
  char *text = nullptr;
  text = static_cast< char * >(std::malloc(capacity));
  if (text == nullptr) {
    std::cerr << "Error: memory allocation failed.\n";
    return exit_code_memory_error;
  }
  while (std::cin.get(c) && c != '\n') {
    if (length + 1 >= capacity) {
      if (capacity > std::numeric_limits< std::size_t >::max() / growth_factor) {
        std::cerr << "Error: memory allocation failed.\n";
        std::free(text);
        return exit_code_memory_error;
      }
      const std::size_t new_capacity = capacity * growth_factor;
      char *temp = static_cast< char * >(std::realloc(text, new_capacity));
      if (temp == nullptr) {
        std::cerr << "Error: memory reallocation failed.\n";
        std::free(text);
        return exit_code_memory_error;
      }
      text = temp;
      capacity = new_capacity;
    }
    text[length] = c;
    length++;
  }
  text[length] = '\0';
  if (std::cin.fail()) {
    std::cerr << "Error: failed to read input.\n";
    std::free(text);
    return exit_code_input_error;
  }
  std::cout << cheberiak::seq_sym(text, length) << std::endl << cheberiak::has_rep(text, length) << std::endl;
  std::free(text);
  return exit_code_success;
}
