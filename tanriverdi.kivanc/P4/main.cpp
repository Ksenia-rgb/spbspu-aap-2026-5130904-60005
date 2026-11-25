#include <cctype>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>
#include <new>
#include <utility>

namespace tanriverdi
{
  char *removeExtraSpaces(const char *src, char *dst) noexcept
  {
    char *out = dst;
    bool pending_space = false;
    for (; *src != '\0'; ++src)
    {
      if (*src == ' ')
      {
        pending_space = (out != dst);
      }
      else
      {
        if (pending_space)
        {
          *out++ = ' ';
          pending_space = false;
        }
        *out++ = *src;
      }
    }
    *out = '\0';
    return dst;
  }

  char *appendDigits(const char *first, const char *second, char *dst) noexcept
  {
    char *out = dst;
    for (; *first != '\0'; ++first)
    {
      *out++ = *first;
    }
    for (; *second != '\0'; ++second)
    {
      if (std::isdigit(static_cast< unsigned char >(*second)))
      {
        *out++ = *second;
      }
    }
    *out = '\0';
    return dst;
  }

  std::unique_ptr< char[] > readLine(std::istream &in)
  {
    const std::size_t initial_capacity = 16;
    const std::size_t growth_factor = 2;
    std::size_t capacity = initial_capacity;
    std::size_t size = 0;
    std::unique_ptr< char[] > buf(new char[capacity]);
    char c = 0;
    while (in.get(c) && c != '\n')
    {
      if (size + 1 >= capacity)
      {
        const std::size_t new_capacity = capacity * growth_factor;
        std::unique_ptr< char[] > bigger(new char[new_capacity]);
        std::memcpy(bigger.get(), buf.get(), size);
        buf = std::move(bigger);
        capacity = new_capacity;
      }
      buf[size++] = c;
    }
    buf[size] = '\0';
    return buf;
  }
}

int main()
{
  try
  {
    const std::unique_ptr< char[] > line = tanriverdi::readLine(std::cin);

    const std::unique_ptr< char[] > res1(new char[std::strlen(line.get()) + 1]);
    tanriverdi::removeExtraSpaces(line.get(), res1.get());
    std::cout << res1.get() << '\n';

    const char *const second = "g1h2k";
    const std::size_t len2 = std::strlen(line.get()) + std::strlen(second) + 1;
    const std::unique_ptr< char[] > res2(new char[len2]);
    tanriverdi::appendDigits(line.get(), second, res2.get());
    std::cout << res2.get() << '\n';
  }
  catch (const std::bad_alloc &)
  {
    std::cerr << "error: not enough memory\n";
    return 1;
  }
  return 0;
}
