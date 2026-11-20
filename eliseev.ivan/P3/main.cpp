#include <iostream>
#include <fstream>
#include <new>
#include <cstddef>

namespace eliseev
{
  int* createMatrix(size_t rows, size_t cols)
  {
    const size_t total = rows * cols;
    int* const matrix = new (std::nothrow) int[total];
    if (matrix == nullptr)
    {
      throw 2;
    }
    return matrix;
  }

  void readSizes(std::ifstream& in, size_t& rows, size_t& cols)
  {
    if (!(in >> rows >> cols))
    {
      throw 3;
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
          throw 3;
        }
      }
    }
    int extra = 0;
    if (in >> extra)
    {
      throw 4;
    }
  }

  int countColsNoSame(const int* matrix, size_t rows, size_t cols)
  {
    int count = 0;
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

  int countNonZeroDiagonals(const int* matrix, size_t rows, size_t cols)
  {
    int count = 0;
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

  void writeResults(std::ofstream& out, int r9, int r18)
  {
    out << r9 << '\n';
    out << r18 << '\n';
  }

  int parseNum(const char* const s)
  {
    if (!s || !*s)
    {
      throw 1;
    }
    if (s[0] != '1' && s[0] != '2')
    {
      throw 1;
    }
    if (s[1] != '\0')
    {
      throw 1;
    }
    return s[0] - '0';
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
      throw 3;
    }

    readMatrix(in, flat, rows, cols);

    const int var9 = countColsNoSame(flat, rows, cols);
    const int var18 = countNonZeroDiagonals(flat, rows, cols);

    writeResults(out, var9, var18);
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

    const int var9 = countColsNoSame(matrix, rows, cols);
    const int var18 = countNonZeroDiagonals(matrix, rows, cols);
    writeResults(out, var9, var18);
    delete[] matrix;
  }
}

int main(int argc, char* argv[])
{
  using namespace eliseev;

  try
  {
    if (argc != 4)
    {
      throw 1;
    }

    const int num = parseNum(argv[1]);

    std::ifstream in(argv[2]);
    if (!in.is_open())
    {
      throw 3;
    }
    std::ofstream out(argv[3]);
    if (!out.is_open())
    {
      throw 3;
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
