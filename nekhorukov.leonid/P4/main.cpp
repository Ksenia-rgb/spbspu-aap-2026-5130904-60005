#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <cctype>

namespace nekhorukov
{
  enum ErrorCode
  {
    kSuccess = 0,
    kMemoryError = 1
  };

  const std::size_t kInitialCapacity = 64;
  const std::size_t kGrowthFactor = 2;
  const std::size_t kAlphabetSize = 256;
  const char kSecondString[] = "abc ef";

  char * readLine(std::istream & in, std::size_t * length)
  {
    std::size_t capacity = kInitialCapacity;
    char * buffer = static_cast< char * >(std::malloc(capacity));
    if (buffer == nullptr)
    {
      return nullptr;
    }
    std::size_t size = 0;
    char symbol = 0;
    while (in.get(symbol) && symbol != '\n')
    {
      if (size + 1 >= capacity)
      {
        if (capacity > std::numeric_limits< std::size_t >::max() / kGrowthFactor)
        {
          std::free(buffer);
          return nullptr;
        }
        const std::size_t new_capacity = capacity * kGrowthFactor;
        char * new_buffer = static_cast< char * >(std::malloc(new_capacity));
        if (new_buffer == nullptr)
        {
          std::free(buffer);
          return nullptr;
        }
        for (std::size_t i = 0; i < size; ++i)
        {
          new_buffer[i] = buffer[i];
        }
        std::free(buffer);
        buffer = new_buffer;
        capacity = new_capacity;
      }
      buffer[size] = symbol;
      ++size;
    }
    buffer[size] = '\0';
    *length = size;
    return buffer;
  }

  char * collectUncommon(char * dest, const char * first, const char * second) noexcept
  {
    bool in_first[kAlphabetSize] = {};
    bool in_second[kAlphabetSize] = {};
    for (std::size_t i = 0; first[i] != '\0'; ++i)
    {
      in_first[static_cast< unsigned char >(first[i])] = true;
    }
    for (std::size_t i = 0; second[i] != '\0'; ++i)
    {
      in_second[static_cast< unsigned char >(second[i])] = true;
    }
    std::size_t written = 0;
    for (std::size_t code = 0; code < kAlphabetSize; ++code)
    {
      if (in_first[code] != in_second[code])
      {
        dest[written] = static_cast< char >(code);
        ++written;
      }
    }
    dest[written] = '\0';
    return dest;
  }

  char * removeDigits(char * dest, const char * src) noexcept
  {
    std::size_t written = 0;
    for (std::size_t i = 0; src[i] != '\0'; ++i)
    {
      if (std::isdigit(static_cast< unsigned char >(src[i])) == 0)
      {
        dest[written] = src[i];
        ++written;
      }
    }
    dest[written] = '\0';
    return dest;
  }

  int process(std::istream & in, std::ostream & out, std::ostream & err)
  {
    std::size_t length = 0;
    char * input = readLine(in, &length);
    if (input == nullptr)
    {
      err << "memory allocation error\n";
      return kMemoryError;
    }
    char * uncommon = static_cast< char * >(std::malloc(kAlphabetSize + 1));
    char * without_digits = static_cast< char * >(std::malloc(length + 1));
    if (uncommon == nullptr || without_digits == nullptr)
    {
      err << "memory allocation error\n";
      std::free(without_digits);
      std::free(uncommon);
      std::free(input);
      return kMemoryError;
    }
    collectUncommon(uncommon, input, kSecondString);
    removeDigits(without_digits, input);
    out << uncommon << '\n';
    out << without_digits << '\n';
    std::free(without_digits);
    std::free(uncommon);
    std::free(input);
    return kSuccess;
  }
}

int main()
{
  return nekhorukov::process(std::cin, std::cout, std::cerr);
}
