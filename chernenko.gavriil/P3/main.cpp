#include <cstddef>
#include <fstream>
#include <iostream>
#include <istream>
#include <limits>
#include <new>
#include <stdexcept>

namespace chernenko {
  const int invalid_program_argument_ec = 1;
  const int invalid_file_ec = 2;
  const int good_ec = 0;

  const std::size_t max_elements = 10000;
  const int max_int = std::numeric_limits< int >::max();
  const int min_int = std::numeric_limits< int >::min();
  const std::size_t max_sizet = std::numeric_limits< std::size_t >::max();
  const int arguments_count_value = 4;
  const char arg_stack_value = '1';
  const char arg_dynamic_value = '2';

  void runStack(std::istream& input, std::ostream& out);
  void runDynamic(std::istream& input, std::ostream& out);
  void readMatrix(std::istream& input, int* matrix, std::size_t size);
  void checkEOF(std::istream& input);
  std::size_t countColumnsNotsame(std::size_t n, std::size_t m, const int* matrix);
  int minSumParallMaind(std::size_t n, std::size_t m, const int* matrix);
  int min(int a, int b);
  std::size_t min(std::size_t a, std::size_t b);
  void add(int&, int);
}

int main(int argc, char** argv)
{
  if (argc > chernenko::arguments_count_value) {
    std::cerr << "Too many arguments" << std::endl;
    return chernenko::invalid_program_argument_ec;
  }
  if (argc < chernenko::arguments_count_value) {
    std::cerr << "Not enough arguments" << std::endl;
    return chernenko::invalid_program_argument_ec;
  }
  if (argv[1][0] < '0' || argv[1][0] > '9' || (argv[1][0] != 0 && argv[1][1] != 0)) {
    std::cerr << "First parameter is not a number" << std::endl;
    return chernenko::invalid_program_argument_ec;
  }
  if (argv[1][0] < chernenko::arg_stack_value || argv[1][0] > chernenko::arg_dynamic_value) {
    std::cerr << "First parameter is out of range" << std::endl;
    return chernenko::invalid_program_argument_ec;
  }
  bool is_dynamic = false;
  if (argv[1][0] == chernenko::arg_dynamic_value) {
    is_dynamic = true;
  }
  std::ifstream in_file(argv[2]);
  if (!in_file) {
    std::cerr << "Incorrect input file" << std::endl;
    return chernenko::invalid_program_argument_ec;
  }
  std::ofstream out_file(argv[3]);
  if (!out_file) {
    std::cerr << "Incorrect output file" << std::endl;
    return chernenko::invalid_program_argument_ec;
  }
  try {
    if (is_dynamic) {
      chernenko::runDynamic(in_file, out_file);
    } else {
      chernenko::runStack(in_file, out_file);
    }
  } catch (const std::overflow_error& e) {
    std::cerr << "Overflow error: " << e.what() << std::endl;
    return chernenko::invalid_file_ec;
  } catch (const std::underflow_error& e) {
    std::cerr << "Underflow error: " << e.what() << std::endl;
    return chernenko::invalid_file_ec;
  } catch (const std::runtime_error& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
    return chernenko::invalid_file_ec;
  } catch (const std::bad_alloc& e) {
    std::cerr << "Allocation error: " << e.what() << std::endl;
    return chernenko::invalid_file_ec;
  }
  return chernenko::good_ec;
}

void chernenko::runStack(std::istream& input, std::ostream& out)
{
  std::size_t rows = 0, cols = 0;
  if (!(input >> rows >> cols)) {
    throw std::runtime_error("Incorrect file content");
  }
  if (cols != 0 && rows > max_elements / cols) {
    throw std::runtime_error("Incorrect file content");
  }
  if (rows * cols == 0 && rows != cols) {
    throw std::runtime_error("Incorrect file content");
  }
  int matrix[max_elements]{};
  std::size_t spec1 = 0;
  int spec2 = 0;
  readMatrix(input, matrix, rows * cols);
  checkEOF(input);
  spec1 = countColumnsNotsame(rows, cols, matrix);
  spec2 = minSumParallMaind(rows, cols, matrix);
  out << spec1 << " " << spec2 << std::endl;
}

void chernenko::runDynamic(std::istream& input, std::ostream& out)
{
  std::size_t rows = 0, cols = 0;
  if (!(input >> rows >> cols)) {
    throw std::runtime_error("Incorrect file content");
  }
  if (cols != 0 && rows > max_sizet / cols) {
    throw std::runtime_error("Incorrect file content");
  }
  if (rows * cols == 0 && rows != cols) {
    throw std::runtime_error("Incorrect file content");
  }
  int* const matrix = new int[rows * cols]{};
  std::size_t spec1 = 0;
  int spec2 = 0;
  try {
    readMatrix(input, matrix, rows * cols);
    checkEOF(input);
    spec1 = countColumnsNotsame(rows, cols, matrix);
    spec2 = minSumParallMaind(rows, cols, matrix);
  } catch (...) {
    delete[] matrix;
    throw;
  }
  delete[] matrix;
  out << spec1 << " " << spec2 << std::endl;
}

void chernenko::readMatrix(std::istream& input, int* matrix, std::size_t size)
{
  for (std::size_t i = 0; i < size; i++) {
    if (!(input >> matrix[i])) {
      throw std::runtime_error("Incorrect file content");
    }
  }
}

void chernenko::checkEOF(std::istream& input)
{
  char rubbish = ' ';
  if (input >> rubbish) {
    throw std::runtime_error("Incorrect file content");
  }
}

std::size_t chernenko::countColumnsNotsame(std::size_t n, std::size_t m, const int* matrix)
{
  std::size_t count = 0;
  for (std::size_t i = 0; i < m; i++) {
    bool flag = true;
    for (std::size_t j = 1; j < n; j++) {
      if (matrix[(j - 1) * m + i] == matrix[j * m + i]) {
        flag = false;
        break;
      }
    }
    if (flag) {
      count++;
    }
  }
  return count;
}

int chernenko::minSumParallMaind(std::size_t n, std::size_t m, const int* matrix)
{
  const std::size_t len = min(n, m);
  if (len < 2) {
    return 0;
  }
  int res = std::numeric_limits< int >::max();
  for (std::size_t d = 1; d < len; d++) {
    int cur = 0;
    for (std::size_t j = d, i = 0; j < len; j++, i++) {
      add(cur, matrix[j * m + i]);
    }
    res = min(res, cur);
    cur = 0;
    for (std::size_t j = 0, i = d; i < len; j++, i++) {
      add(cur, matrix[j * m + i]);
    }
    res = min(res, cur);
  }
  return res;
}

int chernenko::min(int a, int b)
{
  if (a < b) {
    return a;
  }
  return b;
}

std::size_t chernenko::min(std::size_t a, std::size_t b)
{
  if (a < b) {
    return a;
  }
  return b;
}

void chernenko::add(int& a, int b)
{
  if (a > 0 && b > 0 && a > max_int - b) {
    throw std::overflow_error("Too big int value");
  }
  if (a < 0 && b < 0 && a < min_int - b) {
    throw std::underflow_error("Too small int value");
  }
  a += b;
}
