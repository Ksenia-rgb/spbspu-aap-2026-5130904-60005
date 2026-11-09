#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>

size_t sharedSymbols(const char* src, char* dest)
{
  bool present[256] = {false};

  for (const char* p = src; *p != '\0'; ++p) {
    unsigned char c = static_cast< unsigned char >(*p);
    if (c >= 'A' && c <= 'Z') {
      c = static_cast< unsigned char >(std::tolower(c));
    }
    present[c] = true;
  }

  size_t i = 0;
  for (int c = 'a'; c <= 'z'; ++c) {
    if (!present[c]) {
      dest[i++] = static_cast< char >(c);
    }
  }
  dest[i] = '\0';
  return i;
}

size_t excludeSecond(const char* s1, const char* s2, char* dest)
{
  bool inSecond[256] = {false};

  for (const char* p = s2; *p; ++p) {
    inSecond[static_cast< unsigned char >(*p)] = true;
  }

  size_t i = 0;
  for (const char* p = s1; *p; ++p) {
    if (!inSecond[static_cast< unsigned char >(*p)]) {
      dest[i++] = *p;
    }
  }
  dest[i] = '\0';
  return i;
}

int main()
{
  const char* second_str = "abc";

  size_t buffer_size = 256;
  char* input_str = static_cast< char* >(std::malloc(buffer_size * sizeof(char)));
  if (!input_str) {
    std::cerr << "Memory allocation error" << std::endl;
    return 1;
  }

  size_t len = 0;
  char c;
  while (std::cin.get(c) && c != '\n') {
    input_str[len++] = c;
    if (len == buffer_size) {
      buffer_size *= 2;
      char* temp = static_cast< char* >(std::realloc(input_str, buffer_size * sizeof(char)));
      if (!temp) {
        std::cerr << "Memory allocation error" << std::endl;
        std::free(input_str);
        return 1;
      }
      input_str = temp;
    }
  }
  input_str[len] = '\0';

  if (std::cin.fail()) {
    std::cerr << "Input error" << std::endl;
    std::free(input_str);
    return 1;
  }

  char* result_14 = static_cast< char* >(std::malloc(27 * sizeof(char)));
  int ok_14 = 0;
  if (result_14 != nullptr) {
    sharedSymbols(input_str, result_14);
    ok_14 = 1;
  }

  char* result_15 = static_cast< char* >(std::malloc((len + 1) * sizeof(char)));
  int ok_15 = 0;
  if (result_15 != nullptr) {
    excludeSecond(input_str, second_str, result_15);
    ok_15 = 1;
  }

  if (!ok_14 && !ok_15) {
    std::cerr << "Both transformations failed" << std::endl;
    std::free(input_str);
    return 1;
  }
  if (ok_14 && !ok_15) {
    std::cout << result_14 << std::endl;
    std::cerr << "Transformation 15 is failed" << std::endl;
  }
  if (ok_15 && !ok_14) {
    std::cout << result_15 << std::endl;
    std::cerr << "Transformation 14 is failed" << std::endl;
  }
  if (ok_15 && ok_14) {
    std::cout << result_14 << std::endl;
    std::cout << result_15 << std::endl;
  }
  std::free(input_str);
  std::free(result_14);
  std::free(result_15);

  return 0;
}
