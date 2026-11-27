#include <iostream>
#include <fstream>
#include <cstddef>
#include <limits>
#include <new>

namespace belykov {
  const int argument_error_code = 1;
  const int input_error_code = 2;
  const int mode_static = 1;
  const int mode_dynamic = 2;
  const size_t max_static_size = 10000;

  size_t numColLsr(const int* matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }
    size_t max_len = 1;
    size_t best_col_index = 1;
    for (size_t col = 0; col < cols; ++col) {
      size_t equal_cnt = 1;
      for (size_t row = 0; row + 1 < rows; ++row) {
        if (matrix[row * cols + col] == matrix[(row + 1) * cols + col]) {
          ++equal_cnt;
        } else {
          if (equal_cnt > max_len) {
            max_len = equal_cnt;
            best_col_index = col + 1;
          }
          equal_cnt = 1;
        }
      }
      if (equal_cnt > max_len) {
        max_len = equal_cnt;
        best_col_index = col + 1;
      }
    }
    return best_col_index;
  }

  long long maxSumMdg(const int* matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0 || (rows == 1 && cols == 1)) {
      return 0;
    }
    long long max_sum = std::numeric_limits< long long >::min();
    bool has_parallel = false;
    const size_t main_diagonal_index = cols - 1;
    for (size_t k = 0; k < (rows + cols - 1); ++k) {
      if (k == main_diagonal_index) {
        continue;
      }
      long long current_diagonal_sum = 0;
      for (size_t r = 0; r < rows; ++r) {
        if (k >= r && (k - r) < cols) {
          current_diagonal_sum += matrix[r * cols + (k - r)];
        }
      }
      if (!has_parallel || current_diagonal_sum > max_sum) {
        max_sum = current_diagonal_sum;
        has_parallel = true;
      }
    }
    return has_parallel ? max_sum : 0;
  }

  bool readMatrix(std::istream& input, int* matrix, size_t total)
  {
    for (size_t i = 0; i < total; ++i) {
      if (!(input >> matrix[i])) {
        std::cerr << "Error: Failed to read matrix elements or invalid data.\n";
        return false;
      }
    }
    int extra = 0;
    if (input >> extra) {
      std::cerr << "Error: Invalid data.\n";
      return false;
    }
    return true;
  }

  int process(const int* matrix, size_t rows, size_t cols, std::ostream& output)
  {
    output << numColLsr(matrix, rows, cols) << "\n";
    output << maxSumMdg(matrix, rows, cols) << "\n";
    if (!output.good()) {
      std::cerr << "Error: Failed to write to output stream.\n";
      return input_error_code;
    }
    return 0;
  }

  int runStatic(std::istream& input, std::ostream& output, size_t rows, size_t cols, size_t total)
  {
    if (total > max_static_size) {
      std::cerr << "Error: Matrix too large for static buffer.\n";
      return input_error_code;
    }
    int buffer[max_static_size] = {0};
    if (!readMatrix(input, buffer, total)) {
      return input_error_code;
    }
    return process(buffer, rows, cols, output);
  }

  int runDynamic(std::istream& input, std::ostream& output, size_t rows, size_t cols, size_t total)
  {
    int* const matrix = new (std::nothrow) int[total];
    if (matrix == nullptr) {
      std::cerr << "Error: Memory allocation failed.\n";
      return input_error_code;
    }
    int result = input_error_code;
    if (readMatrix(input, matrix, total)) {
      result = process(matrix, rows, cols, output);
    }
    delete[] matrix;
    return result;
  }

  int run(std::istream& input, std::ostream& output, int mode)
  {
    long long raw_rows = 0;
    long long raw_cols = 0;
    if (!(input >> raw_rows >> raw_cols) || raw_rows < 0 || raw_cols < 0) {
      std::cerr << "Error: Invalid matrix dimensions or empty file.\n";
      return input_error_code;
    }
    const size_t rows = static_cast< size_t >(raw_rows);
    const size_t cols = static_cast< size_t >(raw_cols);
    if (rows == 0 || cols == 0) {
      output << 0 << "\n" << 0 << "\n";
      return 0;
    }
    if (rows > std::numeric_limits< size_t >::max() / cols) {
      std::cerr << "Error: Matrix is too large.\n";
      return input_error_code;
    }
    const size_t total = rows * cols;
    if (mode == mode_static) {
      return runStatic(input, output, rows, cols, total);
    }
    return runDynamic(input, output, rows, cols, total);
  }
}

int main(int argc, char** argv)
{
  const int expected_argc = 4;
  const size_t arg_index_mode = 1;
  const size_t arg_index_input = 2;
  const size_t arg_index_output = 3;
  if (argc != expected_argc) {
    std::cerr << "Error: Wrong number of arguments.\n";
    return belykov::argument_error_code;
  }
  const char* const mode_str = argv[arg_index_mode];
  if (*mode_str == '\0' || *(mode_str + 1) != '\0' || (*mode_str != '1' && *mode_str != '2')) {
    std::cerr << "Error: Invalid first parameter (must be 1 or 2).\n";
    return belykov::argument_error_code;
  }
  const int mode = *mode_str - '0';
  std::ifstream input{argv[arg_index_input]};
  if (!input) {
    std::cerr << "Error: Cannot open input file.\n";
    return belykov::input_error_code;
  }
  std::ofstream output{argv[arg_index_output]};
  if (!output) {
    std::cerr << "Error: Cannot open output file.\n";
    return belykov::input_error_code;
  }
  return belykov::run(input, output, mode);
}
