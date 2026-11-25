#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <limits>

size_t sharedSymbols(const char* src, char* res)
{
  const size_t BUFF_CAP = static_cast< size_t >(std::numeric_limits< unsigned char >::max()) + 1;
  bool present[BUFF_CAP] = {false};

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
      res[i++] = static_cast< char >(c);
    }
  }
  res[i] = '\0';
  return i;
}

size_t excludeSecond(const char* s1, const char* s2, char* res)
{
  const size_t BUFF_CAP = static_cast< size_t >(std::numeric_limits< unsigned char >::max()) + 1;
  bool in_second[BUFF_CAP] = {false};

  for (const char* p = s2; *p != '\0'; ++p) {
    in_second[static_cast< unsigned char >(*p)] = true;
  }

  size_t i = 0;
  for (const char* p = s1; *p != '\0'; ++p) {
    if (!in_second[static_cast< unsigned char >(*p)]) {
      res[i++] = *p;
    }
  }
  res[i] = '\0';
  return i;
}

int main()
{
  const char* const second_str = "abc";
  const size_t BUFF_CAP = static_cast< size_t >(std::numeric_limits< unsigned char >::max()) + 1;
  const int error = 1;

  size_t buffer_size = BUFF_CAP;
  char* input_str = static_cast< char* >(std::malloc(buffer_size * sizeof(char)));
  if (!input_str) {
    std::cerr << "Memory allocation error" << std::endl;
    return error;
  }

  size_t len = 0;
  char c = '\0';
  while (std::cin.get(c) && c != '\n') {
    input_str[len++] = c;
    if (len == buffer_size) {
      if (buffer_size > std::numeric_limits< size_t >::max() / 2) {
        std::cerr << "Buffer size overflow\n";
        std::free(input_str);
        return error;
      }
      buffer_size *= 2;
      char* const temp = static_cast< char* >(std::realloc(input_str, buffer_size * sizeof(char)));
      if (temp == nullptr) {
        std::cerr << "Memory allocation error" << std::endl;
        std::free(input_str);
        return error;
      }
      input_str = temp;
    }
  }
  input_str[len] = '\0';

  if (std::cin.fail()) {
    std::cerr << "Input error" << std::endl;
    std::free(input_str);
    return error;
  }

  const size_t RES_14_SIZE = 27;
  char* const result_14 = static_cast< char* >(std::malloc(RES_14_SIZE * sizeof(char)));
  bool ok_14 = false;
  if (result_14 != nullptr) {
    sharedSymbols(input_str, result_14);
    ok_14 = true;
  }

  char* const result_15 = static_cast< char* >(std::malloc((len + 1) * sizeof(char)));
  bool ok_15 = false;
  if (result_15 != nullptr) {
    excludeSecond(input_str, second_str, result_15);
    ok_15 = true;
  }

  if (!ok_14 && !ok_15) {
    std::cerr << "Both transformations failed" << std::endl;
    std::free(input_str);
    return error;
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
