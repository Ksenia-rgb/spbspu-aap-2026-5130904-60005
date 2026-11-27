#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <new>

namespace novikov
{
  const std::size_t fixed_capacity = 10000;
  const int fixed_mode = 1;
  const int dynamic_mode = 2;
  const int expected_arguments = 4;
  const long long unit = 1;
  const std::size_t size_unit = 1;
  const char first_digit = '0';
  const char last_digit = '9';

  const int status_ok = 0;
  const int status_failure = 1;
  const int status_invalid_input = 2;

  static bool safeAdd(long long first, long long second, long long& result)
  {
    const long long max_value = std::numeric_limits< long long >::max();
    const long long min_value = std::numeric_limits< long long >::min();
    if ((second > 0) && (first > (max_value - second)))
    {
      return false;
    }
    if ((second < 0) && (first < (min_value - second)))
    {
      return false;
    }
    result = first + second;
    return true;
  }

  static bool safeAdd(std::size_t first, std::size_t second, std::size_t& result)
  {
    if (first > (std::numeric_limits< std::size_t >::max() - second))
    {
      return false;
    }
    result = first + second;
    return true;
  }

  static bool safeSub(long long first, long long second, long long& result)
  {
    const long long max_value = std::numeric_limits< long long >::max();
    const long long min_value = std::numeric_limits< long long >::min();
    if ((second > 0) && (first < (min_value + second)))
    {
      return false;
    }
    if ((second < 0) && (first > (max_value + second)))
    {
      return false;
    }
    result = first - second;
    return true;
  }

  static bool safeSub(std::size_t first, std::size_t second, std::size_t& result)
  {
    if (first < second)
    {
      return false;
    }
    result = first - second;
    return true;
  }

  static bool safeMul(std::size_t first, std::size_t second, std::size_t& result)
  {
    if ((first != 0) && (second > (std::numeric_limits< std::size_t >::max() / first)))
    {
      return false;
    }
    result = first * second;
    return true;
  }

  static bool parseNumber(const char* text, int& result)
  {
    if ((text[0] < first_digit) || (text[0] > last_digit) || (text[1] != '\0'))
    {
      return false;
    }
    result = text[0] - first_digit;
    return true;
  }

  static bool readSize(std::istream& input, std::size_t& rows, std::size_t& cols)
  {
    long long row_count = 0;
    long long col_count = 0;
    if (!(input >> row_count >> col_count))
    {
      return false;
    }
    if ((row_count < 0) || (col_count < 0))
    {
      return false;
    }
    rows = static_cast< std::size_t >(row_count);
    cols = static_cast< std::size_t >(col_count);
    return true;
  }

  static bool readElements(std::istream& input, int* data, std::size_t count)
  {
    std::size_t index = 0;
    while (index < count)
    {
      if (!(input >> data[index]))
      {
        return false;
      }
      if (!safeAdd(index, size_unit, index))
      {
        return false;
      }
    }
    char extra = 0;
    input >> extra;
    return input.eof();
  }

  static bool elementIndex(std::size_t row, std::size_t col, std::size_t cols, std::size_t& index)
  {
    std::size_t shift = 0;
    if (!safeMul(row, cols, shift))
    {
      return false;
    }
    if (!safeAdd(shift, col, index))
    {
      return false;
    }
    return true;
  }

  static bool countDiagonal(const int* matrix, std::size_t rows, std::size_t cols, std::size_t start_row,
      std::size_t start_col, std::size_t& count)
  {
    std::size_t row = start_row;
    std::size_t col = start_col;
    while ((row < rows) && (col < cols))
    {
      std::size_t index = 0;
      if (!elementIndex(row, col, cols, index))
      {
        return false;
      }
      if (matrix[index] == 0)
      {
        return true;
      }
      if (!safeAdd(row, size_unit, row) || !safeAdd(col, size_unit, col))
      {
        return false;
      }
    }
    if (!safeAdd(count, size_unit, count))
    {
      return false;
    }
    return true;
  }

  static bool countZeroFreeDiagonals(const int* matrix, std::size_t rows, std::size_t cols, std::size_t& count)
  {
    count = 0;
    if ((rows == 0) || (cols == 0))
    {
      return true;
    }
    std::size_t row = size_unit;
    while (row < rows)
    {
      if (!countDiagonal(matrix, rows, cols, row, 0, count))
      {
        return false;
      }
      if (!safeAdd(row, size_unit, row))
      {
        return false;
      }
    }

    std::size_t col = size_unit;
    while (col < cols)
    {
      if (!countDiagonal(matrix, rows, cols, 0, col, count))
      {
        return false;
      }
      if (!safeAdd(col, size_unit, col))
      {
        return false;
      }
    }
    return true;
  }

  static bool decreaseElement(int* matrix, std::size_t row, std::size_t col, std::size_t cols, long long& step)
  {
    std::size_t index = 0;
    if (!elementIndex(row, col, cols, index))
    {
      return false;
    }
    long long result = 0;
    if (!safeSub(matrix[index], step, result))
    {
      return false;
    }
    if ((result < std::numeric_limits< int >::min()) || (result > std::numeric_limits< int >::max()))
    {
      return false;
    }
    if (!safeAdd(step, unit, step))
    {
      return false;
    }
    matrix[index] = static_cast< int >(result);
    return true;
  }

  static bool spiralDecrease(int* matrix, std::size_t rows, std::size_t cols)
  {
    std::size_t top = 0;
    std::size_t bottom = rows;
    std::size_t left = 0;
    std::size_t right = cols;
    long long step = unit;

    while ((top < bottom) && (left < right))
    {
      std::size_t row = bottom;
      while (row > top)
      {
        if (!safeSub(row, size_unit, row))
        {
          return false;
        }
        if (!decreaseElement(matrix, row, left, cols, step))
        {
          return false;
        }
      }
      if (!safeAdd(left, size_unit, left))
      {
        return false;
      }
      if (left >= right)
      {
        break;
      }

      std::size_t col = left;
      while (col < right)
      {
        if (!decreaseElement(matrix, top, col, cols, step))
        {
          return false;
        }
        if (!safeAdd(col, size_unit, col))
        {
          return false;
        }
      }
      if (!safeAdd(top, size_unit, top))
      {
        return false;
      }
      if (top >= bottom)
      {
        break;
      }

      if (!safeSub(right, size_unit, right))
      {
        return false;
      }
      row = top;
      while (row < bottom)
      {
        if (!decreaseElement(matrix, row, right, cols, step))
        {
          return false;
        }
        if (!safeAdd(row, size_unit, row))
        {
          return false;
        }
      }
      if (left >= right)
      {
        break;
      }

      if (!safeSub(bottom, size_unit, bottom))
      {
        return false;
      }
      col = right;
      while (col > left)
      {
        if (!safeSub(col, size_unit, col))
        {
          return false;
        }
        if (!decreaseElement(matrix, bottom, col, cols, step))
        {
          return false;
        }
      }
    }
    return true;
  }

  static bool printMatrix(
      std::ostream& output, const int* matrix, std::size_t rows, std::size_t cols, std::size_t count)
  {
    output << rows << ' ' << cols;
    std::size_t index = 0;
    while (index < count)
    {
      output << ' ' << matrix[index];
      if (!safeAdd(index, size_unit, index))
      {
        return false;
      }
    }
    output << '\n';
    return true;
  }

  static int process(
      std::istream& input, std::ostream& output, int* data, std::size_t rows, std::size_t cols, std::size_t count)
  {
    if (!readElements(input, data, count))
    {
      std::cerr << "Invalid matrix\n";
      return status_invalid_input;
    }

    int status = status_ok;

    std::size_t diagonals = 0;
    if (countZeroFreeDiagonals(data, rows, cols, diagonals))
    {
      output << diagonals << '\n';
    }
    else
    {
      std::cerr << "CNT-NZR-DIG: Overflow\n";
      status = status_failure;
    }

    if (spiralDecrease(data, rows, cols))
    {
      if (!printMatrix(output, data, rows, cols, count))
      {
        std::cerr << "LFT-BOT-CLK: Overflow\n";
        status = status_failure;
      }
    }
    else
    {
      std::cerr << "LFT-BOT-CLK: Element overflow\n";
      status = status_failure;
    }

    if (!output.good())
    {
      std::cerr << "Cannot write output file\n";
      status = status_failure;
    }
    return status;
  }

  static int run(std::istream& input, std::ostream& output, int mode)
  {
    std::size_t rows = 0;
    std::size_t cols = 0;
    if (!readSize(input, rows, cols))
    {
      std::cerr << "Invalid matrix\n";
      return status_invalid_input;
    }

    std::size_t count = 0;
    const bool fits = safeMul(rows, cols, count);

    if (mode == fixed_mode)
    {
      if (!fits || (count > fixed_capacity))
      {
        std::cerr << "Invalid matrix\n";
        return status_invalid_input;
      }
      int data[fixed_capacity] = {};
      return process(input, output, data, rows, cols, count);
    }

    if (!fits)
    {
      std::cerr << "Not enough memory\n";
      return status_invalid_input;
    }

    int* data = nullptr;
    int status = status_ok;
    try
    {
      data = new int[count];
      status = process(input, output, data, rows, cols, count);
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "Not enough memory\n";
      status = status_invalid_input;
    }
    delete[] data;
    return status;
  }
}

int main(int argc, char** argv)
{
  if (argc != novikov::expected_arguments)
  {
    std::cerr << ((argc < novikov::expected_arguments) ? "Not enough arguments\n" : "Too many arguments\n");
    return novikov::status_failure;
  }

  const char* const mode_text = argv[1];
  const char* const input_path = argv[2];
  const char* const output_path = argv[3];

  int mode = 0;
  if (!novikov::parseNumber(mode_text, mode))
  {
    std::cerr << "First parameter is not a number\n";
    return novikov::status_failure;
  }
  if ((mode != novikov::fixed_mode) && (mode != novikov::dynamic_mode))
  {
    std::cerr << "First parameter is out of range\n";
    return novikov::status_failure;
  }

  std::ifstream input{input_path};
  if (!input)
  {
    std::cerr << "Cannot open input file\n";
    return novikov::status_failure;
  }

  std::ofstream output{output_path};
  if (!output)
  {
    std::cerr << "Cannot open output file\n";
    return novikov::status_failure;
  }

  return novikov::run(input, output, mode);
}
