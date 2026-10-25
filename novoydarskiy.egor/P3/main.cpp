#include <iostream>
#include <fstream>
#include <cstddef>
#include <limits>
#include <new>

namespace novoydarskiy {

  bool mode(const char* s, int& mode)
  {
    if (s == nullptr) {
      return false;
    }
    if (s[0] == '\0' || s[1] != '\0') {
      return false;
    }
    if (s[0] == '1') {
      mode = 1;
      return true;
    }
    if (s[0] == '2') {
      mode = 2;
      return true;
    }
    return false;
  }

  long long longest_cols_series(const long long* data, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }
    long long best_col = 1;
    size_t best_len = 0;
    for (size_t c = 0; c < cols; ++c) {
      size_t current_len = 1;
      size_t max_len = 1;
      for (size_t r = 1; r < rows; ++r) {
        if (data[r * cols + c] == data[(r - 1) * cols + c]) {
          ++current_len;
          if (current_len > max_len) {
            max_len = current_len;
          }
        } else {
          current_len = 1;
        }
      }
      if (max_len > best_len) {
        best_len = max_len;
        best_col = static_cast< long long >(c) + 1;
      }
    }
    return best_col;
  }

  long long longest_rows_series(const long long* data, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }
    long long best_row = 1;
    size_t best_len = 0;
    for (size_t r = 0; r < rows; ++r) {
      size_t current_len = 1;
      size_t max_len = 1;
      for (size_t c = 1; c < cols; ++c) {
        if (data[r * cols + c] == data[r * cols + (c - 1)]) {
          ++current_len;
          if (current_len > max_len) {
            max_len = current_len;
          }
        } else {
          current_len = 1;
        }
      }
      if (max_len > best_len) {
        best_len = max_len;
        best_row = static_cast< long long >(r) + 1;
      }
    }
    return best_row;
  }

  bool read_sizes(std::istream& in, size_t& rows, size_t& cols)
  {
    long long r = 0, c = 0;
    if (!(in >> r >> c)) {
      return false;
    }
    if (r < 0 || c < 0) {
      return false;
    }
    rows = static_cast< size_t >(r);
    cols = static_cast< size_t >(c);
    return true;
  }

  bool write_results(std::ostream& out, const long long* data, size_t rows, size_t cols)
  {
    long long res11 = longest_cols_series(data, rows, cols);
    long long res12 = longest_rows_series(data, rows, cols);
    out << res12 << '\n' << res11 << '\n';
    return out.good();
  }

  bool fixed_massiv(std::istream& in, std::ostream& out)
  {
    size_t rows = 0, cols = 0;
    if (!read_sizes(in, rows, cols)) {
      return false;
    }

    const size_t CAPACITY = 10000;
    if (rows != 0 && cols > std::numeric_limits< size_t >::max() / rows) {
      return false;
    }
    if (rows * cols > CAPACITY) {
      return false;
    }

    long long buffer[CAPACITY];
    for (size_t i = 0; i < rows * cols; ++i) {
      if (!(in >> buffer[i])) {
        return false;
      }
    }
    long long extra = 0;
    if (in >> extra) {
      return false;
    }

    return write_results(out, buffer, rows, cols);
  }

  bool dynamic_massiv(std::istream& in, std::ostream& out)
  {
    size_t rows = 0, cols = 0;
    if (!read_sizes(in, rows, cols)) {
      return false;
    }
    if (rows != 0 && cols > std::numeric_limits< size_t >::max() / rows) {
      return false;
    }
    size_t total = rows * cols;
    long long* buffer = nullptr;
    if (total > 0) {
      buffer = new (std::nothrow) long long[total];
      if (buffer == nullptr) {
        return false;
      }
    }

    for (size_t i = 0; i < total; ++i) {
      if (!(in >> buffer[i])) {
        delete[] buffer;
        return false;
      }
    }
    long long extra = 0;
    if (in >> extra) {
      delete[] buffer;
      return false;
    }

    bool res_of_func = write_results(out, buffer, rows, cols);
    delete[] buffer;
    return res_of_func;
  }
}

int main(int argc, char** argv)
{
  if (argc != 4) {
    std::cerr << "Ошибка: требуется 3 аргумента\n";
    return 1;
  }

  int mode = 0;
  if (!novoydarskiy::mode(argv[1], mode)) {
    std::cerr << "Ошибка: первый параметр - 1 или 2\n";
    return 1;
  }

  const char* input_path = argv[2];
  const char* output_path = argv[3];

  std::ifstream input{input_path};
  if (!input) {
    std::cerr << "Ошибка: не получается открыть входной файл\n";
    return 2;
  }

  std::ofstream output{output_path};
  if (!output) {
    std::cerr << "Ошибка: не получается открыть выходной файл\n";
    return 2;
  }

  bool completed = false;
  if (mode == 1) {
    completed = novoydarskiy::fixed_massiv(input, output);
  } else {
    completed = novoydarskiy::dynamic_massiv(input, output);
  }

  if (!completed) {
    std::cerr << "Ошибка чтения входного файла\n";
    return 2;
  }

  return 0;
}
