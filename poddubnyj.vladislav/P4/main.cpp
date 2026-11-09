#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>

namespace poddubnyj
{
constexpr int ok = 0;
constexpr int fail = 1;
constexpr std::size_t initialCapacity = 16;

int read(std::istream &in, char *&source, std::size_t &length) noexcept
{
  std::size_t capacity = initialCapacity;
  source = static_cast<char *>(std::malloc(capacity));
  if (source == nullptr)
  {
    return fail;
  }
  length = 0;
  char symbol = 0;
  while (in.get(symbol) && symbol != '\n')
  {
    if (length + 1 >= capacity)
    {
      if (capacity > std::numeric_limits<std::size_t>::max() / 2)
      {
        std::free(source);
        return fail;
      }
      capacity *= 2;
      char *const grown = static_cast<char *>(std::realloc(source, capacity));
      if (grown == nullptr)
      {
        std::free(source);
        return fail;
      }
      source = grown;
    }
    source[length++] = symbol;
  }
  if (in.bad())
  {
    std::free(source);
    return fail;
  }
  source[length] = '\0';
  return ok;
}

bool removeLatin(const char *source, char *dest, std::size_t destSize,
                 std::size_t &resultLength) noexcept
{
  std::size_t outIndex = 0;
  for (std::size_t index = 0; source[index] != '\0'; ++index)
  {
    if (!std::isalpha(static_cast<unsigned char>(source[index])))
    {
      if ((outIndex + 1) >= destSize)
      {
        return false;
      }
      dest[outIndex++] = source[index];
    }
  }
  if (outIndex >= destSize)
  {
    return false;
  }
  dest[outIndex] = '\0';
  resultLength = outIndex;
  return true;
}

bool mix(const char *first, const char *second, char *dest,
         std::size_t destSize, std::size_t &resultLength) noexcept
{
  const std::size_t firstLength = std::strlen(first);
  const std::size_t secondLength = std::strlen(second);
  const std::size_t maxLength = (firstLength > secondLength) ? firstLength : secondLength;
  std::size_t outIndex = 0;
  for (std::size_t index = 0; index < maxLength; ++index)
  {
    if (index < firstLength)
    {
      if ((outIndex + 1) >= destSize)
      {
        return false;
      }
      dest[outIndex++] = first[index];
    }
    if (index < secondLength)
    {
      if ((outIndex + 1) >= destSize)
      {
        return false;
      }
      dest[outIndex++] = second[index];
    }
  }
  if (outIndex >= destSize)
  {
    return false;
  }
  dest[outIndex] = '\0';
  resultLength = outIndex;
  return true;
}

int run()
{
  char *source = nullptr;
  std::size_t length = 0;
  if (read(std::cin, source, length) != ok)
  {
    std::cerr << "input or memory error\n";
    return fail;
  }

  const char second[] = "def_";
  const std::size_t mixedSize = length + sizeof(second);
  char *const mixed = (mixedSize < length) ? nullptr
                                           : static_cast<char *>(std::malloc(mixedSize));
  std::size_t mixedLength = 0;
  std::size_t removedLength = 0;
  if (mixed == nullptr
      || !mix(source, second, mixed, mixedSize, mixedLength)
      || !removeLatin(source, source, length + 1, removedLength))
  {
    std::free(mixed);
    std::free(source);
    std::cerr << "internal error\n";
    return fail;
  }

  std::cout.write(source, static_cast<std::streamsize>(removedLength));
  std::cout << '\n';
  std::cout.write(mixed, static_cast<std::streamsize>(mixedLength));
  std::cout << '\n';

  std::free(mixed);
  std::free(source);
  return ok;
}
}

int main()
{
  return poddubnyj::run();
}
