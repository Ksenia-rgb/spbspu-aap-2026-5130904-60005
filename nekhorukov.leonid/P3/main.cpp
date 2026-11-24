#include <iostream>
#include <fstream>
#include <cstddef>
#include <limits>

namespace nekhorukov
{
  enum ErrorCode
  {
    kSuccess = 0,
    kInputError = 1,
    kCalcError = 2
  };

  const std::size_t kMaxFixedElements = 10000;

  bool isUpperTriangular(const long long * data, std::size_t rows, std::size_t cols)
  {
    const std::size_t size = rows < cols ? rows : cols;
    for (std::size_t i = 0; i < size; ++i)
    {
      for (std::size_t j = 0; j < i; ++j)
      {
        if (data[i * cols + j] != 0)
        {
          return false;
        }
      }
    }
    return true;
  }

  int countRowsWithoutEqualNeighbors(const long long * data, std::size_t rows, std::size_t cols)
  {
    int count = 0;
    for (std::size_t i = 0; i < rows; ++i)
    {
      bool has_equal = false;
      for (std::size_t j = 1; j < cols; ++j)
      {
        if (data[i * cols + j] == data[i * cols + j - 1])
        {
          has_equal = true;
          break;
        }
      }
      if (!has_equal)
      {
        ++count;
      }
    }
    return count;
  }

  bool readMatrix(std::istream & input, long long * data, std::size_t total)
  {
    for (std::size_t i = 0; i < total; ++i)
    {
      if (!(input >> data[i]))
      {
        return false;
      }
    }
    input >> std::ws;
    if (input.peek() != std::char_traits< char >::eof())
    {
      return false;
    }
    return true;
  }

  bool writeResults(std::ostream & output, const long long * data, std::size_t rows, std::size_t cols)
  {
    output << (isUpperTriangular(data, rows, cols) ? "true" : "false") << '\n';
    output << countRowsWithoutEqualNeighbors(data, rows, cols) << '\n';
    return output.good();
  }

  bool writeEmptyResults(std::ostream & output)
  {
    output << "false" << '\n';
    output << 0 << '\n';
    return output.good();
  }

  int process(const char * mode, const char * input_path, const char * output_path)
  {
    const bool is_fixed = mode[0] == '1' && mode[1] == '\0';
    const bool is_dynamic = mode[0] == '2' && mode[1] == '\0';
    if (!is_fixed && !is_dynamic)
    {
      std::cerr << "first argument must be 1 or 2\n";
      return kInputError;
    }
    std::ifstream input(input_path);
    if (!input)
    {
      std::cerr << "cannot open input file\n";
      return kInputError;
    }
    std::ofstream output(output_path);
    if (!output)
    {
      std::cerr << "cannot open output file\n";
      return kInputError;
    }
    int rows = 0;
    int cols = 0;
    if (!(input >> rows >> cols) || rows < 0 || cols < 0)
    {
      std::cerr << "invalid matrix dimensions\n";
      return kCalcError;
    }
    const std::size_t row_count = static_cast< std::size_t >(rows);
    const std::size_t col_count = static_cast< std::size_t >(cols);
    if (row_count == 0 && col_count == 0)
    {
      if (!writeEmptyResults(output))
      {
        std::cerr << "cannot write results\n";
        return kCalcError;
      }
      return kSuccess;
    }
    if (row_count == 0 || col_count == 0)
    {
      std::cerr << "invalid matrix dimensions\n";
      return kCalcError;
    }
    if (row_count > std::numeric_limits< std::size_t >::max() / col_count)
    {
      std::cerr << "matrix is too large\n";
      return kCalcError;
    }
    const std::size_t total = row_count * col_count;
    if (is_fixed)
    {
      if (total > kMaxFixedElements)
      {
        std::cerr << "matrix is too large for fixed storage\n";
        return kCalcError;
      }
      long long buffer[kMaxFixedElements] = {};
      if (!readMatrix(input, buffer, total))
      {
        std::cerr << "invalid matrix data\n";
        return kCalcError;
      }
      if (!writeResults(output, buffer, row_count, col_count))
      {
        std::cerr << "cannot write results\n";
        return kCalcError;
      }
      return kSuccess;
    }
    long long * buffer = nullptr;
    try
    {
      buffer = new long long[total];
    }
    catch (...)
    {
      std::cerr << "memory allocation failed\n";
      return kCalcError;
    }
    if (!readMatrix(input, buffer, total))
    {
      delete[] buffer;
      std::cerr << "invalid matrix data\n";
      return kCalcError;
    }
    if (!writeResults(output, buffer, row_count, col_count))
    {
      delete[] buffer;
      std::cerr << "cannot write results\n";
      return kCalcError;
    }
    delete[] buffer;
    return kSuccess;
  }
}

int main(int argc, char * argv[])
{
  if (argc != 4)
  {
    std::cerr << "expected 3 arguments\n";
    return nekhorukov::kInputError;
  }
  return nekhorukov::process(argv[1], argv[2], argv[3]);
}
