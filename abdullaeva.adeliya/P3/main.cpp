#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <new>

namespace abdullaeva {
  const int arguments_error_code = 1;
  const int data_error_code = 2;
  const int fixed_mode = 1;
  const int dynamic_mode = 2;
  const std::size_t fixed_capacity = 10000;

  bool parseMode(const char * text, int & mode)
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

  bool readSize(std::istream & input, std::size_t & value)
  {
    long long number = 0;
    if (!(input >> number) || number < 0) {
      return false;
    }
    value = static_cast< std::size_t >(number);
    return true;
  }

  bool readDimensions(std::istream & input, std::size_t & rows, std::size_t & cols)
  {
    if (!readSize(input, rows) || !readSize(input, cols)) {
      return false;
    }
    const std::size_t max_size = std::numeric_limits< std::size_t >::max();
    return rows == 0 || cols <= max_size / rows;
  }

  bool readElements(std::istream & input, int * data, std::size_t count)
  {
    for (std::size_t i = 0; i < count; ++i) {
      if (!(input >> data[i])) {
        return false;
      }
    }
    return true;
  }

  bool writeMatrix(std::ostream & output, const int * data, std::size_t rows, std::size_t cols)
  {
    output << rows << ' ' << cols;
    for (std::size_t i = 0; i < rows * cols; ++i) {
      output << ' ' << data[i];
    }
    output << '\n';
    return output.good();
  }

  std::size_t cntColNsm(const int * data, std::size_t rows, std::size_t cols)
  {
    if (rows == 0) {
      return 0;
    }
    std::size_t count = 0;
    for (std::size_t j = 0; j < cols; ++j) {
      bool has_repeat = false;
      for (std::size_t i = 1; i < rows && !has_repeat; ++i) {
        has_repeat = (data[i * cols + j] == data[(i - 1) * cols + j]);
      }
      if (!has_repeat) {
        ++count;
      }
    }
    return count;
  }

  bool fllIncWav(int * data, std::size_t rows, std::size_t cols)
  {
    for (std::size_t i = 0; i < rows; ++i) {
      for (std::size_t j = 0; j < cols; ++j) {
        const std::size_t layer = std::min(std::min(i, rows - 1 - i), std::min(j, cols - 1 - j));
        const long long next = static_cast< long long >(data[i * cols + j]) + static_cast< long long >(layer + 1);
        if (next > std::numeric_limits< int >::max()) {
          return false;
        }
        data[i * cols + j] = static_cast< int >(next);
      }
    }
    return true;
  }

  int process(int * data, std::size_t rows, std::size_t cols, std::ostream & output)
  {
    const std::size_t columns = cntColNsm(data, rows, cols);
    const bool is_err_8 = !fllIncWav(data, rows, cols);

    output << columns << '\n';
    if (is_err_8) {
      std::cerr << "FLL-INC-WAV error: value overflow\n";
    } else if (!writeMatrix(output, data, rows, cols)) {
      std::cerr << "Output write error\n";
      return data_error_code;
    }
    return is_err_8 ? data_error_code : 0;
  }

  int readAndProcess(int * data, std::size_t rows, std::size_t cols, std::istream & input, std::ostream & output)
  {
    if (!readElements(input, data, rows * cols)) {
      std::cerr << "Invalid matrix elements\n";
      return data_error_code;
    }
    return process(data, rows, cols, output);
  }

  int run(int mode, std::istream & input, std::ostream & output)
  {
    std::size_t rows = 0;
    std::size_t cols = 0;
    if (!readDimensions(input, rows, cols)) {
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
      return readAndProcess(buffer, rows, cols, input, output);
    }

    int * const buffer = new (std::nothrow) int[count];
    if (buffer == nullptr) {
      std::cerr << "Not enough memory\n";
      return data_error_code;
    }
    const int code = readAndProcess(buffer, rows, cols, input, output);
    delete[] buffer;
    return code;
  }
}

int main(int argc, char ** argv)
{
  if (argc != 4) {
    std::cerr << "Usage: ./lab num input output\n";
    return abdullaeva::arguments_error_code;
  }
  int mode = 0;
  if (!abdullaeva::parseMode(argv[1], mode)) {
    std::cerr << "First parameter must be 1 or 2\n";
    return abdullaeva::arguments_error_code;
  }
  std::ifstream input(argv[2]);
  if (!input) {
    std::cerr << "Cannot open input file\n";
    return abdullaeva::arguments_error_code;
  }
  std::ofstream output(argv[3]);
  if (!output) {
    std::cerr << "Cannot open output file\n";
    return abdullaeva::arguments_error_code;
  }
  return abdullaeva::run(mode, input, output);
}
