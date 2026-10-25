#include <fstream>
#include <iostream>
#include <new>
#include <limits>

namespace cheberiak {

  void smoothMatrix(const int *matrix, double *result, int rows, int cols)
  {
    if (rows == 1 && cols == 1) {
      result[0] = matrix[0];
      return;
    }
    double summ = 0;
    int count = 0;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        for (int newRow = i - 1; newRow <= i + 1; newRow++) {
          for (int newCol = j - 1; newCol <= j + 1; newCol++) {
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && !(newRow == i && newCol == j)) {
              summ += matrix[newRow * cols + newCol];
              count++;
            }
          }
        }
        result[i * cols + j] = summ / count;
        summ = 0, count = 0;
      }
    }
  }

  int countNonZeroDiagonals(const int *matrix, int rows, int cols)
  {
    int count = 0;
    for (int j = 1; j < cols; j++) {
      int newRow = 0;
      int newCol = j;
      bool flag = true;
      while (newRow < rows && newCol < cols) {
        if (matrix[newRow * cols + newCol] == 0) {
          flag = false;
        }
        newRow++;
        newCol++;
      }
      if (flag) {
        count++;
      }
    }
    for (int i = 1; i < rows; i++) {
      int newRow = i;
      int newCol = 0;
      bool flag = true;
      while (newRow < rows && newCol < cols) {
        if (matrix[newRow * cols + newCol] == 0) {
          flag = false;
        }
        newRow++;
        newCol++;
      }
      if (flag) {
        count++;
      }
    }
    return count;
  }

}

int main(int argc, char *argv[])
{
  const int SUCCESS = 0;
  const int INVALID_ARGUMENTS = 1;
  const int INVALID_INPUT = 2;
  const int OTHER_ERROR = 3;

  if (argc != 4) {
    std::cerr << "Wrong arguments\n";
    return INVALID_ARGUMENTS;
  }
  if ((argv[1][0] != '1' && argv[1][0] != '2') || argv[1][1] != '\0') {
    std::cerr << "Invalid mode: expected 1 or 2\n";
    return INVALID_ARGUMENTS;
  }

  std::ifstream input(argv[2]);
  if (input.fail()) {
    std::cerr << "Cannot open file\n" << std::endl;
    return INVALID_INPUT;
  }
  int rows = 0, cols = 0, count = 0;
  input >> rows >> cols;
  if (input.fail() || rows < 0 || cols < 0 || (rows == 0 && cols != 0) || (rows != 0 && cols == 0)
      || (rows != 0 && cols > std::numeric_limits< int >::max() / rows)) {
    std::cerr << "Invalid matrix dimensions\n" << std::endl;
    return INVALID_INPUT;
  }

  int matrix_for1[10000];
  double result_for1[10000];
  int *matrix = nullptr;
  double *result = nullptr;

  if (argv[1][0] == '1') {
    matrix = matrix_for1;
    result = result_for1;
  } else if (argv[1][0] == '2') {
    try {
      matrix = new int[rows * cols];
    } catch (const std::bad_alloc &) {
      std::cerr << "Memory allocation failed\n" << std::endl;
      return OTHER_ERROR;
    }
    try {
      result = new double[rows * cols];
    } catch (const std::bad_alloc &) {
      std::cerr << "Memory allocation failed\n" << std::endl;
      delete[] matrix;
      return OTHER_ERROR;
    }
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      input >> matrix[i * cols + j];
    }
  }
  if (input.fail()) {
    std::cerr << "Failed to read matrix element\n" << std::endl;
    if (argv[1][0] == '2') {
      delete[] matrix;
      delete[] result;
    }
    return INVALID_INPUT;
  }

  cheberiak::smoothMatrix(matrix, result, rows, cols);
  count = cheberiak::countNonZeroDiagonals(matrix, rows, cols);

  std::ofstream output(argv[3]);
  if (output.fail()) {
    std::cerr << "Cannot open output file\n";
    if (argv[1][0] == '2') {
      delete[] matrix;
      delete[] result;
    }
    return OTHER_ERROR;
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
    if (argv[1][0] == '2') {
      delete[] matrix;
      delete[] result;
    }
    return OTHER_ERROR;
  }
  if (argv[1][0] == '2') {
    delete[] matrix;
    delete[] result;
  }
  return SUCCESS;
}
