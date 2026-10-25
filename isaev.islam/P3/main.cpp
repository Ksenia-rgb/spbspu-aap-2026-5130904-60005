#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace isaev
{
  const int expectedArgc = 4;
  const int taskIndex = 1;
  const int inputIndex = 2;
  const int outputIndex = 3;
  const int badArgumentsCode = 1;
  const int badDataCode = 2;
  const std::size_t fixedCapacity = 10000;
  const std::size_t lastIndexOffset = 1;

  static bool isFixedArrayTask(const char *argument)
  {
    if (std::strcmp(argument, "1") == 0)
    {
      return true;
    }
    if (std::strcmp(argument, "2") == 0)
    {
      return false;
    }
    throw std::invalid_argument("the first parameter must be 1 or 2");
  }

  static void readMatrixSize(std::istream &input, std::size_t &rows, std::size_t &cols)
  {
    int intRows = 0;
    int intCols = 0;
    if (!(input >> intRows >> intCols))
    {
      throw std::runtime_error("matrix size is missing or is not a pair of integers");
    }
    if ((intRows < 0) || (intCols < 0))
    {
      throw std::runtime_error("matrix size must not be negative");
    }
    rows = static_cast< std::size_t >(intRows);
    cols = static_cast< std::size_t >(intCols);
    if ((cols != 0) && (rows > (std::numeric_limits< std::size_t >::max() / cols)))
    {
      throw std::runtime_error("matrix is too large");
    }
  }

  static void readMatrixElements(std::istream &input, int *elements, std::size_t rows, std::size_t cols)
  {
    const std::size_t count = rows * cols;
    for (std::size_t index = 0; index < count; ++index)
    {
      if (!(input >> elements[index]))
      {
        throw std::runtime_error("matrix element is missing or is not an integer");
      }
    }
    char extra = 0;
    if (input >> extra)
    {
      throw std::runtime_error("unexpected data after the matrix elements");
    }
  }

  static bool containsZero(const int *elements, std::size_t rows, std::size_t cols, std::size_t row, std::size_t col)
  {
    for (std::size_t step = 0; ((row + step) < rows) && ((col + step) < cols); ++step)
    {
      if (elements[(row + step) * cols + (col + step)] == 0)
      {
        return true;
      }
    }
    return false;
  }

  static long long sumMinorDiagonal(const int *elements, std::size_t rows, std::size_t cols,
      std::size_t row, std::size_t col)
  {
    long long sum = 0;
    for (std::size_t step = 0; ((row + step) < rows) && (step <= col); ++step)
    {
      sum += elements[(row + step) * cols + (col - step)];
    }
    return sum;
  }

  static std::size_t countNonZeroDiagonals(const int *elements, std::size_t rows, std::size_t cols)
  {
    std::size_t count = 0;
    for (std::size_t col = 0; col < cols; ++col)
    {
      if ((col != 0) && !containsZero(elements, rows, cols, 0, col))
      {
        ++count;
      }
    }
    for (std::size_t row = 0; row < rows; ++row)
    {
      if ((row != 0) && !containsZero(elements, rows, cols, row, 0))
      {
        ++count;
      }
    }
    return count;
  }

  static long long findMaxMinorDiagonalSum(const int *elements, std::size_t rows, std::size_t cols)
  {
    if ((rows == 0) || (cols == 0))
    {
      return 0;
    }
    const std::size_t lastCol = cols - lastIndexOffset;
    const long long noSum = std::numeric_limits< long long >::min();
    long long maxSum = noSum;
    for (std::size_t col = 0; col < lastCol; ++col)
    {
      maxSum = std::max(maxSum, sumMinorDiagonal(elements, rows, cols, 0, col));
    }
    for (std::size_t row = 0; row < rows; ++row)
    {
      if (row != 0)
      {
        maxSum = std::max(maxSum, sumMinorDiagonal(elements, rows, cols, row, lastCol));
      }
    }
    return (maxSum == noSum) ? 0 : maxSum;
  }

  static void writeResults(const char *outputName, const int *elements, size_t rows, size_t cols)
  {
    const std::size_t nonZeroDiagonals = countNonZeroDiagonals(elements, rows, cols);
    const long long maxDiagonalSum = findMaxMinorDiagonalSum(elements, rows, cols);
    std::ofstream output(outputName);
    if (!output)
    {
      throw std::runtime_error("cannot open the output file");
    }
    output << nonZeroDiagonals << '\n';
    output << maxDiagonalSum << '\n';
    output.flush();
    if (!output)
    {
      throw std::runtime_error("cannot write to the output file");
    }
  }

  static void processFixedArray(std::istream &input, const char *outputName)
  {
    std::size_t rows = 0;
    std::size_t cols = 0;
    readMatrixSize(input, rows, cols);
    if ((rows * cols) > fixedCapacity)
    {
      throw std::runtime_error("the matrix does not fit into the fixed-size array");
    }
    int elements[fixedCapacity] = {};
    readMatrixElements(input, elements, rows, cols);
    writeResults(outputName, elements, rows, cols);
  }

  static void processDynamicArray(std::istream &input, const char *outputName)
  {
    std::size_t rows = 0;
    std::size_t cols = 0;
    readMatrixSize(input, rows, cols);
    int *elements = new int[rows * cols]();
    try
    {
      readMatrixElements(input, elements, rows, cols);
      writeResults(outputName, elements, rows, cols);
    }
    catch (...)
    {
      delete[] elements;
      throw;
    }
    delete[] elements;
  }
}

int main(int argc, char **argv)
{
  try
  {
    if (argc != isaev::expectedArgc)
    {
      throw std::invalid_argument("expected parameters: <task number> <input file> <output file>");
    }
    const bool isFixed = isaev::isFixedArrayTask(argv[isaev::taskIndex]);
    std::ifstream input(argv[isaev::inputIndex]);
    if (!input)
    {
      throw std::runtime_error("cannot open the input file");
    }
    if (isFixed)
    {
      isaev::processFixedArray(input, argv[isaev::outputIndex]);
    }
    else
    {
      isaev::processDynamicArray(input, argv[isaev::outputIndex]);
    }
  }
  catch (const std::invalid_argument &error)
  {
    std::cerr << "Error:" << error.what() << '\n';
    return isaev::badArgumentsCode;
  }
  catch (const std::exception &error)
  {
    std::cerr << "Error" << error.what() << '\n';
    return isaev::badDataCode;
  }
  return 0;
}