#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

namespace evsyukov {

  const int k_arg_error = 1;
  const int k_data_error = 2;

  constexpr long long k_max_matrix_elements = 100000000;
  constexpr long long k_static_buffer_size = 10000;

  long long minArg(long long a, long long b)
  {
    return (a < b) ? a : b;
  }

  int countLocalMax(const int* a, long long rows, long long cols)
  {
    if (rows < 3 || cols < 3 || a == nullptr) {
      return 0;
    }
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

  bool fillIncWav(int* a, long long rows, long long cols)
  {
    if (a == nullptr || rows <= 0 || cols <= 0) {
      return true;
    }
    long long layer = 0;
    for (long long i = 0; i < rows; ++i) {
      for (long long j = 0; j < cols; ++j) {
        layer = minArg(minArg(i, j), minArg(rows - 1 - i, cols - 1 - j)) + 1;
        if (a[i * cols + j] > std::numeric_limits< int >::max() - layer) {
          return false;
        }
        a[i * cols + j] += static_cast< int >(layer);
      }
    }
    return true;
  }

  int run(int argc, char** argv)
  {
    int num = 0;
    if (argc != 4) {
      std::cerr << "Invalid_input\n";
      return k_arg_error;
    }

    const char* const s = argv[1];
    if (s[0] != '\0' && s[1] == '\0' && (s[0] == '1' || s[0] == '2')) {
      num = argv[1][0] - '0';
    } else {
      std::cerr << "Error first parameter \n";
      return k_arg_error;
    }

    std::ifstream in(argv[2]);
    if (!(in)) {
      std::cerr << "Invalid_file\n";
      return k_data_error;
    }

    long long rows = 0, cols = 0;
    if (!(in >> rows >> cols) || rows <= 0 || cols <= 0) {
      std::cerr << "Error rows or cols/over_flow\n";
      return k_data_error;
    }

    if (rows > k_max_matrix_elements / cols) {
      std::cerr << "Error rows or cols\n";
      return k_data_error;
    }

    int buf[k_static_buffer_size];
    std::vector< int > dynamic_buf;
    int* a = nullptr;

    if (num == 1) {
      if (rows * cols > k_static_buffer_size) {
        std::cerr << "Over_flow\n";
        return k_data_error;
      }
      a = buf;
    } else if (num == 2) {
      try {
        dynamic_buf.resize(rows * cols);
        a = dynamic_buf.data();
      } catch (const std::bad_alloc&) {
        std::cerr << "Memory allocation failed\n";
        return k_data_error;
      }
    }

    for (long long i = 0; i < rows * cols; ++i) {
      int x = 0;
      if (!(in >> x)) {
        std::cerr << "Error arguments matrix\n";
        return k_data_error;
      }
      a[i] = x;
    }

    int dummy = 0;
    if (in >> dummy) {
      std::cerr << "Error arguments matrix\n";
      return k_data_error;
    }

    const int count = countLocalMax(a, rows, cols);
    std::ofstream out(argv[3]);
    if (!out) {
      std::cerr << "Cannot open output\n";
      return k_data_error;
    }

    const bool is_overflow = !fillIncWav(a, rows, cols);
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
}

int main(int argc, char** argv)
{
  return evsyukov::run(argc, argv);
}

