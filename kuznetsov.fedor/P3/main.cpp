#include <iostream>
#include <fstream>
#include <stdexcept>
#include <limits>

namespace kuznetsov
{

  int** fsForMatrix(size_t k_rows, size_t k_colomn)
  {
    if (k_rows == 0 || k_colomn == 0)
    {
      throw(std::invalid_argument("invalid_argument\n"));
    }
    int** matrix = nullptr;
    size_t accept_rows = 0;
    try
    {
      matrix = new int*[k_rows];
      for (size_t i = 0; i < k_rows; ++i)
      {
        int* rows = new int[k_colomn];
        matrix[i] = rows;
        accept_rows++;
      }

      return matrix;
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "bad_alloc" << "\n";
      for (size_t i = 0; i < accept_rows; ++i)
      {
        delete[] matrix[i];
      }
      delete[] matrix;
      return nullptr;
    }
  }

  void fsDataCp(int** matrix, std::istream& input, size_t k_rows, size_t k_colomn)
  {
    for (size_t i = 0; i < k_rows; ++i)
    {
      for (size_t v = 0; v < k_colomn; ++v)
      {
        if (!(input >> matrix[i][v]))
        {
          throw std::invalid_argument("invalid arg in f\n");
        }
      }
    }
  }

  void stecDataCp(std::istream& input, int* matrix, size_t k_row, size_t k_column)
  {
    for (size_t i = 0; i < k_row * k_column; ++i)
    {
      if (!(input >> matrix[i]))
      {
        throw std::invalid_argument("invalid arg\n");
      }
    }
  }
  int numRowLsrStec(int* const matrix, size_t k_row, size_t k_colomn)
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

  int numRowLsrFs(int** matrix, size_t k_row, size_t k_colomn)
  {
    int max_ser = 0, row_res = 0;
    for (size_t i = 0; i < k_row; ++i)
    {
      int now_res = 1, max_res = 1;
      for (size_t v = 1; v < k_colomn; ++v)
      {
        if (matrix[i][v] == matrix[i][v - 1])
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
        row_res = i;
      }
    }
    return row_res + 1;
  }

  int minSumSdgStec(const int* matrix, size_t k_row, size_t k_colomn)
  {
    if (matrix == nullptr)
    {
      throw std::invalid_argument("invalid arg in minSumSdgStec");
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

  int minSumSdgFs(int** const matrix, size_t k_rows, size_t k_colomn)
  {
    if (matrix == nullptr)
    {
      throw(std::invalid_argument("invalid arg in min_sum_sdg\n"));
    }
    int res = std::numeric_limits< int >::max();

    for (size_t i = 0; i < k_colomn; i++)
    {
      int d_sum = 0;

      size_t col = i, row = 0;

      while (col < k_colomn && row < k_rows)
      {
        d_sum += matrix[row][col];
        col++;
        row++;
      }

      if (d_sum < res)
      {
        res = d_sum;
      }
    }

    for (size_t i = 1; i < k_rows; ++i)
    {
      int d_sum = 0;
      size_t col = 0, row = i;

      while (row < k_rows && col < k_colomn)
      {
        d_sum += matrix[row][col];
        row++;
        col++;
      }

      if (d_sum < res)
      {
        res = d_sum;
      }
    }

    return res;
  }
}
int main(int argc, char** argv)
{
  const int arg_err_in_f = 2, arg_err_in_term = 1;
  size_t k_rows = 0, k_colomn = 0;

  const size_t size_stec = 10000;

  if (argc < 4)
  {
    std::cerr << "Not enough arguments" << "\n";
    return arg_err_in_term;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments" << "\n";
    return arg_err_in_term;
  }
  const char* const mode = argv[1];
  const char* const input_path = argv[2];
  const char* const output_path = argv[3];

  if ((mode[0] == '1' || mode[0] == '2') && mode[1] == '\0')
  {
    std::ifstream input{input_path};
    if (!input)
    {
      return arg_err_in_f;
    }

    switch (mode[0])
    {
    case '1':
    {
      if (!(input >> k_rows >> k_colomn))
      {
        return arg_err_in_f;
      }
      if (k_rows == 0 || k_colomn == 0)
      {
        return 0;
      }
      std::ofstream output(output_path);

      int arg_1 = 0, arg_2 = 0;
      int matrix[size_stec] = {};
      try
      {
        kuznetsov::stecDataCp(input, matrix, k_rows, k_colomn);
      }
      catch (...)
      {
        std::cerr << "invalid arg in stecDataCp" << "\n";
        return arg_err_in_f;
      }
      try
      {
        arg_1 = kuznetsov::numRowLsrStec(matrix, k_rows, k_colomn);
        arg_2 = kuznetsov::minSumSdgStec(matrix, k_rows, k_colomn);
      }
      catch (...)
      {
        arg_1 = kuznetsov::numRowLsrStec(matrix, k_rows, k_colomn);
        std::cerr << "unreal minSumSdgStec" << "\n";
        output << arg_1 << "\n";
        return arg_err_in_f;
      }
      output << arg_1 << "\n";
      output << arg_2 << "\n";

      break;
    }
    case '2':
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

      int arg_1 = 0, arg_2 = 0;

      int** matrix_fs = kuznetsov::fsForMatrix(k_rows, k_colomn);
      try
      {
        kuznetsov::fsDataCp(matrix_fs, input, k_rows, k_colomn);
      }
      catch (...)
      {
        std::cerr << "invalid arg in fsDataCp" << "\n";
        for (size_t i = 0; i < k_rows; i++)
        {
          delete[] matrix_fs[i];
        }
        delete[] matrix_fs;
        return arg_err_in_f;
      }
      arg_1 = kuznetsov::numRowLsrFs(matrix_fs, k_rows, k_colomn);
      try
      {
        arg_2 = kuznetsov::minSumSdgFs(matrix_fs, k_rows, k_colomn);
      }
      catch (...)
      {
        output << arg_1 << "\n";
        std::cerr << "invalid arg in minSumSdgFs\n";
        for (size_t i = 0; i < k_rows; i++)
        {
          delete[] matrix_fs[i];
        }
        delete[] matrix_fs;
        return arg_err_in_f;
      }

      output << arg_1 << "\n";
      output << arg_2 << "\n";

      if (matrix_fs != nullptr)
      {
        for (size_t i = 0; i < k_rows; i++)
        {
          delete[] matrix_fs[i];
        }

        delete[] matrix_fs;
      }

      break;
    }
    }
  }
  else
  {
    std::cerr << "Uncorrect mode argument" << "\n";
    return arg_err_in_term;
  }
  return 0;
}

