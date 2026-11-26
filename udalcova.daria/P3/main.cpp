#include <iostream>
#include <fstream>
#include <cstddef>

namespace udalcova {
  const int error_args = 1;
  const int error_input = 2;
  const int expected_args = 4;
  const size_t max_size = 10000;
  const char fixed_mode = '1';
  const char dynamic_mode = '2';

  bool good(const int* a, size_t r, size_t c, size_t i, size_t j)
  {
    size_t len = 0;
    for (; i < r && j < c; ++i, ++j, ++len) {
      if (a[i * c + j] == 0) {
        return false;
      }
    }
    return len >= 2;
  }

  size_t count(const int* a, size_t r, size_t c)
  {
    size_t cnt = 0;
    for (size_t i = 0; i < r; i++) {
      cnt += good(a, r, c, i, 0);
    }
    for (size_t j = 1; j < c; ++j) {
      cnt += good(a, r, c, 0, j);
    }
    return cnt;
  }

  void spiral(int* a, size_t r, size_t c)
  {
    size_t top = 0, bot = r, left = 0, right = c;
    int k = 1;
    while (top < bot && left < right) {
      for (size_t i = top; i < bot; ++i) {
        a[i * c + left] += k++;
      }
      ++left;
      for (size_t j = left; j < right; j++) {
        a[(bot - 1) * c + j] += ++k;
      }
      --bot;
      if (left < right) {
        for (size_t i = bot; i > top; --i) {
          a[(i - 1) * c + right - 1] += k++;
        }
        --right;
      }
      if (top < bot) {
        for (size_t j = right; j > left; --j) {
          a[top * c + j - 1] += k++;
        }
        ++top;
      }
    }
  }

  bool process(int* a, size_t r, size_t c, std::ostream& out)
  {
    out << count(a, r, c) << '\n';
    spiral(a, r, c);
    out << r << ' ' << c;
    for (size_t i = 0; i < r * c; ++i) {
      out << ' ' << a[i];
    }
    out << '\n';
    return out.good();
  }

  int solve(std::istream& in, std::ostream& out, int* a, size_t r, size_t c)
  {
    for (size_t i = 0; i < r * c; ++i) {
      if (!(in >> a[i])) {
        std::cerr << "Ошибка: содержимое не является матрицей\n";
        return error_input;
      }
    }
    if (!process(a, r, c, out)) {
      std::cerr << "Ошибка: не удалось записать результат\n";
      return error_input;
    }
    return 0;
  }

  int runStatic(std::istream& in, std::ostream& out, size_t r, size_t c)
  {
    int fixed[max_size];
    return solve(in, out, fixed, r, c);
  }

  int runDynamic(std::istream& in, std::ostream& out, size_t r, size_t c)
  {
    int* const a = new int[r * c];
    const int result = solve(in, out, a, r, c);
    delete[] a;
    return result;
  }

  int run(std::istream& in, std::ostream& out, bool dynamic)
  {
    size_t r = 0, c = 0;
    if (!(in >> r >> c)) {
      std::cerr << "Ошибка: не удалось прочитать размер матрицы\n";
      return error_input;
    }
    if (c != 0 && r > max_size / c) {
      std::cerr << "Ошибка: матрица слишком большая\n";
      return error_input;
    }
    if (dynamic) {
      return runDynamic(in, out, r, c);
    }
    return runStatic(in, out, r, c);
  }
}

int main(int argc, char** argv)
{
  using namespace udalcova;

  if (argc != expected_args) {
    std::cerr << "Ошибка: неверное количество аргументов\n";
    return error_input;
  }

  const char* const num = argv[1];
  if ((num[0] != fixed_mode && num[0] != dynamic_mode) || num[1] != '\0') {
    std::cerr << "Ошибка: первый параметр должен быть 1 или 2\n";
    return error_args;
  }

  std::ifstream in(argv[2]);
  if (!in) {
    std::cerr << "Ошибка: не удалось открыть входной файл\n";
    return error_args;
  }
  std::ofstream out(argv[3]);
  if (!out) {
    std::cerr << "Ошибка: не удалось открыть входной файл\n";
    return error_args;
  }

  return run(in, out, num[0] == dynamic_mode);
}
