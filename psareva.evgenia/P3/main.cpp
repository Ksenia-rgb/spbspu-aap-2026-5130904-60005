#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <new>

namespace psareva
{
  const int k_args_count = 4;
  const int k_arg_error_code = 1;
  const int k_data_error_code = 2;
  const int k_fixed_mode = 1;
  const int k_dynamic_mode = 2;
  const std::size_t k_fixed_capacity = 10000;
  const std::size_t k_min_dimension_for_loc_min = 3;
  const std::size_t k_neighbor_span = 3;
  const std::size_t k_center_index = 1;
  const std::size_t k_mode_arg_index = 1;
  const std::size_t k_input_path_arg_index = 2;
  const std::size_t k_output_path_arg_index = 3;

  std::size_t minSize(std::size_t a, std::size_t b)
  {
    return (a < b) ? a : b;
  }

  bool parseMode(const char * text, int & mode)
  {
    if (text == nullptr || text[0] == '\0' || text[1] != '\0')
    {
      return false;
    }
    if (text[0] == '1')
    {
      mode = k_fixed_mode;
      return true;
    }
    if (text[0] == '2')
    {
      mode = k_dynamic_mode;
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

  bool writeMatrix(std::ostream & output, const int * data, std::size_t rows, std::size_t cols)
  {
    output << rows << ' ' << cols;
    for (std::size_t i = 0; i < rows * cols; ++i)
    {
      output << ' ' << data[i];
    }
    output << '\n';
    return output.good();
  }

  std::size_t countLocalMin(const int * data, std::size_t rows, std::size_t cols)
  {
    if (rows < k_min_dimension_for_loc_min || cols < k_min_dimension_for_loc_min)
    {
      return 0;
    }
    std::size_t count = 0;
    for (std::size_t i = 1; i + 1 < rows; ++i)
    {
      for (std::size_t j = 1; j + 1 < cols; ++j)
      {
        const int current = data[i * cols + j];
        bool is_min = true;
        for (std::size_t di = 0; di < k_neighbor_span && is_min; ++di)
        {
          for (std::size_t dj = 0; dj < k_neighbor_span; ++dj)
          {
            if (di == k_center_index && dj == k_center_index)
            {
              continue;
            }
            const std::size_t ni = i + di - k_center_index;
            const std::size_t nj = j + dj - k_center_index;
            if (current >= data[ni * cols + nj])
            {
              is_min = false;
              break;
            }
          }
        }
        if (is_min)
        {
          ++count;
        }
      }
    }
    return count;
  }

  bool fillIncWav(int * data, std::size_t rows, std::size_t cols)
  {
    for (std::size_t i = 0; i < rows; ++i)
    {
      for (std::size_t j = 0; j < cols; ++j)
      {
        const std::size_t top = i;
        const std::size_t bottom = rows - 1 - i;
        const std::size_t left = j;
        const std::size_t right = cols - 1 - j;
        const std::size_t layer = minSize(minSize(top, bottom), minSize(left, right));
        const long long val = static_cast< long long >(data[i * cols + j]);
        const long long inc = static_cast< long long >(layer) + 1;
        const long long next = val + inc;
        if (next > std::numeric_limits< int >::max() || next < std::numeric_limits< int >::min())
        {
          return false;
        }
        data[i * cols + j] = static_cast< int >(next);
      }
    }
    return true;
  }

  int process(int * data, std::size_t rows, std::size_t cols, std::ostream & output)
  {
    const std::size_t loc_mins = countLocalMin(data, rows, cols);
    if (!fillIncWav(data, rows, cols))
    {
      std::cerr << "Value overflow\n";
      return k_data_error_code;
    }
    output << loc_mins << '\n';
    if (!writeMatrix(output, data, rows, cols))
    {
      std::cerr << "Write error\n";
      return k_data_error_code;
    }
    return 0;
  }

  int run(int mode, std::istream & input, std::ostream & output)
  {
    std::size_t rows = 0;
    std::size_t cols = 0;
    if (!readDimensions(input, rows, cols))
    {
      std::cerr << "Invalid matrix size\n";
      return k_data_error_code;
    }
    const std::size_t count = rows * cols;
    if (mode == k_fixed_mode)
    {
      if (count > k_fixed_capacity)
      {
        std::cerr << "Matrix is too big for fixed array\n";
        return k_data_error_code;
      }
      int buffer[k_fixed_capacity];
      if (!readElements(input, buffer, count))
      {
        std::cerr << "Invalid matrix elements\n";
        return k_data_error_code;
      }
      return process(buffer, rows, cols, output);
    }
    int * const buffer = new (std::nothrow) int[count];
    if (count > 0 && buffer == nullptr)
    {
      std::cerr << "Not enough memory\n";
      return k_data_error_code;
    }
    if (!readElements(input, buffer, count))
    {
      delete[] buffer;
      std::cerr << "Invalid matrix elements\n";
      return k_data_error_code;
    }
    const int code = process(buffer, rows, cols, output);
    delete[] buffer;
    return code;
  }
}

int main(int argc, char ** argv)
{
  if (argc != psareva::k_args_count)
  {
    std::cerr << "Invalid arguments count\n";
    return psareva::k_arg_error_code;
  }
  int mode = 0;
  if (!psareva::parseMode(argv[psareva::k_mode_arg_index], mode))
  {
    std::cerr << "First parameter must be 1 or 2\n";
    return psareva::k_arg_error_code;
  }
  std::ifstream input(argv[psareva::k_input_path_arg_index]);
  if (!input)
  {
    std::cerr << "Cannot open input file\n";
    return psareva::k_arg_error_code;
  }
  std::ofstream output(argv[psareva::k_output_path_arg_index]);
  if (!output)
  {
    std::cerr << "Cannot open output file\n";
    return psareva::k_arg_error_code;
  }
  return psareva::run(mode, input, output);
}
