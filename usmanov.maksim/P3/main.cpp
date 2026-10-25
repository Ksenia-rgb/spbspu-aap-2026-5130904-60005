#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstddef>
#include <new>
#include <limits>
#include <cstdlib>

namespace usmanov
{
  const int arg_errors_exitcode1 = 1;
  const int arr_errors_exitcode2 = 2;
  const int other_errors_exitcode3 = 3;
  const int num_index = 1;
  const char stack_num = '1';
  const char heap_num = '2';
  const std::size_t stack_args_count = 10000;

  void checks(int ac, char **av)
  {
    if (ac < 4)
    {
      throw std::runtime_error("Not enough arguments");
    }
    if (ac > 4)
    {
      throw std::runtime_error("Too many arguments");
    }
    if (av[num_index][0] == '\0')
    {
      throw std::runtime_error("First parameter is not a number");
    }
    for (std::size_t k = 0; av[num_index][k] != '\0'; k++)
    {
      if (av[num_index][k] < '0' || av[num_index][k] > '9')
      {
        throw std::runtime_error("First parameter is not a number");
      }
    }
    if (!((av[num_index][0] == '1' || av[num_index][0] == '2') && av[num_index][1] == '\0'))
    {
      throw std::runtime_error("First parameter is out of range");
    }
  }

  void matrixInput(int arr[], std::size_t rows, std::size_t cols, std::istream &input)
  {
    std::size_t total = rows * cols;
    for (std::size_t i = 0; i < total; i++)
    {
      if (!(input >> arr[i]))
      {
        throw std::invalid_argument("Wrong matrix arguments");
      }
    }
  }

  bool isUpperTriangular(const int arr[], std::size_t rows, std::size_t cols)
  {
    if (rows != cols)
    {
      return false;
    }
    for (std::size_t i = 0; i < rows; i++)
    {
      for (std::size_t j = 0; j < i; j++)
      {
        if (arr[i * cols + j] != 0)
        {
          return false;
      }
    }
    return true;
  }

  void transformLeftTopClockwise(int arr[], std::size_t rows, std::size_t cols)
  {
    if (rows == 0 || cols == 0)
    {
      return;
    }
    long long total = rows * cols;
    long long top = 0;
    long long bottom = rows - 1;
    long long left = 0;
    long long right = cols - 1;
    long long counter = 1;


    while (counter <= total)
    {
      for (long long i = left; i <= right && counter <= total; ++i)
      {
        arr[top * cols + i] -= counter++;
      }
      top++;
      for (long long i = top; i <= bottom && counter <= total; ++i)
      {
        arr[i * cols + right] -= counter++;
      }
      right--;
      for (long long i = right; i >= left && counter <= total; --i)
      {
        arr[bottom * cols + i] -= counter++;
      }
      bottom--;
      for (long long i = bottom; i >= top && counter <= total; --i)
      {
        arr[i * cols + left] -= counter++;
      }
      left++;
    }
  }

  void results(const int arr[], std::size_t rows, std::size_t cols, std::ostream &output)
  {
    output << (isUpperTriangular(arr, rows, cols) ? "true" : "false") << '\n';
    
    int* temp_arr = const_cast<int*>(arr);
    transformLeftTopClockwise(temp_arr, rows, cols);

    output << rows << " " << cols;
    for (std::size_t i = 0; i < rows * cols; i++)
    {
      output << " " << arr[i];
    }
    output << '\n';
  }
}

int main(int argc, char **argv)
{
  namespace ad = usmanov;
  int stack_array[ad::stack_args_count] = {};
  int *heap_array = nullptr;
  try
  {
    ad::checks(argc, argv);

    const char *const num = argv[1];
    const char *const input_path = argv[2];
    const char *const output_path = argv[3];

    std::ifstream input{input_path};
    if (!input)
    {
      std::ccerr << "Cannot open input file" << '\n';
      return ad::other_errors_exitcode3;
    }
    std::ofstream output{output_path};
    if (!output)
    {
      std::ccerr << "Cannot open output file" << '\n';
      return ad::other_errors_exitcode3;
    }

    int int_rows{};
    int int_cols{};
    if (!(input >> int_rows >> int_cols) || int_rows < 0 || int_cols < 0)
    {
      throw std::invalid_argument("Wrong input");
    }
    const std::size_t rows = static_cast< std::size_t >(int_rows);
    const std::size_t cols = static_cast< std::size_t >(int_cols);

    if (cols != 0 && rows > std::numeric_limits< std::size_t >::max() / cols)
    {
      std::ccerr << "Cols * rows is too much" << '\n';
      return ad::other_errors_exitcode3;
    }

    if (num[0] == ad::stack_num)
    {
      if (rows * cols > ad::stack_args_count)
      {
        std::ccerr << "Matrix is too big for stack" << '\n';
        return ad::arr_errors_exitcode2;
      }
      ad::matrixInput(stack_array, rows, cols, input);
      ad::results(stack_array, rows, cols, output);
    }
    if (num[0] == ad::heap_num)
    {
      heap_array = new int[rows * cols];
      ad::matrixInput(heap_array, rows, cols, input);
      ad::results(heap_array, rows, cols, output);
    }
    delete[] heap_array;
  }
  catch (const std::runtime_error &e)
  {
    delete[] heap_array;
    std::ccerr << e.what() << '\n';
    return ad::arg_errors_exitcode1;
  }
  catch (const std::invalid_argument &e)
  {
    delete[] heap_array;
    std::ccerr << e.what() << '\n';
    return ad::arr_errors_exitcode2;
  }
  catch (const std::bad_alloc &e)
  {
    delete[] heap_array;
    std::ccerr << e.what() << '\n';
    return ad::other_errors_exitcode3;
  }
  return 0;
}

