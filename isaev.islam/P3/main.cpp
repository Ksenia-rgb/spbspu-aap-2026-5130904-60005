#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <cstddef>

namespace isaev
{
  bool readMatrixData(std::istream& in, std::size_t& rows, std::size_t& cols)
  {
    if (!(in >> rows >> cols))
    {
      return false;
    }

    if (rows == 0 || cols == 0)
    {
      return false;
    }

    return true;
  }

  namespace detail
  {
    void countValidDiagonals(std::size_t rows, std::size_t cols, const int* matrix, std::ostream& out)
    {
      bool first = true;

      for (int d = -static_cast<int>(rows) + 1; d < static_cast<int>(cols); ++d)
      {
        bool hasZero = false;
        bool hasElements = false;

        for (std::size_t i = 0; i < rows; ++i)
        {
          int j = static_cast<int>(i) - d;
          if (j >= 0 && j < static_cast<int>(cols))
          {
            hasElements = true;
            if (matrix[i * cols + j] == 0)
            {
              hasZero = true;
              break;
            }
          }
        }

        if (hasElements && !hasZero)
        {
          int count = 0;
          for (std::size_t i = 0; i < rows; ++i)
          {
            int j = static_cast<int>(i) - d;
            if (j >= 0 && j < static_cast<int>(cols))
            {
              count++;
            }
          }

          if (!first)
          {
            out << ' ';
          }
          out << count;
          first = false;
        }
      }

      if (first)
      {
        out << 0;
      }
      out << '\n';
    }

    void maxSecondaryDiagonalSum(std::size_t rows, std::size_t cols, const int* matrix, std::ostream& out)
    {
      long long maxSum = 0;
      bool initialized = false;

      for (int s = 0; s < static_cast<int>(rows + cols - 1); ++s)
      {
        long long currentSum = 0;
        bool hasElements = false;

        for (std::size_t i = 0; i < rows; ++i)
        {
          int j = s - static_cast<int>(i);
          if (j >= 0 && j < static_cast<int>(cols))
          {
            hasElements = true;
            currentSum += matrix[i * cols + j];
          }
        }

        if (hasElements)
        {
          if (!initialized || currentSum > maxSum)
          {
            maxSum = currentSum;
            initialized = true;
          }
        }
      }

      if (initialized)
      {
        out << maxSum << '\n';
      }
      else
      {
        out << 0 << '\n';
      }
    }
  }

  void processTasks(std::size_t rows, std::size_t cols, const int* matrix, std::ostream& out)
  {
    detail::countValidDiagonals(rows, cols, matrix, out);
    detail::maxSecondaryDiagonalSum(rows, cols, matrix, out);
  }
}

int main(int argc, char** argv)
{
  if (argc != 4)
  {
    std::cerr << "Error: Not enough or too many arguments\n";
    return 1;
  }

  int mode = 0;
  try
  {
    std::size_t idx = 0;
    mode = std::stoi(argv[1], &idx);
    if (idx != std::strlen(argv[1]))
    {
      std::cerr << "Error: First parameter is not a valid number\n";
      return 1;
    }
  }
  catch (...)
  {
    std::cerr << "Error: First parameter cannot be interpreted as a number\n";
    return 1;
  }

  if (mode != 1 && mode != 2)
  {
    std::cerr << "Error: First parameter is out of range (expected 1 or 2)\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input)
  {
    std::cerr << "Error: Cannot open input file\n";
    return 2;
  }

  std::size_t rows = 0;
  std::size_t cols = 0;
  if (!isaev::readMatrixData(input, rows, cols))
  {
    std::cerr << "Error: Failed to read matrix dimensions\n";
    return 2;
  }

  const std::size_t maxFixedElements = 10000;
  if (rows * cols > maxFixedElements && mode == 1)
  {
    std::cerr << "Error: Matrix size exceeds fixed storage limit\n";
    return 2;
  }

  if (mode == 1)
  {
    int matrix[10000];
    for (std::size_t i = 0; i < rows * cols; ++i)
    {
      if (!(input >> matrix[i]))
      {
        std::cerr << "Error: Failed to read matrix element\n";
        return 2;
      }
    }

    std::ofstream output(argv[3]);
    if (!output)
    {
      std::cerr << "Error: Cannot open output file\n";
      return 2;
    }

    isaev::processTasks(rows, cols, matrix, output);
  }
  else
  {
    int* matrix = nullptr;
    try
    {
      matrix = new int[rows * cols];
    }
    catch (...)
    {
      std::cerr << "Error: Memory allocation failed\n";
      return 2;
    }

    for (std::size_t i = 0; i < rows * cols; ++i)
    {
      if (!(input >> matrix[i]))
      {
        delete[] matrix;
        std::cerr << "Error: Failed to read matrix element\n";
        return 2;
      }
    }

    std::ofstream output(argv[3]);
    if (!output)
    {
      delete[] matrix;
      std::cerr << "Error: Cannot open output file\n";
      return 2;
    }

    isaev::processTasks(rows, cols, matrix, output);
    delete[] matrix;
  }

  return 0;
}