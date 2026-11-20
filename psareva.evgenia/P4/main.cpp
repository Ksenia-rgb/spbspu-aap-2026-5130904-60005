#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <limits>

namespace psareva {
  const int success_code = 0;
  const int error_alloc = 1;
  const char* const error_alloc_message = "Memory allocation error\n";

  char* removeSpaces(char* res, const char* data) noexcept;
  char* removeLatin(char* res, const char* data) noexcept;
  char* expandBuffer(char* buffer, std::size_t& capacity);
  char* readString(std::istream& in, std::size_t& len);
}

char* psareva::expandBuffer(char* buffer, std::size_t& capacity)
{
  const std::size_t growth_factor = 2;

  if (capacity > (std::numeric_limits< std::size_t >::max() / growth_factor)) {
    return nullptr;
  }

  const std::size_t new_capacity = capacity * growth_factor;
  char* new_buffer = static_cast< char* >(std::malloc(new_capacity));
  if (new_buffer == nullptr) {
    return nullptr;
  }

  for (std::size_t i = 0; i < capacity; ++i) {
    new_buffer[i] = buffer[i];
  }

  capacity = new_capacity;
  std::free(buffer);
  return new_buffer;
}

char* psareva::removeSpaces(char* res, const char* data) noexcept
{
  char* out = res;
  int has_written = 0;
  int pending_space = 0;

  while (*data != '\0') {
    if (std::isspace(static_cast< unsigned char >(*data))) {
      if (has_written) {
        pending_space = 1;
      }
    } else {
      if (pending_space) {
        *out = ' ';
        ++out;
        pending_space = 0;
      }
      *out = *data;
      ++out;
      has_written = 1;
    }
    ++data;
  }

  *out = '\0';
  return res;
}

char* psareva::removeLatin(char* res, const char* data) noexcept
{
  char* out = res;

  while (*data != '\0') {
    if (!std::isalpha(static_cast< unsigned char >(*data))) {
      *out = *data;
      ++out;
    }
    ++data;
  }

  *out = '\0';
  return res;
}

char* psareva::readString(std::istream& in, std::size_t& len)
{
  const std::size_t initial_capacity = 16;

  std::size_t capacity = initial_capacity;
  char* str = static_cast< char* >(std::malloc(capacity));
  if (str == nullptr) {
    return nullptr;
  }

  std::size_t size = 0;
  char c = '\0';

  while (in.get(c) && (c != '\n')) {
    if ((size + 1) >= capacity) {
      char* new_str = expandBuffer(str, capacity);
      if (new_str == nullptr) {
        std::free(str);
        return nullptr;
      }
      str = new_str;
    }
    str[size] = c;
    ++size;
  }

  str[size] = '\0';
  len = size;
  return str;
}

int main()
{
  std::size_t len = 0;
  char* data = psareva::readString(std::cin, len);
  if (data == nullptr) {
    std::cerr << psareva::error_alloc_message;
    return psareva::error_alloc;
  }

  char* res = static_cast< char* >(std::malloc(len + 1));
  if (res == nullptr) {
    std::cerr << psareva::error_alloc_message;
    std::free(data);
    return psareva::error_alloc;
  }

  std::cout << psareva::removeSpaces(res, data) << "\n";
  std::cout << psareva::removeLatin(res, data) << "\n";

  std::free(res);
  std::free(data);

  return psareva::success_code;
}

