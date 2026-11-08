#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <limits>

namespace novikov
{
  static char* expandBuffer(char* old_buffer, std::size_t& capacity)
  {
    const std::size_t growth_factor = 2;
    const std::size_t max_capacity = std::numeric_limits< std::size_t >::max() / growth_factor;
    if (capacity > max_capacity)
    {
      return nullptr;
    }
    const std::size_t new_capacity = capacity * growth_factor;
    char* const new_buffer = static_cast< char* >(std::malloc(new_capacity));
    if (new_buffer == nullptr)
    {
      return nullptr;
    }
    for (std::size_t i = 0; i < capacity; ++i)
    {
      new_buffer[i] = old_buffer[i];
    }
    std::free(old_buffer);
    capacity = new_capacity;
    return new_buffer;
  }

  static char* readString(std::istream& input, std::size_t& length)
  {
    const std::size_t initial_capacity = 16;
    const std::size_t terminator_size = 1;

    length = 0;
    std::size_t capacity = initial_capacity;
    char* buffer = static_cast< char* >(std::malloc(capacity));
    if (buffer == nullptr)
    {
      return nullptr;
    }

    char symbol = '\0';
    while (input.get(symbol) && (symbol != '\n'))
    {
      if (length == (capacity - terminator_size))
      {
        char* const expanded_buffer = expandBuffer(buffer, capacity);
        if (expanded_buffer == nullptr)
        {
          std::free(buffer);
          return nullptr;
        }
        buffer = expanded_buffer;
      }
      buffer[length] = symbol;
      ++length;
    }
    buffer[length] = '\0';
    return buffer;
  }

  static char* removeDigits(const char* src, char* dst) noexcept
  {
    std::size_t length = 0;
    for (std::size_t i = 0; src[i] != '\0'; ++i)
    {
      if (!std::isdigit(static_cast< unsigned char >(src[i])))
      {
        dst[length] = src[i];
        ++length;
      }
    }
    dst[length] = '\0';
    return dst;
  }

  static std::size_t countDifferentLatin(const char* src) noexcept
  {
    const std::size_t latin_letters_count = 26;

    char found[latin_letters_count] = {};
    std::size_t count = 0;
    for (std::size_t i = 0; src[i] != '\0'; ++i)
    {
      const unsigned char symbol = static_cast< unsigned char >(src[i]);
      if (std::isalpha(symbol))
      {
        const char letter = static_cast< char >(std::tolower(symbol));
        std::size_t j = 0;
        while ((j < count) && (found[j] != letter))
        {
          ++j;
        }
        if (j == count)
        {
          found[count] = letter;
          ++count;
        }
      }
    }
    return count;
  }
}

int main()
{
  const std::size_t terminator_size = 1;

  std::size_t length = 0;
  char* const input = novikov::readString(std::cin, length);
  if (input == nullptr)
  {
    std::cerr << "Error: not enough memory\n";
    return 1;
  }
  if (std::cin.fail() && (length == 0))
  {
    std::free(input);
    std::cerr << "Error: input is empty\n";
    return 1;
  }

  std::cout << novikov::countDifferentLatin(input) << '\n';

  int code = 0;

  char* const digitless = static_cast< char* >(std::malloc(length + terminator_size));
  if (digitless == nullptr)
  {
    std::cerr << "Error: not enough memory\n";
    code = 1;
  }
  else
  {
    novikov::removeDigits(input, digitless);
    std::cout << digitless << '\n';
    std::free(digitless);
  }

  std::free(input);
  return code;
}
