#include <fstream>
#include <iostream>

namespace demyanov {
  const int ERR_ARGS = 1;
  const int ERR_MATRIX = 2;
  const int ARGS_COUNT = 4;
  const int MAX_SIZE = 10000;

  bool readMatrix(std::istream& input, int* data, int rows, int cols)
  {
    for (int i = 0; i < rows * cols; ++i) {
      if (!(input >> data[i])) {
        return false;
      }
    }
    return true;
  }

  int findRowWithLongestRun(const int* data, int rows, int cols)
  {
    int best_row = 0;
    int best_len = 0;
    for (int i = 0; i < rows; ++i) {
      int len = 0;
      for (int j = 0; j < cols; ++j) {
        if (j > 0 && data[i * cols + j] == data[i * cols + j - 1]) {
          ++len;
        } else {
          len = 1;
        }
        if (len > best_len) {
          best_len = len;
          best_row = i + 1;
        }
      }
    }
    return best_row;
  }

  void spiral(int* data, int rows, int cols)
  {
    int top = 0;
    int bottom = rows - 1;
    int left = 0;
    int right = cols - 1;
    int step = 1;

    while (top <= bottom && left <= right) {
      for (int i = top; i <= bottom; ++i) {
        data[i * cols + left] += step;
        ++step;
      }
      ++left;

      for (int j = left; j <= right; ++j) {
        data[bottom * cols + j] += step;
        ++step;
      }
      --bottom;

      if (left <= right) {
        for (int i = bottom; i >= top; --i) {
          data[i * cols + right] += step;
          ++step;
        }
        --right;
      }

      if (top <= bottom) {
        for (int j = right; j >= left; --j) {
          data[top * cols + j] += step;
          ++step;
        }
        ++top;
      }
    }
  }

  void writeMatrix(std::ostream& output, const int* data, int rows, int cols)
  {
    output << rows << " " << cols;
    for (int i = 0; i < rows * cols; ++i) {
      output << " " << data[i];
    }
    output << "\n";
  }

  void solveMatrix(std::ostream& output, int* data, int rows, int cols)
  {
    const int row = findRowWithLongestRun(data, rows, cols);
    spiral(data, rows, cols);
    writeMatrix(output, data, rows, cols);
    output << row << "\n";
  }

  int run(int argc, char** argv)
  {
    if (argc != ARGS_COUNT) {
      std::cerr << "Error: need 3 arguments: num input output" << "\n";
      return ERR_ARGS;
    }

    const bool is_fixed = (argv[1][0] == '1' && argv[1][1] == '\0');
    const bool is_dynamic = (argv[1][0] == '2' && argv[1][1] == '\0');
    if (!is_fixed && !is_dynamic) {
      std::cerr << "Error: num is not 1 or 2" << "\n";
      return ERR_ARGS;
    }

    std::ifstream input{argv[2]};
    if (!input) {
      std::cerr << "Error: can't open input file" << "\n";
      return ERR_ARGS;
    }
    std::ofstream output{argv[3]};
    if (!output) {
      std::cerr << "Error: can't open output file" << "\n";
      return ERR_ARGS;
    }

    int rows = 0;
    int cols = 0;
    if (!(input >> rows >> cols) || rows < 0 || cols < 0) {
      std::cerr << "Error: bad matrix size" << "\n";
      return ERR_MATRIX;
    }

    if (is_fixed) {
      int fixed_data[MAX_SIZE] = {};
      if (!readMatrix(input, fixed_data, rows, cols)) {
        std::cerr << "Error: bad matrix elements" << "\n";
        return ERR_MATRIX;
      }
      solveMatrix(output, fixed_data, rows, cols);
      return 0;
    }

    int* const dynamic_data = new int[rows * cols];
    if (!readMatrix(input, dynamic_data, rows, cols)) {
      std::cerr << "Error: bad matrix elements" << "\n";
      delete[] dynamic_data;
      return ERR_MATRIX;
    }
    solveMatrix(output, dynamic_data, rows, cols);
    delete[] dynamic_data;
    return 0;
  }
}

int main(int argc, char** argv)
{
  return demyanov::run(argc, argv);
}
