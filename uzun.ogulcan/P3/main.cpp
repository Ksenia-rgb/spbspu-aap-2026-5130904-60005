#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <new>

namespace uzun
{
  const int arguments_error_code = 1;
  const int data_error_code = 2;
  const int fixed_mode = 1;
  const int dynamic_mode = 2;
  const std::size_t fixed_capacity = 10000;

  bool parse_mode(const char * text, int & mode)
  {
    if (text == nullptr || text[0] == '\0' || text[1] != '\0') {
      return false;
    }
    if (text[0] == '1') {
      mode = fixed_mode;
      return true;
    }
    if (text[0] == '2') {
      mode = dynamic_mode;
      return true;
    }
    return false;
  }

  bool read_size(std::istream & input, std::size_t & value)
  {
    long long number = 0;
    if (!(input >> number) || number < 0) {
      return false;
    }
    value = static_cast< std::size_t >(number);
    return true;
  }

  bool read_dimensions(std::istream & input, std::size_t & rows, std::size_t & cols)
  {
    if (!read_size(input, rows) || !read_size(input, cols)) {
      return false;
    }
    const std::size_t max_size = std::numeric_limits< std::size_t >::max();
    return rows == 0 || cols <= max_size / rows;
  }

  bool read_elements(std::istream & input, int * data, std::size_t count)
  {
    for (std::size_t i = 0; i < count; ++i) {
      if (!(input >> data[i])) {
        return false;
      }
    }
    return true;
  }

  bool write_matrix(std::ostream & output, const int * data, std::size_t rows, std::size_t cols)
  {
    output << rows << ' ' << cols;
    for (std::size_t i = 0; i < rows * cols; ++i) {
      output << ' ' << data[i];
    }
    output << '\n';
    return output.good();
  }

  long long diagonal_sum(const int * data, std::size_t rows, std::size_t cols, std::size_t start_row, std::size_t start_col)
  {
    long long sum = 0;
    for (std::size_t i = start_row, j = start_col; i < rows && j < cols; ++i, ++j) {
      sum += data[i * cols + j];
    }
    return sum;
  }

  void update_max(long long candidate, bool & is_found, long long & best)
  {
    if (!is_found || candidate > best) {
      best = candidate;
      is_found = true;
    }
  }

  long long max_sum_sdg(const int * data, std::size_t rows, std::size_t cols)
  {
    bool is_found = false;
    long long best = 0;
    if (rows == 0 || cols == 0) {
      return best;
    }
    for (std::size_t i = 1; i < rows; ++i) {
      update_max(diagonal_sum(data, rows, cols, i, 0), is_found, best);
    }
    for (std::size_t j = 1; j < cols; ++j) {
      update_max(diagonal_sum(data, rows, cols, 0, j), is_found, best);
    }
    return best;
  }

  bool decrease(int & value, long long & step)
  {
    const long long next = static_cast< long long >(value) - step;
    if (next < std::numeric_limits< int >::min()) {
      return false;
    }
    value = static_cast< int >(next);
    ++step;
    return true;
  }

  bool lft_top_clk(int * data, std::size_t rows, std::size_t cols)
  {
    std::size_t top = 0;
    std::size_t left = 0;
    std::size_t bottom = rows;
    std::size_t right = cols;
    long long step = 1;
    while (top < bottom && left < right) {
      for (std::size_t j = left; j < right; ++j) {
        if (!decrease(data[top * cols + j], step)) {
          return false;
        }
      }
      ++top;
      for (std::size_t i = top; i < bottom; ++i) {
        if (!decrease(data[i * cols + (right - 1)], step)) {
          return false;
        }
      }
      --right;
      if (top < bottom) {
        for (std::size_t j = right; j > left; --j) {
          if (!decrease(data[(bottom - 1) * cols + (j - 1)], step)) {
            return false;
          }
        }
        --bottom;
      }
      if (left < right) {
        for (std::size_t i = bottom; i > top; --i) {
          if (!decrease(data[(i - 1) * cols + left], step)) {
            return false;
          }
        }
        ++left;
      }
    }
    return true;
  }

  int process(int * data, std::size_t rows, std::size_t cols, std::ostream & output)
  {
    const long long max_sum = max_sum_sdg(data, rows, cols);
    const bool is_err_5 = !lft_top_clk(data, rows, cols);

    output << max_sum << '\n';
    if (is_err_5) {
      std::cerr << "LFT-TOP-CLK error: value overflow\n";
    } else if (!write_matrix(output, data, rows, cols)) {
      std::cerr << "Output write error\n";
      return data_error_code;
    }
    return is_err_5 ? data_error_code : 0;
  }

  int read_and_process(int * data, std::size_t rows, std::size_t cols, std::istream & input, std::ostream & output)
  {
    if (!read_elements(input, data, rows * cols)) {
      std::cerr << "Invalid matrix elements\n";
      return data_error_code;
    }
    return process(data, rows, cols, output);
  }

  int run(int mode, std::istream & input, std::ostream & output)
  {
    std::size_t rows = 0;
    std::size_t cols = 0;
    if (!read_dimensions(input, rows, cols)) {
      std::cerr << "Invalid matrix size\n";
      return data_error_code;
    }
    const std::size_t count = rows * cols;

    if (mode == fixed_mode) {
      if (count > fixed_capacity) {
        std::cerr << "Matrix is too big for fixed array\n";
        return data_error_code;
      }
      int buffer[fixed_capacity];
      return read_and_process(buffer, rows, cols, input, output);
    }

    int * buffer = new (std::nothrow) int[count];
    if (buffer == nullptr) {
      std::cerr << "Not enough memory\n";
      return data_error_code;
    }
    const int code = read_and_process(buffer, rows, cols, input, output);
    delete[] buffer;
    return code;
  }
}

int main(int argc, char ** argv)
{
  if (argc != 4) {
    std::cerr << "Usage: ./lab num input output\n";
    return uzun::arguments_error_code;
  }
  int mode = 0;
  if (!uzun::parse_mode(argv[1], mode)) {
    std::cerr << "First parameter must be 1 or 2\n";
    return uzun::arguments_error_code;
  }
  std::ifstream input(argv[2]);
  if (!input) {
    std::cerr << "Cannot open input file\n";
    return uzun::arguments_error_code;
  }
  std::ofstream output(argv[3]);
  if (!output) {
    std::cerr << "Cannot open output file\n";
    return uzun::arguments_error_code;
  }
  return uzun::run(mode, input, output);
}
