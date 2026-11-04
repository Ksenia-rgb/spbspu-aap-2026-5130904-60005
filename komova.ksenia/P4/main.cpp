#include <iostream>
#include <exception>
#include <stdexcept>
#include <new>
#include <cstddef>
#include <limits>
#include <cctype>

namespace komova {
  const size_t max_sizet = std::numeric_limits< std::size_t >::max();
  void latTwo(const char* const str1, const char* const str2)
  {
    const size_t count_letters = 26;
    int letters[count_letters]{};
    size_t j = 0;
    while (str1[j] != '\0') {
      if (std::isalpha(str1[j])) {
        letters[std::tolower(str1[j]) - 'a'] = 1;
      }
      j++;
    }
    j = 0;
    while (str2[j] != '\0') {
      if (std::isalpha(str2[j])) {
        letters[std::tolower(str2[j]) - 'a'] = 1;
      }
      j++;
    }
    for (size_t i = 0; i < count_letters; i++) {
      if (letters[i] == 1) {
        std::cout << char('a' + i);
      }
    }
  }

  int isGlasn(char c)
  {
    c = std::tolower(c);
    if (c == 'a' or c == 'e' or c == 'y' or c == 'u' or c == 'i' or c == 'o') {
      return 1;
    }
    return 0;
  }

  void rmvVow(const char* const str)
  {
    size_t j = 0;
    while (str[j] != '\0') {
      if (!(isGlasn(str[j]))) {
        std::cout << str[j];
      }
      j++;
    }
  }

  char* readStr()
  {
    size_t len = 10;
    size_t real_len = 0;
    char* str = new char[len];
    char c = 0;
    while (std::cin.get(c) and c != '\n') {
      if (real_len + 1 >= len) {
        if (max_sizet / 2 < len) {
          delete[] str;
          throw std::overflow_error("");
        }
        len *= 2;
        char* const new_str = new char[len];
        for (size_t i = 0; i < real_len; i++) {
          new_str[i] = str[i];
        }
        delete[] str;
        str = new_str;
      }
      str[real_len++] = c;
    }
    str[real_len] = '\0';
    if (real_len == 0) {
      delete[] str;
      throw std::invalid_argument("");
    }
    return str;
  }
}

int main()
{
  const int bad_alloc = 1;
  const int overflow_error = 1;
  const int empty_imput = 1;
  try {
    const char* const str = komova::readStr();
    komova::latTwo(str, "def ghk");
    std::cout << std::endl;
    komova::rmvVow(str);
    delete[] str;
  } catch (std::bad_alloc&) {
    return bad_alloc;
  } catch (std::overflow_error&) {
    return overflow_error;
  } catch (std::invalid_argument&) {
    return empty_imput;
  }
}
