#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace uzun {

  const int alloc_error_code = 1;

  char * readLine(std::istream & in)
  {
    const std::size_t initial_capacity = 16;
    std::size_t capacity = initial_capacity;
    std::size_t length = 0;
    char * buffer = static_cast< char * >(std::malloc(capacity));
    if (buffer == nullptr) {
      return nullptr;
    }
    char symbol = 0;
    while (in.get(symbol) && symbol != '\n') {
      if (length + 1 >= capacity) {
        const std::size_t new_capacity = capacity * 2;
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

  char * uniteTwo(const char * first, const char * second, char * out) noexcept
  {
    std::size_t i = 0;
    std::size_t j = 0;
    std::size_t k = 0;
    while (first[i] != '\0' && second[j] != '\0') {
      out[k++] = first[i++];
      out[k++] = second[j++];
    }
    while (first[i] != '\0') {
      out[k++] = first[i++];
    }
    while (second[j] != '\0') {
      out[k++] = second[j++];
    }
    out[k] = '\0';
    return out;
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

}

int main()
{
  const char second[] = "def ";

  char * const input = uzun::readLine(std::cin);
  if (input == nullptr) {
    std::cerr << "memory allocation error\n";
    return uzun::alloc_error_code;
  }

  const std::size_t united_size = std::strlen(input) + std::strlen(second) + 1;
  char * const united = static_cast< char * >(std::malloc(united_size));
  char * const no_digits = static_cast< char * >(std::malloc(std::strlen(input) + 1));
  if (united == nullptr || no_digits == nullptr) {
    std::cerr << "memory allocation error\n";
    std::free(input);
    std::free(united);
    std::free(no_digits);
    return uzun::alloc_error_code;
  }

  std::cout << uzun::uniteTwo(input, second, united) << '\n';
  std::cout << uzun::removeDigits(input, no_digits) << '\n';

  std::free(input);
  std::free(united);
  std::free(no_digits);
  return 0;
}
