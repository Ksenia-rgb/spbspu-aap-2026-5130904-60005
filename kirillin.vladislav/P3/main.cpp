#include <iostream>
#include <fstream>
#include <cstddef>
#include <limits>
#include <new>

namespace kirillin
{
  const int ret_ok = 0;
  const int ret_args_error = 1;
  const int ret_data_error = 2;
  const int mode_fixed = 1;
  const int mode_dynamic = 2;

  int min(int a, int b)
  {
    if (a < b)
    {
      return a;
    }
    else
    {
      return b;
    }
  }

  bool parseMode(const char *arg, int &mode)
  {
    if (arg == nullptr || arg[0] == '\0' || arg[1] != '\0')
    {
      return false;
    }

    if (arg[0] == '1')
    {
      mode = mode_fixed;
      return true;
    }
    if (arg[0] == '2')
    {
      mode = mode_dynamic;
      return true;
    }
    return false;
  }

  bool isNumber(const char *str)
  {
    if (str == nullptr || str[0] == '\0')
    {
      return false;
    }

    for (int i = 0; str[i] != '\0'; ++i)
    {
      if (str[i] < '0' || str[i] > '9')
      {
        return false;
      }
    }
    return true;
  }

  void process_variant5(int *mat, int rows, int cols)
  {
    if (rows <= 0 || cols <= 0)
    {
      return;
    }

    int top = 0;
    int bottom = rows - 1;
    int left = 0;
    int right = cols - 1;
    int step = 1;

    while (top <= bottom && left <= right)
    {
      for (int i = left; i <= right; ++i)
      {
        mat[top * cols + i] -= step++;
      }
      top++;
      for (int i = top; i <= bottom; ++i)
      {
        mat[i * cols + right] -= step++;
      }
      right--;
      if (top <= bottom)
      {
        for (int i = right; i >= left; --i)
        {
          mat[bottom * cols + i] -= step++;
        }
        bottom--;
      }
      if (left <= right)
      {
        for (int i = bottom; i >= top; --i)
        {
          mat[i * cols + left] -= step++;
        }
        left++;
      }
    }
  }

  void process_variant8(int *mat, int rows, int cols)
  {
    if (rows <= 0 || cols <= 0)
    {
      return;
    }

    int layers = (min(rows, cols) + 1) / 2;
    for (int L = 0; L < layers; ++L)
    {
      int step = L + 1;
      for (int i = L; i < cols - L; ++i)
      {
        mat[L * cols + i] += step;
      }
      for (int i = L + 1; i < rows - L; ++i)
      {
        mat[i * cols + (cols - 1 - L)] += step;
      }
      if (rows - 1 - L > L)
      {
        for (int i = cols - 2 - L; i >= L; --i)
        {
          mat[(rows - 1 - L) * cols + i] += step;
        }
      }
      if (cols - 1 - L > L)
      {
        for (int i = rows - 2 - L; i > L; --i)
        {
          mat[i * cols + L] += step;
        }
      }
    }
  }

  void print_matrix(std::ostream &output, const int *mat, int rows, int cols)
  {
    output << rows << " " << cols;
    for (int i = 0; i < rows * cols; ++i)
    {
      output << " " << mat[i];
    }
    output << "\n";
  }

  bool read_and_process(std::istream &input, std::ostream &output, int mode)
  {
    int rows = 0, cols = 0;
    if (!(input >> rows >> cols))
    {
      return false;
    }

    if (rows < 0 || cols < 0)
    {
      return false;
    }

    long long total_ll = static_cast< long long >(rows) * cols;
    if (total_ll > std::numeric_limits< int >::max())
    {
      return false;
    }
    if (mode == mode_fixed && total_ll > 10000)
    {
      return false;
    }

    int total = static_cast< int >(total_ll);

    int *mat_original = nullptr;
    int *mat5 = nullptr;
    int *mat8 = nullptr;
    int mat_stack[10000];

    try
    {
      if (mode == mode_fixed)
      {
        mat_original = &mat_stack[0];
      }
      else
      {
        mat_original = new int[total];
      }

      for (int i = 0; i < total; ++i)
      {
        if (!(input >> mat_original[i]))
        {
          if (mode == mode_dynamic)
          {
            delete[] mat_original;
          }
          return false;
        }
      }

      mat5 = new int[total];
      mat8 = new int[total];
      for (int i = 0; i < total; ++i)
      {
        mat5[i] = mat_original[i];
        mat8[i] = mat_original[i];
      }

      process_variant5(mat5, rows, cols);
      process_variant8(mat8, rows, cols);
      output << "Variant 5:\n";
      print_matrix(output, mat5, rows, cols);
      output << "Variant 8:\n";
      print_matrix(output, mat8, rows, cols);
    }
    catch (const std::bad_alloc &)
    {
      if (mode == mode_dynamic && mat_original != nullptr)
      {
        delete[] mat_original;
      }
      if (mat5 != nullptr)
      {
        delete[] mat5;
      }
      if (mat8 != nullptr)
      {
        delete[] mat8;
      }
      return false;
    }

    if (mode == mode_dynamic && mat_original != nullptr)
    {
      delete[] mat_original;
    }
    if (mat5 != nullptr)
    {
      delete[] mat5;
    }
    if (mat8 != nullptr)
    {
      delete[] mat8;
    }

    return true;
  }

}

int main(int argc, char **argv)
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments" << std::endl;
    return kirillin::ret_args_error;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments" << std::endl;
    return kirillin::ret_args_error;
  }

  int mode = 0;
  if (!kirillin::parseMode(argv[1], mode))
  {
    if (kirillin::isNumber(argv[1]))
    {
      std::cerr << "First parameter is out of range" << std::endl;
    }
    else
    {
      std::cerr << "First parameter is not a number" << std::endl;
    }
    return kirillin::ret_args_error;
  }

  std::ifstream input(argv[2]);
  if (!input)
  {
    std::cerr << "Cannot open input file" << std::endl;
    return kirillin::ret_args_error;
  }

  std::ofstream output(argv[3]);
  if (!output)
  {
    std::cerr << "Cannot open output file" << std::endl;
    return kirillin::ret_args_error;
  }

  if (!kirillin::read_and_process(input, output, mode))
  {
    std::cerr << "Invalid input or memory allocation failed" << std::endl;
    return kirillin::ret_data_error;
  }

  return kirillin::ret_ok;
}
