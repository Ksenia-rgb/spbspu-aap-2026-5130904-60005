#include <iostream>
#include <cstddef>
#include <limits>
#include <new>

namespace kozinskiy {

  const int invalid_argument_ec = 1;
  const int good_ec = 0;
  const std::size_t latinLettersCount = 52;
  const unsigned int lowercaseOffset = 26;
  const std::size_t initialCapacity = 16;

  char *excSnd(char *result, const char *first, const char *second)
  {
    std::size_t j = 0;

    for (std::size_t i = 0; first[i] != '\0'; i++) {
      bool found = false;

      for (std::size_t k = 0; second[k] != '\0'; k++) {
        if (first[i] == second[k]) {
          found = true;
          break;
        }
      }

      if (!found) {
        result[j++] = first[i];
      }
    }

    result[j] = '\0';
    return result;
  }

  std::size_t difLat(const char *str)
  {
    bool letters[latinLettersCount] = {};
    std::size_t count = 0;

    for (std::size_t i = 0; str[i] != '\0'; i++) {
      unsigned int index = 0;

      if (str[i] >= 'A' && str[i] <= 'Z') {
        index = str[i] - 'A';
      } else if (str[i] >= 'a' && str[i] <= 'z') {
        index = str[i] - 'a' + lowercaseOffset;
      } else {
        continue;
      }

      if (!letters[index]) {
        letters[index] = true;
        count++;
      }
    }

    return count;
  }

}

int main()
{
  std::size_t capacity = kozinskiy::initialCapacity;
  std::size_t length = 0;

  char *str = new (std::nothrow) char[capacity];

  if (str == nullptr) {
    std::cerr << "Error: memory allocation";
    return kozinskiy::invalid_argument_ec;
  }

  char ch = '\0';

  while (std::cin.get(ch) && ch != '\n') {
    if (length >= capacity - 1) {
      if (capacity > std::numeric_limits< std::size_t >::max() / 2) {
        delete[] str;
        std::cerr << "Error: memory allocation";
        return kozinskiy::invalid_argument_ec;
      }

      const std::size_t new_capacity = capacity * 2;
      char *const temp = new (std::nothrow) char[new_capacity];

      if (temp == nullptr) {
        delete[] str;
        std::cerr << "Error: memory allocation";
        return kozinskiy::invalid_argument_ec;
      }

      for (std::size_t i = 0; i < length; i++) {
        temp[i] = str[i];
      }

      delete[] str;
      str = temp;
      capacity = new_capacity;
    }

    str[length++] = ch;
  }

  if (std::cin.bad()) {
    delete[] str;
    std::cerr << "Error: input failure";
    return kozinskiy::invalid_argument_ec;
  }

  if (length == 0) {
    delete[] str;
    std::cerr << "Error: empty input";
    return kozinskiy::invalid_argument_ec;
  }

  str[length] = '\0';

  const char second[] = "abc";

  char *const result = new (std::nothrow) char[length + 1];

  if (result == nullptr) {
    delete[] str;
    std::cerr << "Error: memory allocation";
    return kozinskiy::invalid_argument_ec;
  }

  kozinskiy::excSnd(result, str, second);
  std::cout << result << '\n';
  std::cout << kozinskiy::difLat(str) << '\n';

  if (!std::cout) {
    delete[] result;
    delete[] str;
    std::cerr << "Error: output failure";
    return kozinskiy::invalid_argument_ec;
  }

  delete[] result;
  delete[] str;

  return kozinskiy::good_ec;
}
