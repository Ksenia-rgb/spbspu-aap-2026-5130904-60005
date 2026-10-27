#include <cstddef>
#include <fstream>
#include <iostream>
#include <istream>
#include <limits>
#include <new>
#include <ostream>

namespace tsulyauf {

  const std::size_t max_fixed_size = 10000;

  const int fixed_array_task = 1;

  const int args_count = 4;
  const std::size_t task_arg_index = 1;
  const std::size_t input_arg_index = 2;
  const std::size_t output_arg_index = 3;

  const int invalid_args_code = 1;
  const int internal_error_code = 2;

  const int first_step = 1;
  const std::size_t second_row = 1;

  void transformMatrix(int* data, const std::size_t rows, const std::size_t columns)
  {
    if (rows == 0 || columns == 0) {
      return;
    }

    long long top = 0, left = 0;
    long long bottom = static_cast< long long >(rows) - 1;
    long long right = static_cast< long long >(columns) - 1;
    const long long width = static_cast< long long >(columns);
    int k = first_step;

    while (bottom >= top && right >= left) {
      for (long long i = left; i <= right; ++i) {
        data[bottom * width + i] += k++;
      }
      --bottom;

      for (long long i = bottom; i >= top; --i) {
        data[i * width + right] += k++;
      }
      --right;

      if (top <= bottom) {
        for (long long i = right; i >= left; --i) {
          data[top * width + i] += k++;
        }
        ++top;
      }

      if (left <= right) {
        for (long long i = top; i <= bottom; ++i) {
          data[i * width + left] += k++;
        }
        ++left;
      }
    }
  }

  std::size_t countColumnsNsm(const int* data, const std::size_t rows, const std::size_t columns)
  {
    if (rows == 0 || columns == 0) {
      return 0;
    }
    std::size_t count = 0;

    for (std::size_t j = 0; j < columns; ++j) {
      bool has_repeat = false;

      for (std::size_t i = second_row; i < rows && !has_repeat; ++i) {
        has_repeat = (data[i * columns + j] == data[(i - 1) * columns + j]);
      }

      if (!has_repeat) {
        ++count;
      }
    }
    return count;
  }

  bool readSize(std::istream& input, std::size_t& rows, std::size_t& columns)
  {
    long long rows_in = 0, columns_in = 0;
    if (!(input >> rows_in >> columns_in) || rows_in < 0 || columns_in < 0) {
      return false;
    }

    rows = static_cast< std::size_t >(rows_in);
    columns = static_cast< std::size_t >(columns_in);

    return true;
  }

  bool readElements(std::istream& input, int* data, const std::size_t count)
  {
    for (std::size_t i = 0; i < count; ++i) {
      if (!(input >> data[i])) {
        return false;
      }
    }
    return true;
  }

  void printMatrix(std::ostream& output, const int* data, const std::size_t rows, const std::size_t columns)
  {
    output << rows << " " << columns;

    for (std::size_t i = 0; i < rows; ++i) {
      for (std::size_t j = 0; j < columns; ++j) {
        output << " " << data[i * columns + j];
      }
    }
    output << "\n";
  }

  bool parseTaskNum(const char* arg, int& num)
  {
    if (arg[0] == '\0' || arg[1] != '\0') {
      return false;
    }
    if (arg[0] == '1' || arg[0] == '2') {
      num = arg[0] - '0';
      return true;
    }
    return false;
  }

  int process(int* data, const std::size_t rows, const std::size_t columns, std::istream& input, std::ostream& output)
  {
    if (!readElements(input, data, rows * columns)) {
      std::cerr << "Ошибка чтения элементов матрицы\n";
      return internal_error_code;
    }

    const std::size_t nsm = countColumnsNsm(data, rows, columns);

    transformMatrix(data, rows, columns);
    printMatrix(output, data, rows, columns);

    output << nsm << "\n";
    return 0;
  }
}

int main(int argc, char** argv)
{
  if (argc != tsulyauf::args_count) {
    std::cerr << "Ошибка: Неверное количество параметров функции\n";
    return tsulyauf::invalid_args_code;
  }

  int num = 0;
  if (!tsulyauf::parseTaskNum(argv[tsulyauf::task_arg_index], num)) {
    std::cerr << "Ошибка: Неверный первый параметр\n";
    return tsulyauf::invalid_args_code;
  }

  std::ifstream input(argv[tsulyauf::input_arg_index]);
  if (!input) {
    std::cerr << "Ошибка открытия входного файла\n";
    return tsulyauf::internal_error_code;
  }

  std::size_t rows = 0, columns = 0;
  if (!tsulyauf::readSize(input, rows, columns)) {
    std::cerr << "Ошибка чтения размеров матрицы\n";
    return tsulyauf::internal_error_code;
  }

  if (columns != 0 && rows > (std::numeric_limits< std::size_t >::max() / columns)) {
    std::cerr << "Ошибка: слишком большие размеры матрицы\n";
    return tsulyauf::internal_error_code;
  }

  const std::size_t count = rows * columns;

  std::ofstream output(argv[tsulyauf::output_arg_index]);
  if (!output) {
    std::cerr << "Ошибка открытия выходного файла\n";
    return tsulyauf::internal_error_code;
  }

  if (num == tsulyauf::fixed_array_task) {
    if (count > tsulyauf::max_fixed_size) {
      std::cerr << "Ошибка: матрица не помещается в массив фиксированного размера\n";
      return tsulyauf::internal_error_code;
    }

    int data[tsulyauf::max_fixed_size] = {};
    return tsulyauf::process(data, rows, columns, input, output);
  }

  int* data = nullptr;

  try {
    data = new int[count];
  } catch (const std::bad_alloc&) {
    std::cerr << "Ошибка выделения памяти\n";
    return tsulyauf::internal_error_code;
  }

  const int result = tsulyauf::process(data, rows, columns, input, output);
  delete[] data;
  return result;
}
