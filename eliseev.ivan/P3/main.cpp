#include <iostream>
#include <fstream>
#include <new>
namespace eliseev
{
  const int ERROR_INVALID_ARGS = 1;
  const int ERROR_ALLOCATION = 2;
  const int ERROR_FILE = 3;
  const int ERROR_EXTRA_DATA = 4;

  int* createMatrix(size_t rows, size_t cols)
  {
    const size_t total = rows * cols;
    int* const matrix = new (std::nothrow) int[total];
    if (matrix == nullptr)
    {
      throw ERROR_ALLOCATION;
    }
    return matrix;
  }

  void readSizes(std::ifstream& in, size_t& rows, size_t& cols)
  {
    if (!(in >> rows >> cols))
    {
      throw ERROR_FILE;
    }
  }

  void readMatrix(std::ifstream& in, int* matrix, size_t rows, size_t cols)
  {
    for (size_t i = 0; i < rows; ++i)
    {
      for (size_t j = 0; j < cols; ++j)
      {
        if (!(in >> matrix[i * cols + j]))
        {
          throw ERROR_FILE;
        }
      }
    }

    char c = '\0';
    if (in >> c)
    {
      throw ERROR_EXTRA_DATA;
    }
  }

  size_t countColsNoSame(const int* matrix, size_t rows, size_t cols)
  {
    size_t count = 0;
    for (size_t j = 0; j < cols; ++j)
    {
      bool has_same = false;
      for (size_t i = 1; i < rows; ++i)
      {
        if (matrix[i * cols + j] == matrix[(i - 1) * cols + j])
        {
          has_same = true;
          break;
        }
      }
      if (!has_same)
      {
        ++count;
      }
    }
    return count;
  }

  size_t countNonZeroDiagonals(const int* matrix, size_t rows, size_t cols)
  {
    size_t count = 0;
    const long long rows_ll = rows;
    const long long cols_ll = cols;

    for (long long d = -(cols_ll - 1); d <= (rows_ll - 1); ++d)
    {
      bool has_zero = false;
      for (size_t i = 0; i < rows; ++i)
      {
        const long long i_ll = i;
        const long long j = i_ll - d;

        if (j < 0 || j >= cols_ll)
        {
          continue;
        }

        if (matrix[i * cols + j] == 0)
        {
          has_zero = true;
          break;
        }
      }
      if (!has_zero)
      {
        ++count;
      }
    }
    return count;
  }

  void runFixed(std::ifstream& in, std::ofstream& out)
  {
    const size_t MAX = 10000;
    int flat[MAX];

    size_t rows = 0, cols = 0;
    readSizes(in, rows, cols);

    if (rows == 0 || cols == 0)
    {
      return;
    }

    if (rows > MAX / cols)
    {
      throw ERROR_FILE;
    }

    readMatrix(in, flat, rows, cols);

    const size_t var9 = countColsNoSame(flat, rows, cols);
    const size_t var18 = countNonZeroDiagonals(flat, rows, cols);

    out << var9 << '\n';
    out << var18 << '\n';
  }

  void runDynamic(std::ifstream& in, std::ofstream& out)
  {
    size_t rows = 0, cols = 0;
    readSizes(in, rows, cols);

    if (rows == 0 || cols == 0)
    {
      return;
    }

    int* const matrix = createMatrix(rows, cols);

    try
    {
      readMatrix(in, matrix, rows, cols);
    }
    catch (int e)
    {
      delete[] matrix;
      throw e;
    }

    const size_t var9 = countColsNoSame(matrix, rows, cols);
    const size_t var18 = countNonZeroDiagonals(matrix, rows, cols);

    out << var9 << '\n';
    out << var18 << '\n';

    delete[] matrix;
  }

  int parseNum(const char* const s)
  {
    if (!s || !*s)
    {
      throw ERROR_INVALID_ARGS;
    }
    if (s[0] != '1' && s[0] != '2')
    {
      throw ERROR_INVALID_ARGS;
    }
    if (s[1] != '\0')
    {
      throw ERROR_INVALID_ARGS;
    }
    return s[0] - '0';
  }
}

int main(int argc, char* argv[])
{
  using namespace eliseev;

  try
  {
    if (argc != 4)
    {
      throw ERROR_INVALID_ARGS;
    }

    const int num = parseNum(argv[1]);

    std::ifstream in(argv[2]);
    if (!in.is_open())
    {
      throw ERROR_FILE;
    }
    std::ofstream out(argv[3]);
    if (!out.is_open())
    {
      throw ERROR_FILE;
    }

    if (num == 1)
    {
      runFixed(in, out);
    }
    else
    {
      runDynamic(in, out);
    }
  }
  catch (int e)
  {
    std::cerr << "Error " << e << '\n';
    return e;
  }

  return 0;
}
