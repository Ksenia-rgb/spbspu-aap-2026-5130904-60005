#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <new>
#include <cstddef>
#include <stdexcept>

namespace komova {
  const size_t max_size_t = std::numeric_limits< std::size_t >::max();
  size_t min(size_t x, size_t y)
  {
    if (x < y) {
      return x;
    }
    return y;
  }
  int max(int x, int y)
  {
    if (x > y) {
      return x;
    }
    return y;
  }
  void readMatrix(int* arr, const size_t rows, const size_t cols, std::istream& input)
  {
    for (size_t i = 0; i < rows * cols; i++) {
      if (!(input >> arr[i])) {
        throw std::invalid_argument("");
      }
    }
  }
  void maxSumSdg(const int* arr, const size_t rows, const size_t cols, std::ostream& output)
  {
    int mxsum = 0;
    for (long long i = -static_cast< long long >(rows) + 1; i < static_cast< long long >(cols); i++) {
      int sum = 0;
      for (size_t j = 0; j < rows; j++) {
        const long long col = static_cast< long long >(j) + i;
        if (i != 0 and col >= 0 and col < static_cast< long long >(cols)) {
          sum += arr[j * cols + col];
        }
      }
      mxsum = max(sum, mxsum);
    }
    output << mxsum << std::endl;
  }
  void lwrTriMtx(const int* arr, const size_t rows, const size_t cols, std::ostream& output)
  {
    bool flag = true;
    const size_t kv_razm = min(rows, cols);
    if (kv_razm > 0) {
      for (size_t i = 0; i < kv_razm - 1; i++) {
        for (size_t j = i + 1; j < kv_razm; j++) {
          if (arr[i * kv_razm + j] != 0) {
            flag = false;
          }
          if (!flag) {
            break;
          }
        }
        if (!flag) {
          break;
        }
      }
    }
    if (flag and kv_razm != 0) {
      output << "true" << std::endl;
    } else {
      output << "false" << std::endl;
    }
  }
}
int main(int argc, char** argv)
{
  const int first_index = 1;
  const int input_index = 2;
  const int output_index = 3;
  const int count_arg = 4;
  const int invalid_arguments = 1;
  const int invalid_input_or_output = 2;
  const size_t max_size_static_matrix = 10000;
  long long rows_signed = 0;
  long long cols_signed = 0;
  if (argc != count_arg) {
    std::cerr << "Неверное количество параметров" << std::endl;
    return invalid_arguments;
  }
  if (std::string(argv[first_index]) != "2" and std::string(argv[first_index]) != "1") {
    std::cerr << "Неверный первый параметр" << std::endl;
    return invalid_arguments;
  }
  std::ifstream input{argv[input_index]};
  if (!input) {
    std::cerr << "Ошибка открытия входного файла" << std::endl;
    return invalid_input_or_output;
  }
  if (!(input >> rows_signed >> cols_signed) or rows_signed < 0 or cols_signed < 0) {
    std::cerr << "Неверные размеры матрицы" << std::endl;
    return invalid_input_or_output;
  }
  const size_t rows = static_cast< std::size_t >(rows_signed);
  const size_t cols = static_cast< std::size_t >(cols_signed);
  if (rows != 0 and komova::max_size_t / rows < cols) {
    std::cerr << "Слишком большие размеры матрицы" << std::endl;
    return invalid_input_or_output;
  }
  std::ofstream output{argv[output_index]};
  if (!output) {
    std::cerr << "Ошибка открытия выходного файла" << std::endl;
    return invalid_input_or_output;
  }
  if (std::string(argv[first_index]) == "1") {
    int matrix[max_size_static_matrix];
    try {
      komova::readMatrix(matrix, rows, cols, input);
    } catch (std::invalid_argument& e) {
      std::cerr << "Ошибка чтения элементов матрицы" << std::endl;
      return invalid_input_or_output;
    }
    komova::maxSumSdg(matrix, rows, cols, output);
    komova::lwrTriMtx(matrix, rows, cols, output);
  }
  if (std::string(argv[first_index]) == "2") {
    int* matrix = nullptr;
    try {
      matrix = new int[rows * cols];
      komova::readMatrix(matrix, rows, cols, input);

    } catch (std::bad_alloc& e) {
      delete[] matrix;
      std::cerr << "Ошибка выделения памяти" << std::endl;
      return invalid_input_or_output;
    } catch (std::invalid_argument& e) {
      delete[] matrix;
      std::cerr << "Ошибка чтения элементов матрицы" << std::endl;
      return invalid_input_or_output;
    }
    komova::maxSumSdg(matrix, rows, cols, output);
    komova::lwrTriMtx(matrix, rows, cols, output);
    delete[] matrix;
  }
  return 0;
}
