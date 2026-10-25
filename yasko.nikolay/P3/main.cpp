#include <iostream>
#include <fstream>
#include <stdexcept>
#include <limits>
#include <cstddef>

const long long min_long_long = std::numeric_limits< long long >::min();
const long long max_long_long = std::numeric_limits< long long >::max();
const size_t max_size_t = std::numeric_limits< size_t >::max();
const size_t max_elements = 10000;
namespace yasko
{
  bool sum_overflow_int(long long a, long long b)
  {
    if (b > 0 && a > max_long_long - b)
    {
      return true;
    }
    if (b < 0 && a < min_long_long - b)
    {
      return true;
    }
    return false;
  }
  bool overflow_multiply(size_t a, size_t b)
  {
    if (b != 0 && a > max_size_t / b)
    {
      return true;
    }
    return false;
  }
  bool check(int argc, char **argv, int &mode)
  {
    if (argc > 4)
    {
      std::cerr << "Too many arguments\n";
      return false;
    }
    if (argc < 4)
    {
      std::cerr << "Not enough arguments\n";
      return false;
    }
    const char *first = argv[1];
    if ((first[0] == '1' || first[0] == '2') && first[1] == '\0')
    {
      if (first[0] == '1')
      {
        mode = 1;
      }
      else
      {
        mode = 2;
      }
      return true;
    }
    bool digits = (first[0] != '\0');
    for (size_t i = 0; first[i] != '\0'; i++)
    {
      if (first[i] < '0' || first[i] > '9')
      {
        digits = false;
      }
    }
    if (digits)
    {
      std::cerr << "First parameter is out of range\n";
      return false;
    }
    else
    {
      std::cerr << "First parameter is not a number\n";
      return false;
    }
  }
  void read_size(std::istream &input, size_t &row, size_t &cols)
  {
    long long rows_i = 0;
    long long cols_i = 0;
    if (!(input >> rows_i >> cols_i) || rows_i < 0 || cols_i < 0)
    {
      throw std::invalid_argument("");
    }
    row = static_cast< size_t >(rows_i);
    cols = static_cast< size_t >(cols_i);
  }
  void matrix_input(int *arr, size_t rows, size_t cols, std::istream &input)
  {
    const size_t len = rows * cols;
    for (size_t i = 0; i < len; i++)
    {
      if (!(input >> arr[i]))
      {
        throw std::invalid_argument("");
      }
    }
  }
  size_t matr_to_sq(int *arr, size_t rows, size_t cols)
  {
    const size_t side = rows < cols ? rows : cols;
    for (size_t i = 0; i < side; i++)
    {
      for (size_t j = 0; j < side; j++)
      {
        arr[i * side + j] = arr[i * cols + j];
      }
    }
    return side;
  }
  bool up_triangle_matr(int *arr, size_t side)
  {
    if (side == 0)
    {
      return false;
    }
    bool flag = true;
    for (size_t i = 0; i < side; i++)
    {
      for (size_t j = 0; j < i; j++)
      {
        if (arr[i * side + j] != 0)
        {
          flag = false;
          break;
        }
      }
      if (!flag)
      {
        break;
      }
    }
    return flag;
  }
  long long max_sum_diag(int *arr, size_t side)
  {
    long long max_sum = min_long_long;
    if (side == 0)
    {
      return 0;
    }
    else if (side == 1)
    {
      return 0;
    }
    for (size_t d = 1; d < side; d++)
    {
      long long up = 0;
      long long down = 0;
      for (size_t i = 0; d + i < side; i++)
      {
        if (sum_overflow_int(up, arr[i * side + i + d]) || sum_overflow_int(down, arr[(i + d) * side + i]))
        {
          throw std::overflow_error("");
        }
        up += arr[i * side + i + d];
        down += arr[(i + d) * side + i];
      }
      if (up > max_sum)
      {
        max_sum = up;
      }
      if (down > max_sum)
      {
        max_sum = down;
      }
    }
    return max_sum;
  }
  bool result(int *arr, size_t side, std::ostream &output)
  {
    if (up_triangle_matr(arr, side))
    {
      output << "[UPP-TRI-MTX] = true\n";
    }
    else
    {
      output << "[UPP-TRI-MTX] = false\n";
    }
    try
    {
      output << "[MAX-SUM-SDG] = " << max_sum_diag(arr, side) << "\n";
    }
    catch (const std::overflow_error &e)
    {
      std::cerr << "Overflow error\n";
      return false;
    }
    return true;
  }
}
{
  const int input_arguments_ex1 = 1;
  const int input_values_ex2 = 2;
  const int other_errors_ex3 = 3;
  int matrix_stack[max_elements] = {};
  int *matrix_free_store = nullptr;
  int mode = 0;
  if (!yasko::check(argc, argv, mode))
  {
    return input_arguments_ex1;
  }
  const char *inputPath = argv[2];
  const char *outputPath = argv[3];
  std::ifstream input{inputPath};
  if (!input)
  {
    std::cerr << "Cannot open input file\n";
    return input_arguments_ex1;
  }
  std::ofstream output{outputPath};
  if (!output)
  {
    std::cerr << "Cannot open output file\n";
    return input_arguments_ex1;
  }
  size_t rows = 0;
  size_t cols = 0;
  bool flag = true;
  try
  {
    yasko::read_size(input, rows, cols);
    if (yasko::overflow_multiply(rows, cols))
    {
      throw std::overflow_error("");
    }
    size_t side = 0;
    if (mode == 1)
    {
      if (rows * cols > max_elements)
      {
        throw std::invalid_argument("Too many elements");
      }
      yasko::matrix_input(matrix_stack, rows, cols, input);
      side = yasko::matr_to_sq(matrix_stack, rows, cols);
      flag = yasko::result(matrix_stack, side, output);
    }
    else
    {
      matrix_free_store = new int[rows * cols];
      yasko::matrix_input(matrix_free_store, rows, cols, input);

      side = yasko::matr_to_sq(matrix_free_store, rows, cols);
      flag = yasko::result(matrix_free_store, side, output);
    }
  }
  catch (const std::invalid_argument &e)
  {
    std::cerr << "Invalid input\n";
    delete[] matrix_free_store;
    return input_values_ex2;
  }
  catch (const std::bad_alloc &e)
  {
    std::cerr << "Bad alloc\n";
    return other_errors_ex3;
  }
  catch (std::overflow_error &e)
  {
    std::cerr << "Overflow error\n";
    delete[] matrix_free_store;
    return other_errors_ex3;
  }
  delete[] matrix_free_store;
  if (!flag)
  {
    return other_errors_ex3;
  }
  return 0;
}
