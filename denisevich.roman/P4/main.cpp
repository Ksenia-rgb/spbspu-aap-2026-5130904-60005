#include <iostream>
#include <new>
#include <cstring>
#include <cctype>

namespace denisevich {
  char* removelatin(const char* string, char* buffer)
  {
    std::size_t j = 0;
    for (std::size_t i = 0; string[i] != '\0'; ++i) {
      if (!std::isalpha(static_cast< unsigned char >(string[i]))) {
        buffer[j++] = string[i];
      }
    }
    buffer[j] = '\0';
    return buffer;
  }
  int hasLetter(const char* str, char letter)
  {
    for (std::size_t i = 0; str[i] != '\0'; ++i) {
      if (std::tolower(static_cast< unsigned char >(str[i])) == letter) {
        return 1;
      }
    }
    return 0;
  }
  char* unitelatin(const char* str1, const char* str2, char* buffer)
  {
    std::size_t j = 0;
    for (char a = 'a'; a <= 'z'; ++a) {
      if (hasLetter(str1, a) || hasLetter(str2, a)) {
        buffer[j++] = a;
      }
    }
    buffer[j] = '\0';
    return buffer;
  }
  char* reading(std::size_t& size)
  {
    std::size_t capacity = 1;
    char* buf = new char[capacity];
    char c = 0;
    while (std::cin.get(c) && c != '\n') {
      if (size + 1 == capacity) {
        const std::size_t newCap = capacity * 2;
        char* tmp = nullptr;
        try {
          tmp = new char[newCap];
        } catch (const std::bad_alloc&) {
          delete[] buf;
          throw;
        }
        for (std::size_t k = 0; k < size; ++k) {
          tmp[k] = buf[k];
        }
        delete[] buf;
        buf = tmp;
        capacity = newCap;
      }
      buf[size++] = c;
    }
    buf[size] = '\0';
    return buf;
  }
}

int main()
{
  const char defline[] = "def ghk";
  std::size_t size = 0;
  char* line = nullptr;
  char* result = nullptr;
  char* result2 = nullptr;
  const int resultsize = 27;
  try {
    line = denisevich::reading(size);
    result = new char[resultsize]();
    result2 = new char[std::strlen(line) + 1]();
  } catch (const std::bad_alloc&) {
    std::cerr << "Memory allocation failed" << "\n";
    delete[] line;
    delete[] result;
    delete[] result2;
    return 1;
  }
  if (line[0] == '\0' && std::cin.eof()) {
    std::cerr << "Input is empty\n";
    delete[] line;
    delete[] result;
    delete[] result2;
    return 1;
  }
  result = denisevich::unitelatin(line, defline, result);
  result2 = denisevich::removelatin(line, result2);
  std::cout << result << "\n" << result2 << "\n";
  delete[] line;
  delete[] result;
  delete[] result2;
  return 0;
}
