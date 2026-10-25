#include <fstream>
#include <iostream>
#include <limits>
#include <new>

namespace evsyukov {

  const int k_arg_error = 1;
  const int k_data_error = 2;
  const long long k_static_limit = 10000;
  const long long k_dynamic_limit = 100000000;

  long long min(long long a, long long b)
  {
    long long min_arg = 0;
    if (a < b) {
      min_arg = a;
    } else {
      min_arg = b;
    }
    return min_arg;
  }

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

  bool fill_inc_wav(int* a, long long rows, long long cols)
  {
    long long layer = 0;
    for (long long i = 0; i < rows; ++i) {
      for (long long j = 0; j < cols; ++j) {
        layer = min(min(i, j), min(rows - 1 - i, cols - 1 - j)) + 1;
        if (a[i * cols + j] > std::numeric_limits< int >::max() - layer) {
          return false;
        }
        a[i * cols + j] += static_cast< int >(layer);
      }
    }
    return true;
  }

  int process(std::ifstream& in, int* a, long long rows, long long cols, const char* out_name)
  {
    for (long long i = 0; i < rows * cols; ++i) {
      if (!(in >> a[i])) {
        std::cerr << "Error arguments matrix\n";
        return k_data_error;
      }
    }
    int dummy = 0;
    if (in >> dummy) {
      std::cerr << "Error arguments matrix\n";
      return k_data_error;
    }
    int count = count_local_max(a, rows, cols);
    std::ofstream out(out_name);
    if (!out) {
      std::cerr << "Cannot open output\n";
      return k_data_error;
    }
    bool is_overflow = !fill_inc_wav(a, rows, cols);
    if (is_overflow) {
      std::cerr << "FLL-INC-WAV error\n";
    } else {
      out << rows << " " << cols;
      for (long long i = 0; i < rows * cols; ++i) {
        out << " " << a[i];
      }
      out << "\n";
    }
    out << count << "\n";
    if (is_overflow) {
      return k_data_error;
    }
    return 0;
  }

  int run(int argc, char** argv)
  {
    if (argc != 4) {
      std::cerr << "Invalid_input\n";
      return k_arg_error;
    }
    const char* num_arg = argv[1];
    if (num_arg[0] == '\0' || num_arg[1] != '\0' || (num_arg[0] != '1' && num_arg[0] != '2')) {
      std::cerr << "Error first parameter\n";
      return k_arg_error;
    }
    int num = num_arg[0] - '0';
    std::ifstream in(argv[2]);
    if (!in) {
      std::cerr << "Invalid_file\n";
      return k_data_error;
    }
    long long rows = 0, cols = 0;
    if (!(in >> rows >> cols) || rows < 0 || cols < 0) {
      std::cerr << "Error rows or cols/over_flow\n";
      return k_data_error;
    }
    if (rows > k_dynamic_limit || cols > k_dynamic_limit) {
      std::cerr << "Error rows or cols\n";
      return k_data_error;
    }
    if (cols > 0 && rows > k_dynamic_limit / cols) {
      std::cerr << "Error rows or cols\n";
      return k_data_error;
    }
    int buf[k_static_limit];
    int* matrix = buf;
    if (num == 1) {
      if (rows * cols > k_static_limit) {
        std::cerr << "Matrix is too large for static array\n";
        return k_data_error;
      }
    } else {
      try {
        matrix = new int[rows * cols];
      } catch (const std::bad_alloc&) {
        std::cerr << "Not enough memory\n";
        return k_data_error;
      }
    }
    int code = process(in, matrix, rows, cols, argv[3]);
    if (num == 2) {
      delete[] matrix;
    }
    return code;
  }
}

int main(int argc, char** argv)
{
  return evsyukov::run(argc, argv);
}
