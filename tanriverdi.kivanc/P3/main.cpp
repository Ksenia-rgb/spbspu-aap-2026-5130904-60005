#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <new>

namespace tanriverdikivanc {
  const int argument_error = 1;
  const int input_error = 2;
  const int max_fixed_elements = 10000;

  bool readMatrix(std::istream& input, long long* matrix, long long count)
  {
    for (long long i = 0; i < count; ++i) {
      if (!(input >> matrix[i])) {
        return false;
      }
    }

    char extra = '\0';
    if (input >> extra) {
      return false;
    }

    return true;
  }

  void transformMatrix(long long* matrix, long long rows, long long cols)
  {
    long long top = 0;
    long long bottom = rows - 1;
    long long left = 0;
    long long right = cols - 1;
    long long step = 1;

    while (top <= bottom && left <= right) {
      for (long long col = left; col <= right; ++col) {
        matrix[top * cols + col] -= step++;
      }
      ++top;

      for (long long row = top; row <= bottom; ++row) {
        matrix[row * cols + right] -= step++;
      }
      --right;

      if (top <= bottom) {
        for (long long col = right; col >= left; --col) {
          matrix[bottom * cols + col] -= step++;
        }
        --bottom;
      }

      if (left <= right) {
        for (long long row = bottom; row >= top; --row) {
          matrix[row * cols + left] -= step++;
        }
        ++left;
      }
    }
  }

  long long minSecondaryParallelSum(const long long* matrix, long long rows, long long cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }

    bool has_result = false;
    long long minimum = 0;

    for (long long diagonal = 0; diagonal <= rows + cols - 2; ++diagonal) {
      if (diagonal == cols - 1) {
        continue;
      }

      long long sum = 0;

      for (long long row = 0; row < rows; ++row) {
        const long long col = diagonal - row;

        if (col >= 0 && col < cols) {
          sum += matrix[row * cols + col];
        }
      }

      if (!has_result || sum < minimum) {
        minimum = sum;
        has_result = true;
      }
    }

    return has_result ? minimum : 0;
  }

  bool writeResults(
      std::ostream& output, const long long* original, long long rows, long long cols, long long* transformed)
  {
    const long long count = rows * cols;

    for (long long i = 0; i < count; ++i) {
      transformed[i] = original[i];
    }

    transformMatrix(transformed, rows, cols);

    output << rows << ' ' << cols;

    for (long long i = 0; i < count; ++i) {
      output << ' ' << transformed[i];
    }

    output << '\n';
    output << minSecondaryParallelSum(original, rows, cols) << '\n';

    return output.good();
  }

  int run(int argc, char** argv)
  {
    if (argc != 4) {
      std::cerr << "Error: expected num, input and output arguments\n";
      return argument_error;
    }

    if (std::strcmp(argv[1], "1") != 0 && std::strcmp(argv[1], "2") != 0) {
      std::cerr << "Error: num must be 1 or 2\n";
      return argument_error;
    }

    const bool fixed_mode = std::strcmp(argv[1], "1") == 0;

    std::ifstream input(argv[2]);
    if (!input) {
      std::cerr << "Error: cannot open input file\n";
      return argument_error;
    }

    std::ofstream output(argv[3]);
    if (!output) {
      std::cerr << "Error: cannot open output file\n";
      return argument_error;
    }

    long long rows = 0;
    long long cols = 0;

    if (!(input >> rows >> cols) || rows < 0 || cols < 0 || ((rows == 0) != (cols == 0))
        || (cols != 0 && rows > std::numeric_limits< long long >::max() / cols)) {
      std::cerr << "Error: invalid matrix dimensions\n";
      return input_error;
    }

    const long long count = rows * cols;

    if (fixed_mode && count > max_fixed_elements) {
      std::cerr << "Error: matrix is too large for fixed mode\n";
      return input_error;
    }

    if (fixed_mode) {
      long long matrix[max_fixed_elements];
      long long transformed[max_fixed_elements];

      if (!readMatrix(input, matrix, count)) {
        std::cerr << "Error: invalid matrix data\n";
        return input_error;
      }

      if (!writeResults(output, matrix, rows, cols, transformed)) {
        std::cerr << "Error: cannot write output\n";
        return argument_error;
      }
    } else {
      long long* const matrix = new (std::nothrow) long long[count];
      long long* const transformed = new (std::nothrow) long long[count];

      if (count > 0 && (matrix == nullptr || transformed == nullptr)) {
        delete[] matrix;
        delete[] transformed;
        std::cerr << "Error: memory allocation failed\n";
        return input_error;
      }

      if (!readMatrix(input, matrix, count)) {
        delete[] matrix;
        delete[] transformed;
        std::cerr << "Error: invalid matrix data\n";
        return input_error;
      }

      const bool written = writeResults(output, matrix, rows, cols, transformed);

      delete[] matrix;
      delete[] transformed;

      if (!written) {
        std::cerr << "Error: cannot write output\n";
        return argument_error;
      }
    }

    return 0;
  }
}

int main(int argc, char** argv)
{
  return tanriverdikivanc::run(argc, argv);
}
