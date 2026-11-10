#include <iostream>
#include <cctype>
#include <cstddef>

namespace usmanov {

char* uppLow(char* dest, const char* src) noexcept
{
  std::size_t i = 0;
  while (src[i] != '\0') {
    dest[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(src[i])));
    i++;
  }
  dest[i] = '\0';
  return dest;
}

int repDgt(const char* src) noexcept
{
  int counts[10] = {0};
  std::size_t i = 0;
  while (src[i] != '\0') {
    if (std::isdigit(static_cast<unsigned char>(src[i])) != 0) {
      const int digit = src[i] - '0';
      counts[digit]++;
      if (counts[digit] > 1) {
        return 1;
      }
    }
    i++;
  }
  return 0;
}

} // namespace usmanov

int main()
{
  std::size_t capacity = 16;
  std::size_t length = 0;
  char* buffer = new char[capacity];

  char c = 0;
  while (std::cin.get(c) && c != '\n') {
    if (length + 1 >= capacity) {
      const std::size_t new_capacity = capacity * 2;
      char* new_buffer = new char[new_capacity];
      for (std::size_t i = 0; i < length; i++) {
        new_buffer[i] = buffer[i];
      }
      delete[] buffer;
      buffer = new_buffer;
      capacity = new_capacity;
    }
    buffer[length] = c;
    length++;
  }
  buffer[length] = '\0';

  char* dest_str = new char[length + 1];

  usmanov::uppLow(dest_str, buffer);
  std::cout << dest_str << "\n";

  const int has_rep_digits = usmanov::repDgt(buffer);
  std::cout << has_rep_digits << "\n";

  delete[] dest_str;
  delete[] buffer;

  return 0;
}
