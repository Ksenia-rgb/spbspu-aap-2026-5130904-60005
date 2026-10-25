#include <iostream>
#include <fstream>

namespace laptev {

  int numRowLsr(const int* a, int rows, int cols)
  {
    int bestRow = 0;
    int bestLen = 0;
    for (int i = 0; i < rows; ++i) {
      int curLen = 1;
      int rowBest = (cols > 0) ? 1 : 0;
      for (int j = 1; j < cols; ++j) {
        if (a[i * cols + j] == a[i * cols + j - 1]) {
          ++curLen;
        } else {
          curLen = 1;
        }
        if (curLen > rowBest) {
          rowBest = curLen;
        }
      }
      if (rowBest > bestLen) {
        bestLen = rowBest;
        bestRow = i + 1;
      }
    }
    return bestRow;
  }

  int cntNzrDig(const int* a, int rows, int cols)
  {
    int count = 0;
    for (int d = -(rows - 1); d <= (cols - 1); ++d) {
      if (d == 0) {
        continue;
      }
      bool hasZero = false;
      bool hasElem = false;
      for (int i = 0; i < rows; ++i) {
        int j = i + d;
        if (j < 0 || j >= cols) {
          continue;
        }
        hasElem = true;
        if (a[i * cols + j] == 0) {
          hasZero = true;
          break;
        }
      }
      if (hasElem && !hasZero) {
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
  int modeNum = (mode[0] == '1') ? 1 : 2;

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

  int rows = 0, cols = 0;
  int* data = nullptr;
  static int fixedBuf[10000];

  if (modeNum == 1) {
    if (!(input >> rows >> cols)) {
      std::cerr << "Cannot read matrix sizes\n";
      return 2;
    }
    if (rows < 0 || cols < 0) {
      std::cerr << "Invalid matrix sizes\n";
      return 2;
    }
    long long total = static_cast< long long >(rows) * cols;
    if (total > 10000) {
      std::cerr << "Matrix too large for fixed mode\n";
      return 2;
    }
    for (long long k = 0; k < total; ++k) {
      if (!(input >> fixedBuf[k])) {
        std::cerr << "Cannot read matrix element\n";
        return 2;
      }
    }
    data = fixedBuf;
  } else {
    if (!(input >> rows >> cols)) {
      std::cerr << "Cannot read matrix sizes\n";
      return 2;
    }
    if (rows < 0 || cols < 0) {
      std::cerr << "Invalid matrix sizes\n";
      return 2;
    }
    long long total = static_cast< long long >(rows) * cols;
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

  int r12 = laptev::numRowLsr(data, rows, cols);
  int r18 = laptev::cntNzrDig(data, rows, cols);

  output << r12 << '\n';
  output << r18 << '\n';

  if (modeNum == 2 && data != nullptr) {
    delete[] data;
  }

  return output.good() ? 0 : 2;
}
