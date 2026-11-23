#include <iostream>
#include <fstream>
#include <stdexcept>
#include <limits>

namespace kuznetsov
{

  int* fsForMatrix(size_t k_rows, size_t k_colomn)
  {
    if (k_rows == 0 || k_colomn == 0)
    {
      throw(std::invalid_argument("invalid_argument\n"));
    }
    try
    {
      return new int[k_rows * k_colomn];
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "bad_alloc\n";
      return nullptr;
    }
  }

  void dataCp(std::istream& input, int* matrix, size_t k_row, size_t k_column)
  {
    char last_symb{};
    for (size_t i = 0; i < k_row * k_column; ++i)
    {
      if (!(input >> matrix[i]))
      {
        throw std::invalid_argument("invalid arg\n");
      }
    }
    if (input >> last_symb)
    {
      throw std::invalid_argument("invalid_arg\n");
    }
  }
  int numRowLsr(const int* matrix, size_t k_row, size_t k_colomn)
  {
    int max_ser = 0, row_res = 0;
    for (size_t r = 0; r < k_row; ++r)
    {
      int now_res = 1;
      int max_res = 1;
      for (size_t c = 1; c < k_colomn; ++c)
      {
        const size_t i_el_now = r * k_colomn + c;
        const size_t i_prev_el = r * k_colomn + (c - 1);

        if (matrix[i_el_now] == matrix[i_prev_el])
        {
          now_res++;
        }
        else
        {
          if (now_res > max_res)
          {
            max_res = now_res;
          }
          now_res = 1;
        }
      }

      if (now_res > max_res)
      {
        max_res = now_res;
      }

      if (max_res > max_ser)
      {
        max_ser = max_res;
        row_res = r;
      }
    }

    return row_res + 1;
  }

  int minSumSdg(const int* matrix, size_t k_row, size_t k_colomn)
  {
    if (matrix == nullptr)
    {
      throw std::invalid_argument("invalid arg in minSumSdg");
    }
    int res = std::numeric_limits< int >::max();
    for (size_t i = 0; i < k_colomn; i++)
    {
      int d_sum = 0;
      size_t col = i, row = 0;

      while (row < k_row && col < k_colomn)
      {
        d_sum += matrix[row * k_colomn + col];
        row++;
        col++;
      }

      if (d_sum < res)
      {
        res = d_sum;
      }
    }
    for (size_t i = 1; i < k_row; ++i)
    {
      int d_sum = 0;
      size_t row = i, col = 0;

      while (row < k_row && col < k_colomn)
      {
        d_sum += matrix[row * k_colomn + col];
        ++row;
        ++col;
      }
      if (d_sum < res)
      {
        res = d_sum;
      }
    }

    return res;
  }

  void processFsMatrix(std::istream& input, std::ostream& output, size_t k_rows, size_t k_colomn)
  {
    int* matrix = new int[k_rows * k_colomn];
    try
    {
      dataCp(input, matrix, k_rows, k_colomn);
      int arg1 = numRowLsr(matrix, k_rows, k_colomn);
      int arg2 = minSumSdg(matrix, k_rows, k_colomn);

      output << arg1 << "\n";
      output << arg2 << "\n";
    }
    catch (...)
    {
      delete[] matrix;
      throw;
    }

    delete matrix;
  }

  void processStexMatrix(std::istream& input, std::ostream& output, size_t k_rows, size_t k_colomn)
  {
    const size_t size = 10000;
    if (k_rows * k_colomn > size)
    {
      throw std::invalid_argument("out of range\n");
    }

    int matrix[size] = {};
    dataCp(input, matrix, k_rows, k_colomn);

    int arg1 = numRowLsr(matrix, k_rows, k_colomn);
    int arg2 = minSumSdg(matrix, k_rows, k_colomn);

    output << arg1 << "\n";
    output << arg2 << "\n";
  }
}
int main(int argc, char** argv)
{
  const int arg_err_in_f = 2, arg_err_in_term = 1, k_arg = 4;
  const char mode_1 = '1', mode_2 = '2';
  size_t k_rows = 0, k_colomn = 0;
  int arg_1 = 0, arg_2 = 0;

  const size_t size_stec = 10000;

  if (argc < k_arg)
  {
    std::cerr << "Not enough arguments" << "\n";
    return arg_err_in_term;
  }
  if (argc > k_arg)
  {
    std::cerr << "Too many arguments" << "\n";
    return arg_err_in_term;
  }
  const char* const mode = argv[1];
  const char* const input_path = argv[2];
  const char* const output_path = argv[3];

  if ((mode[0] == mode_1 || mode[0] == mode_2) && mode[1] == '\0')
  {
    std::ifstream input{input_path};
    if (!input)
    {
      return arg_err_in_f;
    }

    switch (mode[0])
    {
    case mode_1:
    {
      if (!(input >> k_rows >> k_colomn))
      {
        return arg_err_in_f;
      }
      if ((k_rows == 0 || k_colomn == 0) && k_rows * k_colomn < size_stec)
      {
        return 0;
      }
      std::ofstream output(output_path);

      try
      {
        kuznetsov::processStexMatrix(input, output, k_rows, k_colomn);
      }
      catch (...)
      {
        std::cerr << "err in processStexMatrix";
        return arg_err_in_f;
      }

      break;
    }
    case mode_2:
    {
      input >> k_rows >> k_colomn;
      if (!input)
      {
        std::cerr << "invalid arg\n";
        return arg_err_in_f;
      }

      if (k_colomn == 0 || k_rows == 0)
      {
        return 0;
      }
      std::ofstream output(output_path);

      try
      {
        kuznetsov::processFsMatrix(input, output, k_rows, k_colomn);
      }
      catch (...)
      {
        std::cerr << "err in processFsMatrix\n";
        return arg_err_in_f;
      }
    }

    break;
    }
  }
  else
  {
    std::cerr << "Uncorrect mode argument" << "\n";
    return arg_err_in_term;
  }
  return 0;
}
