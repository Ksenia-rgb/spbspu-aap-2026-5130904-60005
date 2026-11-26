#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <new>

namespace evsyukov {

  const int k_arg_error = 1;
  const int k_data_error = 2;
  const int k_arg_count = 4;
  const int k_static_mode = 1;
  const size_t k_static_limit = 10000;
  const size_t k_dynamic_limit = 100000000;

  size_t min(size_t a, size_t b)
  {
    size_t min_arg = 0;
    if (a < b) {
      min_arg = a;
    } else {
      min_arg = b;
    }
    return min_arg;
  }

  size_t countLocalMax(const int* a, size_t rows, size_t cols)
  {
    size_t count = 0;
    for (size_t i = 1; i + 1 < rows; ++i) {
      for (size_t j = 1; j + 1 < cols; ++j) {
        bool is_max = true;
        for (size_t ni = i - 1; ni <= i + 1 && is_max; ++ni) {
          for (size_t nj = j - 1; nj <= j + 1; ++nj) {
            if (ni == i && nj == j) {
              continue;
            }
            if (a[i * cols + j] <= a[ni * cols + nj]) {
              is_max = false;
              break;
            }
          }
        }
        if (is_max) {
          ++count;
        }
      }
    }
    return count;
  }

  bool fillIncWav(int* a, size_t rows, size_t cols)
  {
    const long long max_int = std::numeric_limits< int >::max();
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        const size_t layer = min(min(i, j), min(rows - 1 - i, cols - 1 - j)) + 1;
        if (a[i * cols + j] > max_int - static_cast< long long >(layer)) {
          return false;
        }
        a[i * cols + j] += static_cast< int >(layer);
      }
    }
    return true;
  }

  int process(std::ifstream& in, int* a, size_t rows, size_t cols, const char* out_name)
  {
    for (size_t i = 0; i < rows * cols; ++i) {
      if (!(in >> a[i])) {
        std::cerr << "Error arguments matrix\n";
        return k_data_error;
      }
    }
    char extra = 0;
    in >> extra;
    if (!in.eof()) {
      std::cerr << "Error arguments matrix\n";
      return k_data_error;
    }
    const size_t count = countLocalMax(a, rows, cols);
    std::ofstream out(out_name);
    if (!out) {
      std::cerr << "Cannot open output\n";
      return k_data_error;
    }
    const bool is_overflow = !fillIncWav(a, rows, cols);
    if (is_overflow) {
      std::cerr << "FLL-INC-WAV error\n";
    } else {
      out << rows << " " << cols;
      for (size_t i = 0; i < rows * cols; ++i) {
        out << " " << a[i];
      }
      out << "\n";
    }
    out << count << "\n";
    if (is_overflow) {
      return k_data_error;
    }
    return 0;
  }

  int run(int argc, char** argv)
  {
    if (argc != k_arg_count) {
      std::cerr << "Invalid_input\n";
      return k_arg_error;
    }
    const char* const num_arg = argv[1];
    if (num_arg[0] == '\0' || num_arg[1] != '\0' || (num_arg[0] != '1' && num_arg[0] != '2')) {
      std::cerr << "Error first parameter\n";
      return k_arg_error;
    }
    const int num = num_arg[0] - '0';
    std::ifstream in(argv[2]);
    if (!in) {
      std::cerr << "Invalid_file\n";
      return k_data_error;
    }
    long long raw_rows = 0, raw_cols = 0;
    if (!(in >> raw_rows >> raw_cols) || raw_rows < 0 || raw_cols < 0) {
      std::cerr << "Error rows or cols/over_flow\n";
      return k_data_error;
    }
    const size_t rows = static_cast< size_t >(raw_rows);
    const size_t cols = static_cast< size_t >(raw_cols);
    if (rows > k_dynamic_limit || cols > k_dynamic_limit) {
      std::cerr << "Error rows or cols\n";
      return k_data_error;
    }
    if (cols > 0 && rows > k_dynamic_limit / cols) {
      std::cerr << "Error rows or cols\n";
      return k_data_error;
    }
    if (num == k_static_mode) {
      if (rows * cols > k_static_limit) {
        std::cerr << "Matrix is too large for static array\n";
        return k_data_error;
      }
      int buf[k_static_limit];
      return process(in, buf, rows, cols, argv[3]);
    }
    int* matrix = nullptr;
    try {
      matrix = new int[rows * cols];
    } catch (const std::bad_alloc&) {
      std::cerr << "Not enough memory\n";
      return k_data_error;
    }
    const int code = process(in, matrix, rows, cols, argv[3]);
    delete[] matrix;
    return code;
  }
}

int main(int argc, char** argv)
{
  return evsyukov::run(argc, argv);
}
