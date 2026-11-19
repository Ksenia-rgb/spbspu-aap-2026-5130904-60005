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
  const int max_capacity = 10000;

  int min(int a, int b)
  {
    if (a < b)
    {
      return a;
    }
    return b;
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

  bool safeSub(int &val, int step)
  {
    if (val < std::numeric_limits< int >::min() + step)
    {
      return false;
    }
    val -= step;
    return true;
  }

  bool safeAdd(int &val, int step)
  {
    if (val > std::numeric_limits< int >::max() - step)
    {
      return false;
    }
    val += step;
    return true;
  }

  bool processVariant5(int *mat, int rows, int cols)
  {
    if (rows <= 0 || cols <= 0)
    {
      return true;
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
        if (!safeSub(mat[top * cols + i], step))
        {
          return false;
        }
        step++;
      }
      top++;
      for (int i = top; i <= bottom; ++i)
      {
        if (!safeSub(mat[i * cols + right], step))
        {
          return false;
        }
        step++;
      }
      right--;
      if (top <= bottom)
      {
        for (int i = right; i >= left; --i)
        {
          if (!safeSub(mat[bottom * cols + i], step))
          {
            return false;
          }
          step++;
        }
        bottom--;
      }
      if (left <= right)
      {
        for (int i = bottom; i >= top; --i)
        {
          if (!safeSub(mat[i * cols + left], step))
          {
            return false;
          }
          step++;
        }
        left++;
      }
    }
    return true;
  }

  bool processVariant8(int *mat, int rows, int cols)
  {
    if (rows <= 0 || cols <= 0)
    {
      return true;
    }
    const int smaller = min(rows, cols);
    const int layers = smaller / 2 + smaller % 2;
    for (int l = 0; l < layers; ++l)
    {
      const int step = l + 1;
      for (int i = l; i < cols - l; ++i)
      {
        if (!safeAdd(mat[l * cols + i], step))
        {
          return false;
        }
      }
      for (int i = l + 1; i < rows - l; ++i)
      {
        if (!safeAdd(mat[i * cols + (cols - 1 - l)], step))
        {
          return false;
        }
      }
      if (rows - 1 - l > l)
      {
        for (int i = cols - 2 - l; i >= l; --i)
        {
          if (!safeAdd(mat[(rows - 1 - l) * cols + i], step))
          {
            return false;
          }
        }
      }
      if (cols - 1 - l > l)
      {
        for (int i = rows - 2 - l; i > l; --i)
        {
          if (!safeAdd(mat[i * cols + l], step))
          {
            return false;
          }
        }
      }
    }
    return true;
  }

  void printMatrix(std::ostream &output, const int *mat, int rows, int cols, int total)
  {
    output << rows << " " << cols;
    for (int i = 0; i < total; ++i)
    {
      output << " " << mat[i];
    }
    output << "\n";
  }

  bool safeTotal(int rows, int cols, long long &total_ll)
  {
    if (rows < 0 || cols < 0)
    {
      return false;
    }
    total_ll = static_cast< long long >(rows) * cols;
    if (total_ll >= std::numeric_limits< int >::max())
    {
      return false;
    }
    return true;
  }

  bool processFixed(std::istream &input, std::ostream &output, int rows, int cols, int total)
  {
    if (total > max_capacity)
    {
      return false;
    }
    int mat_stack[max_capacity];
    int mat5_stack[max_capacity];
    int mat8_stack[max_capacity];
    for (int i = 0; i < total; ++i)
    {
      if (!(input >> mat_stack[i]))
      {
        return false;
      }
    }
    for (int i = 0; i < total; ++i)
    {
      mat5_stack[i] = mat_stack[i];
      mat8_stack[i] = mat_stack[i];
    }
    if (!processVariant5(mat5_stack, rows, cols))
    {
      return false;
    }
    if (!processVariant8(mat8_stack, rows, cols))
    {
      return false;
    }
    printMatrix(output, mat5_stack, rows, cols, total);
    printMatrix(output, mat8_stack, rows, cols, total);
    return true;
  }

  bool processDynamic(std::istream &input, std::ostream &output, int rows, int cols, int total)
  {
    if (total == 0)
    {
      output << rows << " " << cols << "\n";
      output << rows << " " << cols << "\n";
      return true;
    }
    int *mat_original = nullptr;
    int *mat5 = nullptr;
    int *mat8 = nullptr;
    try
    {
      mat_original = new int[total];
      for (int i = 0; i < total; ++i)
      {
        if (!(input >> mat_original[i]))
        {
          delete[] mat_original;
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
    }
    catch (const std::bad_alloc &)
    {
      delete[] mat_original;
      delete[] mat5;
      delete[] mat8;
      return false;
    }

    if (!processVariant5(mat5, rows, cols))
    {
      delete[] mat_original;
      delete[] mat5;
      delete[] mat8;
      return false;
    }
    if (!processVariant8(mat8, rows, cols))
    {
      delete[] mat_original;
      delete[] mat5;
      delete[] mat8;
      return false;
    }
    printMatrix(output, mat5, rows, cols, total);
    printMatrix(output, mat8, rows, cols, total);

    delete[] mat_original;
    delete[] mat5;
    delete[] mat8;
    return true;
  }

  bool readAndProcess(std::istream &input, std::ostream &output, int mode)
  {
    int rows = 0, cols = 0;
    if (!(input >> rows >> cols))
    {
      return false;
    }
    long long total_ll = 0;
    if (!safeTotal(rows, cols, total_ll))
    {
      return false;
    }
    const int total = static_cast< int >(total_ll);
    if (mode == mode_fixed)
    {
      return processFixed(input, output, rows, cols, total);
    }
    return processDynamic(input, output, rows, cols, total);
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

  if (!kirillin::readAndProcess(input, output, mode))
  {
    std::cerr << "Invalid input, overflow, or memory allocation failed" << std::endl;
    return kirillin::ret_data_error;
  }
  return kirillin::ret_ok;
}
