#include <iostream>
#include <stdexcept>
#include <cstddef>
#include <new>
#include <limits>
#include <fstream>

namespace kozinskiy {

  const int invalid_argument_ec = 1;
  const int invalid_file_ec = 2;
  const int good_ec = 0;

  const size_t max_elements = 10000;
  const int max_int = std::numeric_limits< int >::max();
  const int min_int = std::numeric_limits< int >::min();
  const size_t max_sizet = std::numeric_limits< size_t >::max();

  void safeAdd(int& result, int value)
  {
    if (result > 0 && value > 0 && result > max_int - value) {
      throw std::overflow_error("too big int value");
    }
    if (result < 0 && value < 0 && result < min_int - value) {
      throw std::underflow_error("too small int value");
    }
    result += value;
  }

  bool parseNum(const char* str, int& value)
  {
    if (!str || *str == '\0') {
      return false;
    }
    int sign = 1;
    if (*str == '+') {
      str++;
    }
    if (*str == '-') {
      sign = -1;
      str++;
    }
    if (*str == '\0') {
      return false;
    }

    int acc = 0;
    for (; *str != '\0'; str++) {
      if (*str < '0' || *str > '9') {
        return false;
      }
      const int digit = *str - '0';
      if (acc > (max_int - digit) / 10) {
        return false;
      }
      acc = acc * 10 + digit;
    }
    value = sign * acc;
    return true;
  }

  size_t countColumnsNoAdjacentEquals(size_t rows, size_t cols, const int* matrix)
  {
    size_t count = 0;
    for (size_t c = 0; c < cols; c++) {
      bool has_adjacent = false;
      for (size_t r = 1; r < rows; r++) {
        if (matrix[(r - 1) * cols + c] == matrix[r * cols + c]) {
          has_adjacent = true;
          break;
        }
      }
      if (!has_adjacent) {
        count++;
      }
    }
    return count;
  }

  int minSumParallelMainDiagonal(size_t rows, size_t cols, const int* matrix)
  {
    size_t len = rows;
    if (cols < rows) {
      len = cols;
    }
    if (len < 2) {
      return 0;
    }

    int min_sum = max_int;
    for (size_t d = 1; d < len; d++) {
      int cur_sum = 0;
      for (size_t r = 0; r < len - d; r++) {
        safeAdd(cur_sum, matrix[r * cols + (r + d)]);
      }
      if (cur_sum < min_sum) {
        min_sum = cur_sum;
      }

      cur_sum = 0;
      for (size_t c = 0; c < len - d; c++) {
        safeAdd(cur_sum, matrix[(c + d) * cols + c]);
      }
      if (cur_sum < min_sum) {
        min_sum = cur_sum;
      }
    }
    return min_sum;
  }

  void parseMatrixValues(std::istream& input, int* matrix, size_t size)
  {
    for (size_t i = 0; i < size; i++) {
      if (!(input >> matrix[i])) {
        throw std::runtime_error("Error: invalid file content");
      }
    }
  }

  void processMatrices(std::istream& input, std::ostream& out, bool is_dynamic)
  {
    size_t rows = 0;
    size_t cols = 0;
    int stack_matrix[max_elements];
    bool is_first = true;

    while (true) {
      if (!(input >> rows)) {
        if (input.eof() && !is_first) {
          break;
        }
        throw std::runtime_error("Error: invalid file content");
      }
      if (!(input >> cols)) {
        throw std::runtime_error("Error: invalid file content");
      }

      if (is_dynamic) {
        if (cols != 0 && rows > max_sizet / cols) {
          throw std::runtime_error("Error: invalid file content");
        }
      } else {
        if (cols != 0 && rows > max_elements / cols) {
          throw std::runtime_error("Error: invalid file content");
        }
      }

      if (rows * cols == 0 && rows != cols) {
        throw std::runtime_error("Error: invalid file content");
      }

      int* matrix = nullptr;
      if (is_dynamic) {
        matrix = new int[rows * cols]();
      } else {
        matrix = stack_matrix;
      }

      try {
        parseMatrixValues(input, matrix, rows * cols);
        const size_t res1 = countColumnsNoAdjacentEquals(rows, cols, matrix);
        const int res2 = minSumParallelMainDiagonal(rows, cols, matrix);
        out << res1 << " " << res2 << "\n";
      } catch (...) {
        if (is_dynamic) {
          delete[] matrix;
        }
        throw;
      }

      if (is_dynamic) {
        delete[] matrix;
      }
      is_first = false;
    }
  }

}

int main(int argc, char** argv)
{
  using namespace kozinskiy;

  if (argc > 4) {
    std::cerr << "Error: too many arguments";
    return invalid_argument_ec;
  }
  if (argc < 4) {
    std::cerr << "Error: not enough arguments";
    return invalid_argument_ec;
  }

  int mode = 0;
  if (!parseNum(argv[1], mode) || (mode != 1 && mode != 2)) {
    std::cerr << "Error: first parameter is out of range or not a number";
    return invalid_argument_ec;
  }

  bool is_dynamic = false;
  if (mode == 2) {
    is_dynamic = true;
  }

  std::ifstream in_file(argv[2]);
  if (!in_file) {
    std::cerr << "Error: incorrect input file";
    return invalid_argument_ec;
  }

  std::ofstream out_file(argv[3]);
  if (!out_file) {
    std::cerr << "Error: incorrect output file";
    return invalid_argument_ec;
  }

  try {
    processMatrices(in_file, out_file, is_dynamic);
  } catch (const std::overflow_error& e) {
    std::cerr << "Error: " << e.what();
    return invalid_file_ec;
  } catch (const std::underflow_error& e) {
    std::cerr << "Error: " << e.what();
    return invalid_file_ec;
  } catch (const std::runtime_error& e) {
    std::cerr << e.what();
    return invalid_file_ec;
  } catch (const std::bad_alloc& e) {
    std::cerr << "Error: " << e.what();
    return invalid_file_ec;
  }

  return good_ec;
}
