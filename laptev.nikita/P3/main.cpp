#include <iostream>
#include <fstream>

namespace laptev {

  const int max_fixed_elements = 10000;

  int numRowLsr(const int* a, int rows, int cols)
  {
    int best_row = 0;
    int best_len = 0;
    for (int i = 0; i < rows; ++i) {
      int cur_len = 1;
      int row_best = (cols > 0) ? 1 : 0;
      for (int j = 1; j < cols; ++j) {
        if (a[i * cols + j] == a[i * cols + j - 1]) {
          ++cur_len;
        } else {
          cur_len = 1;
        }
        if (cur_len > row_best) {
          row_best = cur_len;
        }
      }
      if (row_best > best_len) {
        best_len = row_best;
        best_row = i + 1;
      }
    }
    return best_row;
  }

  int cntNzrDig(const int* a, int rows, int cols)
  {
    int count = 0;
    for (int d = -(rows - 1); d <= (cols - 1); ++d) {
      if (d == 0) {
        continue;
      }
      bool has_zero = false;
      bool has_elem = false;
      for (int i = 0; i < rows; ++i) {
        const int j = i + d;
        if (j < 0 || j >= cols) {
          continue;
        }
        has_elem = true;
        if (a[i * cols + j] == 0) {
          has_zero = true;
          break;
        }
      }
      if (has_elem && !has_zero) {
        ++count;
      }
    }
    return count;
  }

}

int main(int argc, char** argv)
{
  if (argc != 4) {
    std::cerr << "Usage: ./lab num input output\n";
    return 1;
  }

  const char* mode = argv[1];
  if (!(mode[0] == '1' && mode[1] == '\0') && !(mode[0] == '2' && mode[1] == '\0')) {
    std::cerr << "First parameter must be 1 or 2\n";
    return 1;
  }
  const int mode_num = (mode[0] == '1') ? 1 : 2;

  std::ifstream input(argv[2]);
  if (!input) {
    std::cerr << "Cannot open input file\n";
    return 2;
  }
  std::ofstream output(argv[3]);
  if (!output) {
    std::cerr << "Cannot open output file\n";
    return 2;
  }

  int rows = 0;
  int cols = 0;
  int* data = nullptr;
  static int fixed_buf[laptev::max_fixed_elements];

  if (mode_num == 1) {
    if (!(input >> rows >> cols)) {
      std::cerr << "Cannot read matrix sizes\n";
      return 2;
    }
    if (rows < 0 || cols < 0) {
      std::cerr << "Invalid matrix sizes\n";
      return 2;
    }
    const long long total = static_cast< long long >(rows) * cols;
    if (total > laptev::max_fixed_elements) {
      std::cerr << "Matrix too large for fixed mode\n";
      return 2;
    }
    for (long long k = 0; k < total; ++k) {
      if (!(input >> fixed_buf[k])) {
        std::cerr << "Cannot read matrix element\n";
        return 2;
      }
    }
    data = fixed_buf;
  } else {
    if (!(input >> rows >> cols)) {
      std::cerr << "Cannot read matrix sizes\n";
      return 2;
    }
    if (rows < 0 || cols < 0) {
      std::cerr << "Invalid matrix sizes\n";
      return 2;
    }
    const long long total = static_cast< long long >(rows) * cols;
    if (total > 0) {
      data = new int[total];
      for (long long k = 0; k < total; ++k) {
        if (!(input >> data[k])) {
          delete[] data;
          std::cerr << "Cannot read matrix element\n";
          return 2;
        }
      }
    }
  }

  const int r12 = laptev::numRowLsr(data, rows, cols);
  const int r18 = laptev::cntNzrDig(data, rows, cols);

  output << r12 << '\n';
  output << r18 << '\n';

  if (mode_num == 2 && data != nullptr) {
    delete[] data;
  }

  return output.good() ? 0 : 2;
}
