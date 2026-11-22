#include <fstream>
#include <iostream>
#include <new>
#include <limits>

namespace cheberiak {

  const int success = 0;
  const int invalid_arguments = 1;
  const int invalid_input = 2;
  const int other_error = 3;

  void smoothMatrix(const int *const matrix, double *const result, size_t rows, size_t cols)
  {
    if (rows == 1 && cols == 1) {
      result[0] = matrix[0];
      return;
    }
    double summ = 0;
    int count = 0;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        for (int new_row = i - 1; new_row <= i + 1; new_row++) {
          for (int new_col = j - 1; new_col <= j + 1; new_col++) {
            if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols && !(new_row == i && new_col == j)) {
              summ += matrix[new_row * cols + new_col];
              count++;
            }
          }
        }
        result[i * cols + j] = summ / count;
        summ = 0, count = 0;
      }
    }
  }

  size_t countNonZeroDiagonals(const int *matrix, size_t rows, size_t cols)
  {
    int count = 0;
    for (int j = 1; j < cols; j++) {
      int new_row = 0;
      int new_col = j;
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
      int new_row = i;
      int new_col = 0;
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
  int processMatrix(std::ifstream &input, const char *outputFile, int *matrix, double *result, size_t rows, size_t cols)
  {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        input >> matrix[i * cols + j];
      }
    }
    if (input.fail()) {
      std::cerr << "Failed to read matrix element\n";
      return invalid_input;
    }

    char extra;
    if (input >> extra) {
      std::cerr << "Extra data in input file\n";
      return invalid_input;
    }
    size_t count = 0;
    smoothMatrix(matrix, result, rows, cols);
    count = countNonZeroDiagonals(matrix, rows, cols);

    std::ofstream output(outputFile);
    if (output.fail()) {
      std::cerr << "Cannot open output file\n";
      return other_error;
    }

    output << rows << ' ' << cols << '\n';
    output << std::fixed;
    output.precision(1);

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        output << result[i * cols + j] << " ";
      }
      output << '\n';
    }
    output << count << '\n';

    if (output.fail()) {
      std::cerr << "Failed to write output file\n";
      return other_error;
    }
    return success;
  }
}

int main(int argc, char *argv[])
{
  constexpr int expected_argc = 4;
  constexpr int input_arg = 2;
  constexpr int output_arg = 3;
  constexpr int matrix_capacity = 10000;

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
    std::cerr << "Cannot open file\n" << std::endl;
    return cheberiak::invalid_input;
  }
  int rows = 0, cols = 0;
  input >> rows >> cols;
  if (input.fail() || rows < 0 || cols < 0 || (rows == 0 && cols != 0) || (rows != 0 && cols == 0)
      || (rows != 0 && cols > std::numeric_limits< int >::max() / rows)) {
    std::cerr << "Invalid matrix dimensions\n" << std::endl;
    return cheberiak::invalid_input;
  }

  int *matrix = nullptr;
  double *result = nullptr;

  if (argv[1][0] == '1') {
    int matrix_[matrix_capacity];
    double result_[matrix_capacity];
    matrix = matrix_;
    result = result_;
    return cheberiak::processMatrix(input, argv[output_arg], matrix, result, rows, cols);
  }
  if (argv[1][0] == '2') {
    try {
      matrix = new int[rows * cols];
    } catch (const std::bad_alloc &) {
      std::cerr << "Memory allocation failed\n" << std::endl;
      return cheberiak::other_error;
    }
    try {
      result = new double[rows * cols];
    } catch (const std::bad_alloc &) {
      std::cerr << "Memory allocation failed\n" << std::endl;
      delete[] matrix;
      return cheberiak::other_error;
    }
    int code;
    code = cheberiak::processMatrix(input, argv[output_arg], matrix, result, rows, cols);
    delete[] matrix;
    delete[] result;
    return code;
  }
}
