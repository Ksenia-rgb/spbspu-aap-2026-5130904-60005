#include <fstream>
#include <iostream>
#include <new>
#include <limits>
#include <cstddef>

namespace cheberiak {

  const int success = 0;
  const int invalid_arguments = 1;
  const int invalid_input = 2;

  void smoothMatrix(const int *const matrix, double *const result, size_t rows, size_t cols)
  {
    if (rows == 1 && cols == 1) {
      result[0] = matrix[0];
      return;
    }

    double summ = 0;
    size_t count = 0;

    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        for (int new_row = static_cast< int >(i) - 1; new_row <= static_cast< int >(i) + 1; new_row++) {
          for (int new_col = static_cast< int >(j) - 1; new_col <= static_cast< int >(j) + 1; new_col++) {
            if (new_row >= 0 && new_row < static_cast< int >(rows) && new_col >= 0 && new_col < static_cast< int >(cols)
                && !(new_row == static_cast< int >(i) && new_col == static_cast< int >(j))) {
              summ += matrix[static_cast< size_t >(new_row) * cols + static_cast< size_t >(new_col)];
              count++;
            }
          }
        }

        result[i * cols + j] = summ / count;
        summ = 0;
        count = 0;
      }
    }
  }

  size_t countNonZeroDiagonals(const int *const matrix, size_t rows, size_t cols)
  {
    size_t count = 0;

    for (size_t j = 1; j < cols; j++) {
      size_t new_row = 0;
      size_t new_col = j;
      bool flag = true;

      while (new_row < rows && new_col < cols) {
        if (matrix[new_row * cols + new_col] == 0) {
          flag = false;
        }
        new_row++;
        new_col++;
      }

      if (flag) {
        count++;
      }
    }

    for (size_t i = 1; i < rows; i++) {
      size_t new_row = i;
      size_t new_col = 0;
      bool flag = true;

      while (new_row < rows && new_col < cols) {
        if (matrix[new_row * cols + new_col] == 0) {
          flag = false;
        }
        new_row++;
        new_col++;
      }

      if (flag) {
        count++;
      }
    }

    return count;
  }

  int processMatrix(
      std::ifstream &input, const char *output_file, int *matrix, double *result, size_t rows, size_t cols)
  {
    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        input >> matrix[i * cols + j];
      }
    }

    if (input.fail()) {
      std::cerr << "Failed to read matrix element\n";
      return invalid_input;
    }

    char extra = 0;
    if (input >> extra) {
      std::cerr << "Extra data in input file\n";
      return invalid_input;
    }

    smoothMatrix(matrix, result, rows, cols);
    const size_t count = countNonZeroDiagonals(matrix, rows, cols);

    std::ofstream output(output_file);

    if (output.fail()) {
      std::cerr << "Cannot open output file\n";
      return invalid_input;
    }

    output << rows << ' ' << cols << '\n';
    output << std::fixed;
    output.precision(1);

    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        output << result[i * cols + j] << " ";
      }
      output << '\n';
    }

    output << count << '\n';

    if (output.fail()) {
      std::cerr << "Failed to write output file\n";
      return invalid_input;
    }

    return success;
  }

}

int main(int argc, char *argv[])
{
  constexpr int expected_argc = 4;
  constexpr int input_arg = 2;
  constexpr int output_arg = 3;
  constexpr size_t matrix_capacity = 10000;

  if (argc != expected_argc) {
    std::cerr << "Wrong arguments\n";
    return cheberiak::invalid_arguments;
  }

  if ((argv[1][0] != '1' && argv[1][0] != '2') || argv[1][1] != '\0') {
    std::cerr << "Invalid mode: expected 1 or 2\n";
    return cheberiak::invalid_arguments;
  }

  std::ifstream input(argv[input_arg]);

  if (input.fail()) {
    std::cerr << "Cannot open file\n";
    return cheberiak::invalid_input;
  }

  int rows = 0;
  int cols = 0;

  input >> rows >> cols;

  if (input.fail() || rows < 0 || cols < 0 || (rows == 0 && cols != 0) || (rows != 0 && cols == 0)
      || (rows != 0 && cols > std::numeric_limits< int >::max() / rows)) {
    std::cerr << "Invalid matrix dimensions\n";
    return cheberiak::invalid_input;
  }

  const size_t matrix_rows = static_cast< size_t >(rows);
  const size_t matrix_cols = static_cast< size_t >(cols);

  if (argv[1][0] == '1') {
    if (matrix_rows * matrix_cols > matrix_capacity) {
      std::cerr << "Matrix exceeds fixed capacity\n";
      return cheberiak::invalid_input;
    }

    int matrix[matrix_capacity];
    double result[matrix_capacity];

    return cheberiak::processMatrix(input, argv[output_arg], matrix, result, matrix_rows, matrix_cols);
  }

  if (argv[1][0] == '2') {
    int *matrix = nullptr;
    double *result = nullptr;

    try {
      matrix = new int[matrix_rows * matrix_cols];
    } catch (const std::bad_alloc &) {
      std::cerr << "Memory allocation failed\n";
      return cheberiak::invalid_input;
    }

    try {
      result = new double[matrix_rows * matrix_cols];
    } catch (const std::bad_alloc &) {
      std::cerr << "Memory allocation failed\n";
      delete[] matrix;
      return cheberiak::invalid_input;
    }

    const int code = cheberiak::processMatrix(input, argv[output_arg], matrix, result, matrix_rows, matrix_cols);

    delete[] matrix;
    delete[] result;

    return code;
  }

  return cheberiak::success;
}
