#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace abdullaeva {

  const int alloc_error_code = 1;

  char * readLine(std::istream & in)
  {
    const std::size_t initial_capacity = 16;
    const std::size_t growth_factor = 2;
    std::size_t capacity = initial_capacity;
    std::size_t length = 0;
    char * buffer = static_cast< char * >(std::malloc(capacity));
    if (buffer == nullptr) {
      return nullptr;
    }
    char symbol = 0;
    while (in.get(symbol) && symbol != '\n') {
      if (length + 1 >= capacity) {
        const std::size_t new_capacity = capacity * growth_factor;
        char * const tmp = static_cast< char * >(std::realloc(buffer, new_capacity));
        if (tmp == nullptr) {
          std::free(buffer);
          return nullptr;
        }
        buffer = tmp;
        capacity = new_capacity;
      }
      buffer[length++] = symbol;
    }
    if (length == 0 && !in) {
      std::free(buffer);
      return nullptr;
    }
    buffer[length] = '\0';
    return buffer;
  }

  char * removeDigits(const char * src, char * out) noexcept
  {
    std::size_t k = 0;
    for (std::size_t i = 0; src[i] != '\0'; ++i) {
      if (!std::isdigit(static_cast< unsigned char >(src[i]))) {
        out[k++] = src[i];
      }
    }
    out[k] = '\0';
    return out;
  }

  char * uniqueSymbols(const char * first, const char * second, char * out) noexcept
  {
    const std::size_t alphabet_size = 256;
    int in_first[alphabet_size] = {};
    int in_second[alphabet_size] = {};
    for (std::size_t i = 0; first[i] != '\0'; ++i) {
      in_first[static_cast< unsigned char >(first[i])] = 1;
    }
    for (std::size_t i = 0; second[i] != '\0'; ++i) {
      in_second[static_cast< unsigned char >(second[i])] = 1;
    }
    std::size_t k = 0;
    for (std::size_t code = 1; code < alphabet_size; ++code) {
      if (in_first[code] != in_second[code]) {
        out[k++] = static_cast< char >(code);
      }
    }
    out[k] = '\0';
    return out;
  }

}

int main()
{
  const char second[] = "abc ef";

  char * const input = abdullaeva::readLine(std::cin);
  if (input == nullptr) {
    std::cerr << "memory allocation error\n";
    return abdullaeva::alloc_error_code;
  }

  char * const no_digits = static_cast< char * >(std::malloc(std::strlen(input) + 1));
  const std::size_t unique_size = std::strlen(input) + std::strlen(second) + 1;
  char * const unique = static_cast< char * >(std::malloc(unique_size));
  if (no_digits == nullptr || unique == nullptr) {
    std::cerr << "memory allocation error\n";
    std::free(input);
    std::free(no_digits);
    std::free(unique);
    return abdullaeva::alloc_error_code;
  }

  std::cout << abdullaeva::removeDigits(input, no_digits) << '\n';
  std::cout << abdullaeva::uniqueSymbols(input, second, unique) << '\n';

  std::free(input);
  std::free(no_digits);
  std::free(unique);
  return 0;
}
