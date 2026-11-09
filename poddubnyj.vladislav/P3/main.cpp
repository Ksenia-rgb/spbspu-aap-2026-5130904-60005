#include <iostream>
#include <fstream>
#include <cstddef>
#include <stdexcept>
#include <limits>

namespace poddubnyj {
  const int INCORRECT_ARGS = 1;
  const int INCORRECT_MATRIX = 2;
  const int FIXED_MODE = 1;
  const std::size_t FIXED_SIZE = 10000;

  void readMatrix(std::istream &input, int *matrix, std::size_t n);
  bool checkLowerTriangular(const int *matrix, std::size_t cols, std::size_t size);
  void reduceSpiralMatrix(int *matrix, std::size_t rows, std::size_t cols);
  void writeMatrix(std::ostream &out, const int *matrix, std::size_t rows, std::size_t cols);
}

void poddubnyj::readMatrix(std::istream &input, int *matrix, std::size_t n)
{
  for (std::size_t i = 0; i < n; ++i) {
    if (!(input >> matrix[i])) {
      throw std::invalid_argument("incorrect matrix element");
    }
  }

  char extra = 0;
  if (input >> extra) {
    throw std::invalid_argument("extra data after matrix");
  }
}

bool poddubnyj::checkLowerTriangular(const int *matrix, std::size_t cols, std::size_t size)
{
  if (size == 0) {
    return false;
  }

  for (std::size_t i = 0; i < size; ++i) {
    for (std::size_t j = i + 1; j < size; ++j) {
      if (matrix[i * cols + j] != 0) {
        return false;
      }
    }
  }

  return true;
}

void poddubnyj::reduceSpiralMatrix(int *matrix, std::size_t rows, std::size_t cols)
{
  std::size_t left = 0;
  std::size_t right = cols;
  std::size_t top = 0;
  std::size_t bottom = rows;
  int counter = 1;

  while (left < right && top < bottom) {
    for (std::size_t i = left; i < right; ++i) {
      matrix[top * cols + i] -= counter++;
    }

    ++top;

    for (std::size_t j = top; j < bottom; ++j) {
      matrix[j * cols + (right - 1)] -= counter++;
    }

    --right;

    if (top < bottom) {
      for (std::size_t k = right; k > left; --k) {
        matrix[(bottom - 1) * cols + (k - 1)] -= counter++;
      }

      --bottom;
    }

    if (left < right) {
      for (std::size_t i = bottom; i > top; --i) {
        matrix[(i - 1) * cols + left] -= counter++;
      }

      ++left;
    }
  }
}

void poddubnyj::writeMatrix(std::ostream &out, const int *matrix, std::size_t rows, std::size_t cols)
{
  out << rows << ' ' << cols << '\n';

  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < cols; ++j) {
      out << matrix[i * cols + j];
      if (j + 1 < cols) {
        out << ' ';
      }
    }

    out << '\n';
  }
}

int main(int argc, char **argv)
{
  if (argc != 4) {
    std::cerr << "error: incorrect arguments\n";
    return poddubnyj::INCORRECT_ARGS;
  }

  const char *const mode = argv[1];
  if (mode[0] == '\0' || mode[1] != '\0' || (mode[0] != '1' && mode[0] != '2')) {
    std::cerr << "error: incorrect mode\n";
    return poddubnyj::INCORRECT_ARGS;
  }

  const int modeNum = mode[0] - '0';

  std::ifstream input{argv[2]};
  if (!input) {
    std::cerr << "error: cannot open input file\n";
    return poddubnyj::INCORRECT_ARGS;
  }

  std::ofstream output{argv[3]};
  if (!output) {
    std::cerr << "error: cannot open output file\n";
    return poddubnyj::INCORRECT_ARGS;
  }

  long long rowsLL = 0;
  long long colsLL = 0;
  if (!(input >> rowsLL >> colsLL) || rowsLL < 0 || colsLL < 0) {
    std::cerr << "error: incorrect matrix\n";
    return poddubnyj::INCORRECT_MATRIX;
  }

  const std::size_t rows = static_cast< std::size_t >(rowsLL);
  const std::size_t cols = static_cast< std::size_t >(colsLL);

  const std::size_t maxSize = std::numeric_limits< std::size_t >::max();
  if (cols != 0 && rows > maxSize / cols) {
    std::cerr << "error: matrix too large\n";
    return poddubnyj::INCORRECT_MATRIX;
  }

  const std::size_t total = rows * cols;

  int fixedMatrix[poddubnyj::FIXED_SIZE] = {};
  int *matrix = nullptr;
  bool dynamic = false;

  if (modeNum == poddubnyj::FIXED_MODE) {
    if (total > poddubnyj::FIXED_SIZE) {
      std::cerr << "error: matrix too large for fixed mode\n";
      return poddubnyj::INCORRECT_MATRIX;
    }

    matrix = fixedMatrix;
  } else {
    if (total > 0) {
      try {
        matrix = new int[total];
        dynamic = true;
      } catch (const std::bad_alloc &) {
        std::cerr << "error: cannot allocate matrix\n";
        return poddubnyj::INCORRECT_MATRIX;
      }
    }
  }

  try {
    if (total > 0) {
      poddubnyj::readMatrix(input, matrix, total);
    } else {
      char extra = 0;
      if (input >> extra) {
        throw std::invalid_argument("extra data after matrix");
      }
    }
  } catch (const std::invalid_argument &e) {
    if (dynamic) {
      delete[] matrix;
    }

    std::cerr << "error: " << e.what() << '\n';
    return poddubnyj::INCORRECT_MATRIX;
  }

  std::size_t size = 0;
  if (rows < cols) {
    size = rows;
  } else {
    size = cols;
  }

  const bool isLower = poddubnyj::checkLowerTriangular(matrix, cols, size);
  if (isLower) {
    output << "true\n";
  } else {
    output << "false\n";
  }

  if (total > 0) {
    poddubnyj::reduceSpiralMatrix(matrix, rows, cols);
  }

  poddubnyj::writeMatrix(output, matrix, rows, cols);

  if (dynamic) {
    delete[] matrix;
  }

  return 0;
}
