#include <iostream>
#include <fstream>
#include <stdexcept>
#include <limits>

namespace kuznetsov
{
  const size_t size_stec = 10000;
  const int arg_err_in_f = 2, arg_err_in_term = 1, k_arg = 4;

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

  void processMatrix(std::istream& input, std::ostream& output, int* matrix, size_t k_rows, size_t k_colomn)
  {
    dataCp(input, matrix, k_rows, k_colomn);
    const int arg1 = numRowLsr(matrix, k_rows, k_colomn);
    const int arg2 = minSumSdg(matrix, k_rows, k_colomn);

    output << arg1 << "\n";
    output << arg2 << "\n";
  }

  void processFsMatrix(std::istream& input, std::ostream& output, size_t k_rows, size_t k_colomn)
  {
    int* matrix = new int[k_rows * k_colomn];
    try
    {
      processMatrix(input, output, matrix, k_rows, k_colomn);
    }
    catch (...)
    {
      delete[] matrix;
      throw;
    }

    delete[] matrix;
  }

  void processStexMatrix(std::istream& input, std::ostream& output, size_t k_rows, size_t k_colomn)
  {
    if (k_rows * k_colomn > kuznetsov::size_stec)
    {
      throw std::invalid_argument("out of range\n");
    }

    int matrix[kuznetsov::size_stec] = {};
    processMatrix(input, output, matrix, k_rows, k_colomn);
  }
}

bool read(std::istream& input, size_t& k_rows, size_t& k_colomn)
{
  if (!(input >> k_rows >> k_colomn))
  {
    return false;
  }
  return true;
}

bool nullmat(size_t k_rows, size_t k_colomn)
{
  return (k_rows == 0 || k_colomn == 0);
}

int main(int argc, char** argv)
{
  const char mode_1 = '1', mode_2 = '2';
  size_t k_rows = 0, k_colomn = 0;

  if (argc < kuznetsov::k_arg)
  {
    std::cerr << "Not enough arguments" << "\n";
    return kuznetsov::arg_err_in_term;
  }
  if (argc > kuznetsov::k_arg)
  {
    std::cerr << "Too many arguments" << "\n";
    return kuznetsov::arg_err_in_term;
  }
  const char* const mode = argv[1];
  const char* const input_path = argv[2];
  const char* const output_path = argv[3];

  if ((mode[0] == mode_1 || mode[0] == mode_2) && mode[1] == '\0')
  {
    std::ifstream input{input_path};
    if (!input)
    {
      return kuznetsov::arg_err_in_f;
    }
    if (!(read(input, k_rows, k_colomn)))
    {
      std::cerr << "arg_err_in_f\n";
      return kuznetsov::arg_err_in_f;
    }
    if (nullmat(k_rows, k_colomn))
    {
      return 0;
    }

    switch (mode[0])
    {
    case mode_1:
    {
      std::ofstream output(output_path);

      try
      {
        kuznetsov::processStexMatrix(input, output, k_rows, k_colomn);
      }
      catch (const std::exception& e)
      {
        std::cerr << "err in processStexMatrix\n" << e.what() << "\n";
        return kuznetsov::arg_err_in_f;
      }

      break;
    }
    case mode_2:
    {
      std::ofstream output(output_path);

      try
      {
        kuznetsov::processFsMatrix(input, output, k_rows, k_colomn);
      }
      catch (...)
      {
        std::cerr << "err in processFsMatrix\n";
        return kuznetsov::arg_err_in_f;
      }
    }

    break;
    }
  }
  else
  {
    std::cerr << "Uncorrect mode argument" << "\n";
    return kuznetsov::arg_err_in_term;
  }
  return 0;
}
