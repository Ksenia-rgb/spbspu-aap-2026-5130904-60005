#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <new>

namespace shcherbakov
{
  const int error_args = 1;
  const int error_data = 2;
  const int mode_fixed = 1;
  const int mode_dynamic = 2;
  const std::size_t fixed_capacity = 10000;
  const std::size_t min_loc_max_dimension = 3;
  const std::size_t neighbor_span = 3;
  const std::size_t center_offset = 1;

  std::size_t countSaddlePoints(const int * matrix, std::size_t rows, std::size_t cols)
  {
    std::size_t count = 0;
    for (std::size_t i = 0; i < rows; ++i)
    {
      for (std::size_t j = 0; j < cols; ++j)
      {
        const int val = matrix[i * cols + j];
        bool is_saddle = true;
        for (std::size_t c = 0; c < cols; ++c)
        {
          if (matrix[i * cols + c] < val)
          {
            is_saddle = false;
            break;
          }
        }
        if (!is_saddle)
        {
          continue;
        }
        for (std::size_t r = 0; r < rows; ++r)
        {
          if (matrix[r * cols + j] > val)
          {
            is_saddle = false;
            break;
          }
        }
        if (is_saddle)
        {
          ++count;
        }
      }
    }
    return count;
  }

  std::size_t countLocalMax(const int * matrix, std::size_t rows, std::size_t cols)
  {
    if (rows < min_loc_max_dimension || cols < min_loc_max_dimension)
    {
      return 0;
    }
    std::size_t count = 0;
    for (std::size_t i = 1; i + 1 < rows; ++i)
    {
      for (std::size_t j = 1; j + 1 < cols; ++j)
      {
        const int val = matrix[i * cols + j];
        bool is_max = true;
        for (std::size_t di = 0; di < neighbor_span && is_max; ++di)
        {
          for (std::size_t dj = 0; dj < neighbor_span; ++dj)
          {
            if (di == center_offset && dj == center_offset)
            {
              continue;
            }
            const std::size_t ni = i + di - center_offset;
            const std::size_t nj = j + dj - center_offset;
            if (val <= matrix[ni * cols + nj])
            {
              is_max = false;
              break;
            }
          }
        }
        if (is_max)
        {
          ++count;
        }
      }
    }
    return count;
  }

  bool parseMode(const char * arg, int & mode)
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

  bool readDimensions(std::istream & input, std::size_t & rows, std::size_t & cols)
  {
    long long r = 0;
    long long c = 0;
    if (!(input >> r >> c) || r < 0 || c < 0)
    {
      return false;
    }
    rows = static_cast< std::size_t >(r);
    cols = static_cast< std::size_t >(c);
    const std::size_t max_size = std::numeric_limits< std::size_t >::max();
    if (rows != 0 && cols > max_size / rows)
    {
      return false;
    }
    return true;
  }

  bool readElements(std::istream & input, int * data, std::size_t count)
  {
    for (std::size_t i = 0; i < count; ++i)
    {
      if (!(input >> data[i]))
      {
        return false;
      }
    }
    char extra = '\0';
    if (input >> extra)
    {
      return false;
    }
    return true;
  }

  int processMatrix(int mode, std::istream & input, std::ostream & output)
  {
    std::size_t rows = 0;
    std::size_t cols = 0;
    if (!readDimensions(input, rows, cols))
    {
      std::cerr << "Invalid matrix dimensions\n";
      return error_data;
    }
    const std::size_t count = rows * cols;
    if (mode == mode_fixed)
    {
      if (count > fixed_capacity)
      {
        std::cerr << "Matrix size exceeds fixed capacity\n";
        return error_data;
      }
      int static_buffer[fixed_capacity];
      if (!readElements(input, static_buffer, count))
      {
        std::cerr << "Invalid matrix elements\n";
        return error_data;
      }
      const std::size_t saddle_count = countSaddlePoints(static_buffer, rows, cols);
      const std::size_t loc_max_count = countLocalMax(static_buffer, rows, cols);
      output << saddle_count << "\n";
      output << loc_max_count << "\n";
      return 0;
    }
    int * const dynamic_buffer = new (std::nothrow) int[count];
    if (count > 0 && dynamic_buffer == nullptr)
    {
      std::cerr << "Memory allocation failed\n";
      return error_data;
    }
    if (!readElements(input, dynamic_buffer, count))
    {
      delete[] dynamic_buffer;
      std::cerr << "Invalid matrix elements\n";
      return error_data;
    }
    const std::size_t saddle_count = countSaddlePoints(dynamic_buffer, rows, cols);
    const std::size_t loc_max_count = countLocalMax(dynamic_buffer, rows, cols);
    delete[] dynamic_buffer;
    output << saddle_count << "\n";
    output << loc_max_count << "\n";
    return 0;
  }
}

int main(int argc, char ** argv)
{
  const int expected_argc = 4;
  if (argc != expected_argc)
  {
    std::cerr << "Invalid number of arguments\n";
    return shcherbakov::error_args;
  }

  int mode = 0;
  if (!shcherbakov::parseMode(argv[1], mode))
  {
    std::cerr << "Invalid mode argument\n";
    return shcherbakov::error_args;
  }

  std::ifstream input(argv[2]);
  if (!input)
  {
    std::cerr << "Cannot open input file\n";
    return shcherbakov::error_args;
  }

  std::ofstream output(argv[3]);
  if (!output)
  {
    std::cerr << "Cannot open output file\n";
    return shcherbakov::error_args;
  }

  return shcherbakov::processMatrix(mode, input, output);
}
