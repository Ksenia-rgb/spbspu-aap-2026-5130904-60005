#include <fstream>
#include <iostream>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <algorithm>
namespace evsyukov {

  const int zero = 0;
  const int one = 1;
  const int two = 2;
  const int three = 3;
  const int four = 4;
  const long long static_limit = 10000;
  const long long dynamic_limit = 100000000;
  int count_local_max(const int* a, long long rows, long long cols)
  {
    int count = 0;
    for (long long i = 1; i + 1 < rows; ++i) {
      for (long long j = 1; j + 1 < cols; ++j) {
        bool is_max = true;
        for (int di = -1; di <= 1 && is_max; ++di) {
          for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0) {
              continue;
            }
            if (a[i * cols + j] <= a[(i + di) * cols + (j + dj)]) {
              is_max = false;
              break;
            }
          }
        }
        if (is_max) {
          ++count;
        }
      }
    }
    return count;
  }

  void fill_inc_wav(int* a, long long rows, long long cols)
  {
    long long layer = 0;
    for (long long i = 0; i < rows; ++i) {
      for (long long j = 0; j < cols; ++j) {
        layer = std::min(std::min(i, j), std::min(rows - 1 - i, cols - 1 - j)) + 1;
        if (a[i * cols + j] > std::numeric_limits< int >::max() - layer) {
          throw std::invalid_argument("Integer overflow\n");
        }
        a[i * cols + j] += static_cast< int >(layer);
      }
    }
  }

  int run(int argc, char** argv)
  {
    int num = 0;
    if (argc != four) {
      std::cerr << "Invalid_input\n";
      return 1;
    }
    if (std::strcmp(argv[one], "1") == zero) {
      num = 1;
    } else if (std::strcmp(argv[one], "2") == zero) {
      num = 2;
    } else {
      std::cerr << "Invalid num\n";
      return 1;
    }
    std::ifstream in(argv[two]);
    if (!(in)) {
      std::cerr << "Invalid_file\n";
      return 2;
    }
    long long rows = 0, cols = 0;
    if (!(in >> rows >> cols) || rows < zero || cols < zero) {
      std::cerr << "Error rows or cols/over_flow\n";
      return 2;
    }
    if (cols > zero && rows > dynamic_limit / cols) {
      std::cerr << "Error rows or cols\n";
      return 2;
    }
    int buf[static_limit];
    int* a = nullptr;
    if (num == one) {
      a = buf;
    }
    if (num == one && rows * cols > static_limit) {
      std::cerr << "Over_flow\n";
      return 2;
    }
    if (num == two) {
      a = new int[rows * cols];
    }
    for (long long i = 0; i < rows * cols; ++i) {
      int x = 0;
      if (!(in >> x)) {
        std::cerr << "Error arguments matrix\n";
        if (num == two) {
          delete[] a;
        }
        return 2;
      }
      a[i] = x;
    }
    int dummy = 0;
    if (in >> dummy) {
      std::cerr << "Error arguments matrix\n";
      if (num == two) {
        delete[] a;
      }
      return 2;
    }
    int count = count_local_max(a, rows, cols);
    try {
      fill_inc_wav(a, rows, cols);
    } catch (const std::invalid_argument&) {
      std::cerr << "Error: integer overflow\n";
      if (num == two) {
        delete[] a;
      }
      return 2;
    }
    std::ofstream out(argv[three]);
    if (!out) {
      std::cerr << "Cannot open output\n";
      if (num == two) {
        delete[] a;
      }
      return 2;
    }
    out << count << "\n" << rows << " " << cols;
    for (long long i = 0; i < rows * cols; ++i) {
      out << " " << a[i];
    }
    if (num == two) {
      delete[] a;
    }
    return 0;
  }
}

int main(int argc, char** argv)
{
  return evsyukov::run(argc, argv);
}
