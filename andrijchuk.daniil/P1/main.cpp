#include <iostream>
#include <cstddef>

using l_l = long long int;

namespace andrijchuk
{
  void input(l_l &n)
  {
    std::cin >> n;
    if (std::cin.fail())
    {
      throw std::invalid_argument("Incorrect Arguments.\n");
    }
  }

  l_l locMinCnt(l_l n, l_l prev_n, l_l pr_prev_n, l_l count)
  {
    if (pr_prev_n > prev_n && prev_n < n)
    {
      count++;
    }
    return count;
  }

  l_l sumDupCnt(l_l n, l_l prev_n, l_l pr_prev_n, l_l count)
  {
    if (n == pr_prev_n + prev_n)
    {
      count++;
    }
    return count;
  }
}

int main()
{
  namespace ad = andrijchuk;
  const int min_arg = 3;
  const int inp_err_exitcode1 = 1;
  const int seq_err_exitcode2 = 2;
  bool sumdup_err = false;
  l_l n = 0, prev_n = 0, pr_prev_n = 0;
  l_l count_locmin = 0, count_sumdup = 0;
  l_l k = 0;
  try
  {
    ad::input(n);
    if (n == 0)
    {
      throw std::runtime_error("Incorrect sequence.\n");
    }
    k++;
    prev_n = n;
    while (true)
    {
      ad::input(n);
      if (n == 0)
      {
        break;
      }
      k++;
      if (k >= min_arg)
      {
        count_locmin = ad::locMinCnt(n, prev_n, pr_prev_n, count_locmin);
        count_sumdup = ad::sumDupCnt(n, prev_n, pr_prev_n, count_sumdup);
      }
      pr_prev_n = prev_n;
      prev_n = n;
    }
    if (k < min_arg)
    {
      sumdup_err = true;
      throw std::runtime_error("Incorrect sequence for sum_dup.\n");
    }
  }
  catch (const std::invalid_argument &e)
  {
    std::cerr << e.what();
    return inp_err_exitcode1;
  }
  catch (const std::runtime_error &e)
  {
    if (sumdup_err == true)
    {
      std::cout << count_locmin << "\n";
      std::cerr << e.what();
      return seq_err_exitcode2;
    }
    std::cerr << e.what();
    return seq_err_exitcode2;
  }
  std::cout << count_locmin << "\n";
  std::cout << count_sumdup << "\n";
  return 0;
}
