#include <iostream>
#include <fstream>
#include <cstddef>
#include <limits>
#include <new>

namespace novoydarskiy {

  const int all_is_good = 0;
  const int invalid_arguments = 1;
  const int invalid_input_file = 2;
  const int arg_count = 4;
  const int first_mode = 1;
  const int second_mode = 2;
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

  size_t longestColsSeries(const long long* data, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }
    size_t best_col = 1;
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
        best_col = c + 1;
      }
    }
    return best_col;
  }

  size_t longestRowsSeries(const long long* data, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }
    size_t best_row = 1;
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
        best_row = r + 1;
      }
    }
    return best_row;
  }

  bool readSizes(std::istream& in, size_t& rows, size_t& cols)
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

  bool writeResults(std::ostream& out, const long long* data, size_t rows, size_t cols)
  {
    const long long res11 = longestColsSeries(data, rows, cols);
    const long long res12 = longestRowsSeries(data, rows, cols);
    out << res12 << '\n' << res11 << '\n';
    return out.good();
  }

  bool readMassiv(std::istream& in, std::ostream& out, int& mode)
  {
    size_t rows = 0, cols = 0;
    const size_t CAPACITY = 10000;

    if (!readSizes(in, rows, cols)) {
      return false;
    }
    if (rows != 0 && cols > std::numeric_limits< size_t >::max() / rows) {
      return false;
    }

    if (mode == first_mode) {
      if (rows * cols > CAPACITY) {
        return false;
      }

      long long buffer[CAPACITY];
      for (size_t i = 0; i < rows * cols; ++i) {
        if (!(in >> buffer[i])) {
          return false;
        }
      }
      char extra = '\0';
      if (!(in >> extra).eof()) {
        return false;
      }
      return writeResults(out, buffer, rows, cols);
    }

    if (mode == second_mode) {
      const size_t total = rows * cols;
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
      char extra = '\0';
      if (!(in >> extra).eof()) {
        delete[] buffer;
        return false;
      }
      const bool res_of_func = writeResults(out, buffer, rows, cols);
      delete[] buffer;
      return res_of_func;
    }
    return false;
  }
}

int main(int argc, char** argv)
{
  if (argc != novoydarskiy::arg_count) {
    std::cerr << "Ошибка: требуется 3 аргумента\n";
    return novoydarskiy::invalid_arguments;
  }

  int mode = 0;
  if (!novoydarskiy::mode(argv[1], mode)) {
    std::cerr << "Ошибка: первый аргумент - 1 или 2\n";
    return novoydarskiy::invalid_arguments;
  }

  const char* const input_path = argv[2];
  const char* const output_path = argv[3];

  std::ifstream input{input_path};
  if (!input) {
    std::cerr << "Ошибка: не получается открыть входной файл\n";
    return novoydarskiy::invalid_input_file;
  }

  std::ofstream output{output_path};
  if (!output) {
    std::cerr << "Ошибка: не получается открыть выходной файл\n";
    return novoydarskiy::invalid_input_file;
  }

  bool completed = novoydarskiy::readMassiv(input, output, mode);

  if (!completed) {
    std::cerr << "Ошибка чтения входного файла\n";
    return novoydarskiy::invalid_input_file;
  }

  return novoydarskiy::all_is_good;
}
