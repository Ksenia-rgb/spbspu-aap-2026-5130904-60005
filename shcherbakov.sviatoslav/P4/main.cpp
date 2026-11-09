#include <cctype>
#include <cstddef>
#include <iostream>
#include <limits>
#include <new>

namespace shcherbakov
{
  const int error_failure = 1;

  char * convertToLower(const char * src, char * const dst) noexcept
  {
    char * current = dst;
    while (*src != '\0')
    {
      if (std::isupper(*src))
      {
        *current = std::tolower(*src);
      }
      else
      {
        *current = *src;
      }
      ++src;
      ++current;
    }
    *current = '\0';
    return dst;
  }

  char * removeLatinLetters(const char * src, char * const dst) noexcept
  {
    char * current = dst;
    while (*src != '\0')
    {
      if (!std::isalpha(*src))
      {
        *current = *src;
        ++current;
      }
      ++src;
    }
    *current = '\0';
    return dst;
  }

  char * readLine(std::istream & in, std::size_t * const out_length)
  {
    const std::size_t initial_capacity = 32;
    const std::size_t growth_factor = 2;
    const std::size_t max_capacity = std::numeric_limits< std::size_t >::max() / growth_factor;

    std::size_t capacity = initial_capacity;
    std::size_t size = 0;
    char * buffer = new (std::nothrow) char[capacity];
    if (buffer == nullptr)
    {
      return nullptr;
    }

    char ch = '\0';
    while (in.get(ch) && ch != '\n')
    {
      if (size + 1 >= capacity)
      {
        if (capacity > max_capacity)
        {
          delete[] buffer;
          return nullptr;
        }
        const std::size_t new_capacity = capacity * growth_factor;
        char * const new_buffer = new (std::nothrow) char[new_capacity];
        if (new_buffer == nullptr)
        {
          delete[] buffer;
          return nullptr;
        }
        for (std::size_t i = 0; i < size; ++i)
        {
          new_buffer[i] = buffer[i];
        }
        delete[] buffer;
        buffer = new_buffer;
        capacity = new_capacity;
      }
      buffer[size] = ch;
      ++size;
    }

    if (size == 0 && !in)
    {
      delete[] buffer;
      return nullptr;
    }

    buffer[size] = '\0';
    if (out_length != nullptr)
    {
      *out_length = size;
    }
    return buffer;
  }
}

int main()
{
  std::size_t length = 0;
  const char * const input_line = shcherbakov::readLine(std::cin, &length);
  if (input_line == nullptr)
  {
    std::cerr << "Cannot read line or memory error\n";
    return shcherbakov::error_failure;
  }

  char * const first_result = new (std::nothrow) char[length + 1];
  if (first_result == nullptr)
  {
    delete[] input_line;
    std::cerr << "Memory allocation error\n";
    return shcherbakov::error_failure;
  }

  char * const second_result = new (std::nothrow) char[length + 1];
  if (second_result == nullptr)
  {
    delete[] first_result;
    delete[] input_line;
    std::cerr << "Memory allocation error\n";
    return shcherbakov::error_failure;
  }

  shcherbakov::convertToLower(input_line, first_result);
  shcherbakov::removeLatinLetters(input_line, second_result);

  std::cout << first_result << "\n";
  std::cout << second_result << "\n";

  delete[] second_result;
  delete[] first_result;
  delete[] input_line;
  return 0;
}
