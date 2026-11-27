
#include <cctype>
#include <cstring>
#include <iostream>

const int success = 0;
const int error = 1;

namespace demyanov {

  char *readLine()
  {
    int size = 1;
    int length = 0;

    char *str = new char[size];
    char c = '\0';

    while (std::cin.get(c) && c != '\n') {
      if (length + 1 >= size) {
        size *= 2;
        char *const temp = new char[size];

        for (int i = 0; i < length; i++) {
          temp[i] = str[i];
        }

        delete[] str;
        str = temp;
      }

      str[length] = c;
      length++;
    }

    if (!std::cin && length == 0) {
      delete[] str;
      return nullptr;
    }

    str[length] = '\0';

    return str;
  }

  int var16(const char *const str_1, const char *const str_2, char *const result, int max_length)
  {
    int str_1_len = strlen(str_1);

    for (int i = 0; i < str_1_len; i++) {
      result[i] = str_1[i];
    }

    result[str_1_len] = '\0';

    for (int i = 0; str_2[i] != '\0'; i++) {
      if (isdigit(static_cast< unsigned char >(str_2[i]))) {
        if (str_1_len + 1 < max_length) {
          result[str_1_len] = str_2[i];
          str_1_len++;
          result[str_1_len] = '\0';
        } else {
          std::cerr << "error: not enough memory\n";
          return error;
        }
      }
    }

    return success;
  }

  int var5(const char *const str_1, const char *const str_2, char *const result, int max_length)
  {
    int result_len = 0;

    for (int i = 0; str_2[i] != '\0'; i++) {
      if (strchr(str_1, str_2[i]) == nullptr) {
        if (result_len + 1 < max_length) {
          result[result_len] = str_2[i];
          result_len++;
        } else {
          std::cerr << "error: not enough memory\n";
          return error;
        }
      }
    }

    for (int i = 0; str_1[i] != '\0'; i++) {
      if (strchr(str_2, str_1[i]) == nullptr) {
        if (result_len + 1 < max_length) {
          result[result_len] = str_1[i];
          result_len++;
        } else {
          std::cerr << "error: not enough memory\n";
          return error;
        }
      }
    }

    result[result_len] = '\0';

    return success;
  }
}

int main()
{
  using demyanov::readLine;
  using demyanov::var16;
  using demyanov::var5;

  const char *const str_1 = readLine();

  if (str_1 == nullptr) {
    std::cerr << "error: no input\n";
    return error;
  }

  char *str_2 = readLine();

  if (str_2 == nullptr) {
    str_2 = new char[1];
    str_2[0] = '\0';
  }

  const int max_str_length = strlen(str_1) + strlen(str_2) + 1;

  char *const result = new char[max_str_length];

  if (var5(str_1, str_2, result, max_str_length) != success) {
    delete[] str_1;
    delete[] str_2;
    delete[] result;
    return error;
  }

  std::cout << result << '\n';

  if (var16(str_1, str_2, result, max_str_length) != success) {
    delete[] str_1;
    delete[] str_2;
    delete[] result;
    return error;
  }

  std::cout << result << '\n';

  delete[] str_1;
  delete[] str_2;
  delete[] result;

  return success;
}
