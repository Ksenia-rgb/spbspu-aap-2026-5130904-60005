#include <iostream>
#include <cstddef>
#include <limits>
#include <stdexcept>

namespace isaev
{
  constexpr std::size_t initialCapacity = 16;
  constexpr std::size_t growthFactor = 2;
  constexpr std::size_t terminatorSize = 1;
  constexpr int errorCode = 1;

  char* readLine(std::istream& in);
  std::size_t addSizes(std::size_t lhs, std::size_t rhs);

  const char* findSymbol(const char* str, char symbol) noexcept;
  std::size_t getLength(const char* str) noexcept;
  std::size_t appendMissingSymbols(char* result, std::size_t length, const char* source, const char* other) noexcept;

  char* excludeSecond(char* result, const char* first, const char* second) noexcept;
  char* getUncommonSymbols(char* result, const char* first, const char* second) noexcept;
}

char* isaev::readLine(std::istream& in)
{
  const int endOfFile = std::istream::traits_type::eof();
  std::size_t capacity = isaev::initialCapacity;
  std::size_t length = 0;
  char* buffer = new char[capacity]();
  try
  {
    int next = in.get();
    if (next == endOfFile)
    {
      throw std::runtime_error("Input is empty");
    }
    while ((next != endOfFile) && (next != '\n'))
    {
      const char symbol = static_cast< char >(next);
      if (symbol == '\0')
      {
        throw std::invalid_argument("Input contains a null character");
      }
      buffer[length] = symbol;
      ++length;
      if (length == capacity)
      {
        if (capacity > (std::numeric_limits< std::size_t >::max() / isaev::growthFactor))
        {
          throw std::overflow_error("Input line is too long");
        }
        const std::size_t newCapacity = capacity * isaev::growthFactor;
        char* const bigger = new char[newCapacity]();
        for (std::size_t i = 0; i < length; ++i)
        {
          bigger[i] = buffer[i];
        }
        delete[] buffer;
        buffer = bigger;
        capacity = newCapacity;
      }
      next = in.get();
    }
    if (in.bad())
    {
      throw std::runtime_error("Input error");
    }
  }
  catch (...)
  {
    delete[] buffer;
    throw;
  }
  return buffer;
}

std::size_t isaev::addSizes(std::size_t lhs, std::size_t rhs)
{
  if (lhs > (std::numeric_limits< std::size_t >::max() - rhs))
  {
    throw std::overflow_error("Size overflow");
  }
  return lhs + rhs;
}

const char* isaev::findSymbol(const char* str, char symbol) noexcept
{
  while (*str != '\0')
  {
    if (*str == symbol)
    {
      return str;
    }
    ++str;
  }
  return nullptr;
}

std::size_t isaev::getLength(const char* str) noexcept
{
  std::size_t length = 0;
  while (str[length] != '\0')
  {
    ++length;
  }
  return length;
}

std::size_t isaev::appendMissingSymbols(
    char* result, std::size_t length, const char* source, const char* other) noexcept
{
  for (std::size_t i = 0; source[i] != '\0'; ++i)
  {
    if (isaev::findSymbol(other, source[i]) == nullptr)
    {
      result[length] = source[i];
      ++length;
    }
  }
  return length;
}

char* isaev::excludeSecond(char* result, const char* first, const char* second) noexcept
{
  const std::size_t length = isaev::appendMissingSymbols(result, 0, first, second);
  result[length] = '\0';
  return result;
}

char* isaev::getUncommonSymbols(char* result, const char* first, const char* second) noexcept
{
  const std::size_t middle = isaev::appendMissingSymbols(result, 0, second, first);
  const std::size_t length = isaev::appendMissingSymbols(result, middle, first, second);
  result[length] = '\0';
  return result;
}

int main()
{
  const char* const excludeSecondLiteral = "abc";
  const char* const uncommonSecondLiteral = "abc_ef";

  char* input = nullptr;
  char* excluded = nullptr;
  char* uncommon = nullptr;
  int status = 0;
  try
  {
    input = isaev::readLine(std::cin);
    const std::size_t inputLength = isaev::getLength(input);

    const std::size_t excludedSize = isaev::addSizes(inputLength, isaev::terminatorSize);
    excluded = new char[excludedSize]();
    isaev::excludeSecond(excluded, input, excludeSecondLiteral);

    const std::size_t totalLength = isaev::addSizes(inputLength, isaev::getLength(uncommonSecondLiteral));
    const std::size_t uncommonSize = isaev::addSizes(totalLength, isaev::terminatorSize);
    uncommon = new char[uncommonSize]();
    isaev::getUncommonSymbols(uncommon, input, uncommonSecondLiteral);

    std::cout << excluded << '\n';
    std::cout << uncommon << '\n';
    std::cout.flush();
    if (!std::cout)
    {
      throw std::runtime_error("Output error");
    }
  }
  catch (const std::exception& error)
  {
    std::cerr << "Error: " << error.what() << '\n';
    status = isaev::errorCode;
  }
  delete[] uncommon;
  delete[] excluded;
  delete[] input;
  return status;
}
