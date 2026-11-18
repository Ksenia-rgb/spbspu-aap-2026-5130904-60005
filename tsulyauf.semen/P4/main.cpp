#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <istream>
#include <limits>

namespace tsulyauf {
  const std::size_t initial_capacity = 16;
  const std::size_t growth_factor = 2;

  const int memory_error_code = 1;
  const char* const memory_error_message = "Memory allocation error!\n";
  const int input_error_code = 1;
  const char* const input_error_message = "Input error!\n";

  char* expandBuffer(char* old_buffer, std::size_t& capacity);
  char* readString(std::istream& input, std::size_t& length);
  int hasRepeatedSymbols(const char* data) noexcept;
  char* removeDigits(const char* data, char* result) noexcept;
}

char* tsulyauf::expandBuffer(char* old_buffer, std::size_t& capacity)
{
  if (capacity > std::numeric_limits< std::size_t >::max() / growth_factor) {
    return nullptr;
  }

  char* const new_buffer = static_cast< char* >(std::malloc(capacity * growth_factor));
  if (new_buffer == nullptr) {
    return nullptr;
  }

  for (std::size_t i = 0; i < capacity; ++i) {
    new_buffer[i] = old_buffer[i];
  }

  capacity *= growth_factor;
  std::free(old_buffer);
  return new_buffer;
}

char* tsulyauf::readString(std::istream& input, std::size_t& length)
{
  length = 0;
  std::size_t capacity = initial_capacity;
  char symbol = '\0';

  char* buffer = static_cast< char* >(std::malloc(initial_capacity));
  if (buffer == nullptr) {
    return nullptr;
  }

  while (input.get(symbol) && symbol != '\n') {
    if (length == capacity - 1) {
      char* const expanded_buffer = expandBuffer(buffer, capacity);
      if (expanded_buffer == nullptr) {
        std::free(buffer);
        return nullptr;
      }
      buffer = expanded_buffer;
    }
    buffer[length++] = symbol;
  }

  buffer[length] = '\0';

  return buffer;
}

int tsulyauf::hasRepeatedSymbols(const char* data) noexcept
{
  if (data[0] == '\0') {
    return 0;
  }
  for (std::size_t i = 0; data[i + 1] != '\0'; ++i) {
    if (data[i] == data[i + 1]) {
      return 1;
    }
  }

  return 0;
}

char* tsulyauf::removeDigits(const char* data, char* result) noexcept
{
  std::size_t j = 0;
  for (std::size_t i = 0; data[i] != '\0'; ++i) {
    if (!std::isdigit(static_cast< unsigned char >(data[i]))) {
      result[j++] = data[i];
    }
  }
  result[j] = '\0';

  return result;
}

int main()
{
  std::size_t length = 0;
  char* const buffer = tsulyauf::readString(std::cin, length);
  if (buffer == nullptr) {
    std::cerr << tsulyauf::memory_error_message;
    return tsulyauf::memory_error_code;
  }

  if (length == 0 && !std::cin) {
    std::cerr << tsulyauf::input_error_message;
    std::free(buffer);
    return tsulyauf::input_error_code;
  }

  std::cout << tsulyauf::hasRepeatedSymbols(buffer) << "\n";

  char* const new_buffer = static_cast< char* >(std::malloc(length + 1));
  if (new_buffer == nullptr) {
    std::cerr << tsulyauf::memory_error_message;
    std::free(buffer);
    return tsulyauf::memory_error_code;
  }

  tsulyauf::removeDigits(buffer, new_buffer);
  std::cout << new_buffer << "\n";

  std::free(buffer);
  std::free(new_buffer);
}
