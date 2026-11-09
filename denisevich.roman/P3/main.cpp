#include <iostream>
#include <fstream>
#include <limits>
#include <new>
#include <cstddef>
namespace denisevich {
  int* allocateArray(const char* mode, const std::size_t n, const std::size_t m)
  {
    int* back = nullptr;
    const char mode_2 = '2';
    if (mode[0] == mode_2) {
      back = new int[n * m];
    }
    return back;
  }
  std::size_t getMin(std::size_t n, std::size_t m)
  {
    std::size_t back = 0;
    if (n >= m) {
      back = m;
    } else {
      back = n;
    }
    return back;
  }
  std::size_t countLocalMin(const int* data, std::size_t n, std::size_t m)
  {
    std::size_t enumerator = 0;
    for (std::size_t str = 1; str < n - 1; str++) {
      for (std::size_t pillar = 2; pillar < m; pillar++) {
        const std::size_t index = str * m - 1 + pillar;
        const int number = data[index];
        const bool less_than_row = (number < data[index - 1]) && (number < data[index + 1]);
        const bool less_than_upper
            = (number < data[index - m - 1]) && (number < data[index - m]) && (number < data[index - m + 1]);
        const bool less_than_lower
            = (number < data[index + m - 1]) && (number < data[index + m]) && (number < data[index + m + 1]);
        if (less_than_row && less_than_upper && less_than_lower) {
          ++enumerator;
        }
      }
    }
    return enumerator;
  }
  long long int findMaxDiagSum(const int* data, std::size_t n, std::size_t m)
  {
    long long int total = 0;
    long long int prev_total = std::numeric_limits< long long >::min();
    long long int first = static_cast< long long >(n) - static_cast< long long >(m);
    long long int second = static_cast< long long >(m) - static_cast< long long >(n);
    std::size_t str = 0;
    std::size_t pillar = 1;
    if (first <= 0) {
      first = 0;
    }
    if (second <= 0) {
      second = 0;
    }
    std::size_t count = getMin(m, n);
    str += 1;
    std::size_t prev_str = 0;
    while (first != 0) {
      prev_str = str;
      for (std::size_t i = 0; i < count; i++) {
        total += data[str * m - 1 + pillar];
        pillar += 1;
        str += 1;
      }
      str = prev_str + 1;
      pillar = 1;
      if (total > prev_total) {
        prev_total = total;
      }
      total = 0;
      first--;
    }
    count -= 1;
    pillar = 1;
    while (count != 0) {
      prev_str = str;
      for (std::size_t j = count; j > 0; j--) {
        total += data[str * m - 1 + pillar];
        pillar += 1;
        str += 1;
      }
      pillar = 1;
      str = prev_str + 1;
      if (total > prev_total) {
        prev_total = total;
      }
      total = 0;
      count--;
    }
    count = getMin(m, n);
    str = 0;
    pillar = 2;
    std::size_t prev_pillar = 0;
    while (second != 0) {
      prev_pillar = pillar;
      for (std::size_t i = 0; i < count; i++) {
        total += data[str * m - 1 + pillar];
        pillar += 1;
        str += 1;
      }
      pillar = prev_pillar + 1;
      str = 0;
      if (total > prev_total) {
        prev_total = total;
      }
      total = 0;
      second--;
    }
    count -= 1;
    str = 0;
    while (count != 0) {
      prev_pillar = pillar;
      for (std::size_t j = count; j > 0; j--) {
        total += data[str * m - 1 + pillar];
        pillar += 1;
        str += 1;
      }
      str = 0;
      pillar = prev_pillar + 1;
      if (total > prev_total) {
        prev_total = total;
      }
      total = 0;
      count--;
    }
    return prev_total;
  }
}
int main(int argc, char** argv)
{
  const int invalid_input = 1;
  const int all_good = 0;
  const int incorrect_content = 2;
  const int arguments_number = 4;
  const char mode1 = '1';
  const char mode2 = '2';
  if (argc != arguments_number) {
    std::cerr << "Wrong input" << '\n';
    return invalid_input;
  }
  const char* const mode = argv[1];
  const char* const input_path = argv[2];
  const char* const output_path = argv[3];
  if ((mode[0] != mode1 && mode[0] != mode2) || mode[1] != '\0') {
    std::cerr << "Wrong mode" << '\n';
    return invalid_input;
  }
  std::ifstream input{input_path};
  if (!input) {
    std::cerr << "Wrong datafile" << '\n';
    return invalid_input;
  }
  int cursor = 0;
  std::size_t quantity = 0;
  std::size_t lines = 0;
  std::size_t columns = 0;
  while (input >> cursor) {
    if (quantity < 1) {
      if (cursor < 0) {
        std::cerr << "Wrong size" << '\n';
        return incorrect_content;
      }
      lines = cursor;
    } else if (quantity == 1) {
      if (cursor < 0) {
        std::cerr << "Wrong size" << '\n';
        return incorrect_content;
      }
      columns = cursor;
    }
    quantity++;
  }
  if (!input.eof()) {
    std::cerr << "Incorrect content" << '\n';
    return incorrect_content;
  }
  std::ofstream output{output_path};
  if (!output) {
    std::cerr << "Wrong targeted file" << '\n';
    return invalid_input;
  }
  if (quantity == 2 && (lines == 0 && columns == 0)) {
    output << all_good << '\n' << all_good;
    return all_good;
  } else if (quantity <= 2) {
    std::cerr << "Incorrect content" << '\n';
    return incorrect_content;
  }
  quantity -= 2;
  if (quantity != lines * columns) {
    std::cerr << "Incorrect content" << '\n';
    return incorrect_content;
  } else if (lines == 1 && columns == 1) {
    output << all_good << '\n' << all_good;
    return all_good;
  }
  try {
    int* data = denisevich::allocateArray(mode, lines, columns);
    const std::size_t array_size = 10000;
    int array[array_size] = {};
    if (data == nullptr) {
      data = array;
    }
    input.clear();
    input.seekg(0);
    long long skip = 0;
    input >> skip >> skip;
    for (std::size_t i = 0; i < quantity; i++) {
      input >> data[i];
    }
    const std::size_t result_2 = denisevich::countLocalMin(data, lines, columns);
    const long long result_14 = denisevich::findMaxDiagSum(data, lines, columns);
    if (mode[0] == mode2) {
      delete[] data;
    }
    if (!(output << result_14)) {
      std::cerr << "Targeted file error" << '\n';
      return incorrect_content;
    }
    output << '\n';
    if (!(output << result_2)) {
      std::cerr << "Targeted file error" << '\n';
      return incorrect_content;
    }
    return all_good;
  } catch (const std::bad_alloc&) {
    std::cerr << "Memory cannot be allocated" << '\n';
    return incorrect_content;
  }
}
