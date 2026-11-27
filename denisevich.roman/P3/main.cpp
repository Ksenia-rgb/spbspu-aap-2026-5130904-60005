#include <iostream>
#include <fstream>
#include <limits>
#include <new>
#include <cstddef>

namespace denisevich {
  const std::size_t array_size = 10000;

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
    if (n < 3 || m < 3) {
      return 0;
    }
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
    if (n <= 1 || m <= 1) {
      return 0;
    }
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

  void readMatrix(std::istream& input, int* data, std::size_t count)
  {
    int skipped = 0;
    input.clear();
    input.seekg(0);
    input >> skipped >> skipped;
    for (std::size_t i = 0; i < count; ++i) {
      input >> data[i];
    }
  }

  bool writeResults(std::ostream& output, const int* data, std::size_t cols, std::size_t rows)
  {
    const long long max_sum = findMaxDiagSum(data, rows, cols);
    const std::size_t localmin = countLocalMin(data, rows, cols);
    output << max_sum << '\n' << localmin << '\n';
    return output.good();
  }

  bool stackprocessing(std::ostream& output, std::istream& input, std::size_t rows, std::size_t cols)
  {
    int array[array_size] = {};
    readMatrix(input, array, rows * cols);
    return writeResults(output, array, cols, rows);
  }

  bool heapprocessing(std::ostream& output, std::istream& input, std::size_t rows, std::size_t cols)
  {
    int* const data = new int[cols * rows];
    readMatrix(input, data, rows * cols);
    const bool is_written = writeResults(output, data, cols, rows);
    delete[] data;
    return is_written;
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
  long long rows_in = 0;
  long long cols_in = 0;
  if (!(input >> rows_in >> cols_in) || rows_in < 0 || cols_in < 0) {
    std::cerr << "Incorrect content" << '\n';
    return incorrect_content;
  }
  const std::size_t lines = static_cast< std::size_t >(rows_in);
  const std::size_t columns = static_cast< std::size_t >(cols_in);
  if (columns != 0 && lines > std::numeric_limits< std::size_t >::max() / columns) {
    std::cerr << "Incorrect content" << '\n';
    return incorrect_content;
  }
  std::size_t quantity = 0;
  while (input >> cursor) {
    ++quantity;
  }
  if (!input.eof()) {
    std::cerr << "Incorrect content" << '\n';
    return incorrect_content;
  }
  if (quantity != lines * columns) {
    std::cerr << "Incorrect content" << '\n';
    return incorrect_content;
  }
  if (quantity == 0 && (lines != 0 || columns != 0)) {
    std::cerr << "Incorrect content" << '\n';
    return incorrect_content;
  }
  if (mode[0] == mode1 && lines * columns > denisevich::array_size) {
    std::cerr << "Too much" << '\n';
    return invalid_input;
  }
  std::ofstream output{output_path};
  if (!output) {
    std::cerr << "Wrong targeted file" << '\n';
    return invalid_input;
  }
  try {
    bool is_written = false;
    if (mode[0] == mode2) {
      is_written = denisevich::heapprocessing(output, input, lines, columns);
    } else {
      is_written = denisevich::stackprocessing(output, input, lines, columns);
    }
    if (!is_written) {
      std::cerr << "Targeted file error" << '\n';
      return incorrect_content;
    }
    return all_good;
  } catch (const std::bad_alloc&) {
    std::cerr << "Memory cannot be allocated" << '\n';
    return incorrect_content;
  }
}
