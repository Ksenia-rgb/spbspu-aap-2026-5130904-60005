#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <cctype>
#include <stdexcept>

namespace eliseev {

  const size_t kInitialCapacity = 16;

  size_t length(const char* s)
  {
    size_t n = 0;
    while (s[n] != '\0') {
      ++n;
    }
    return n;
  }

  char* removeDigits(char* dst, const char* src)
  {
    size_t j = 0;
    for (size_t i = 0; src[i] != '\0'; ++i) {
      if (!isdigit(static_cast< unsigned char >(src[i]))) {
        dst[j++] = src[i];
      }
    }
    dst[j] = '\0';
    return dst;
  }

  char* removeLatin(char* dst, const char* src)
  {
    size_t j = 0;
    for (size_t i = 0; src[i] != '\0'; ++i) {
      if (!isalpha(static_cast< unsigned char >(src[i]))) {
        dst[j++] = src[i];
      }
    }
    dst[j] = '\0';
    return dst;
  }

  char* readLine(std::istream& in)
  {
    const size_t max_size = std::numeric_limits< size_t >::max();
    size_t capacity = kInitialCapacity;
    size_t len = 0;

    char* buf = static_cast< char* >(malloc(capacity));
    if (buf == nullptr) {
      throw std::bad_alloc();
    }

    char c = 0;
    while (in.get(c) && c != '\n') {
      if (len + 1 == capacity) {
        if (capacity > max_size / 2) {
          free(buf);
          throw std::bad_alloc();
        }
        const size_t new_capacity = capacity * 2;
        char* const bigger = static_cast< char* >(malloc(new_capacity));
        if (bigger == nullptr) {
          free(buf);
          throw std::bad_alloc();
        }
        for (size_t i = 0; i < len; ++i) {
          bigger[i] = buf[i];
        }
        free(buf);
        buf = bigger;
        capacity = new_capacity;
      }
      buf[len++] = c;
    }
    buf[len] = '\0';
    return buf;
  }
}

int main()
{
  using namespace eliseev;
  try {
    char* const input = readLine(std::cin);
    const size_t len = length(input);

    if (len == 0) {
      std::cerr << "Error: empty input\n";
      free(input);
      return 1;
    }

    char* const result = static_cast< char* >(malloc(len + 1));
    if (result == nullptr) {
      free(input);
      throw std::bad_alloc();
    }

    std::cout << removeDigits(result, input) << "\n";
    std::cout << removeLatin(result, input) << "\n";

    free(result);
    free(input);
  } catch (const std::bad_alloc& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
