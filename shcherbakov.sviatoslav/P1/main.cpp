#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace shcherbakov
{
  struct MonIncCounter
  {
    std::size_t cur_len = 0;
    std::size_t max_len = 0;
    int prev_num = 0;
    bool is_first = true;

    void count(int num)
    {
      const std::size_t max_size = std::numeric_limits< std::size_t >::max();
      if (cur_len == max_size)
      {
        throw std::overflow_error("Sequence is too long");
      }

      if (is_first)
      {
        cur_len = 1;
        max_len = 1;
        prev_num = num;
        is_first = false;
        return;
      }

      if (num >= prev_num)
      {
        ++cur_len;
      }
      else
      {
        cur_len = 1;
      }

      if (cur_len > max_len)
      {
        max_len = cur_len;
      }
      prev_num = num;
    }

    std::size_t getResult() const
    {
      return max_len;
    }
  };

  struct EvenCountCounter
  {
    std::size_t cur_count = 0;
    std::size_t max_count = 0;

    void count(int num)
    {
      const std::size_t max_size = std::numeric_limits< std::size_t >::max();
      if (cur_count == max_size)
      {
        throw std::overflow_error("Sequence is too long");
      }

      const int even_divisor = 2;
      if (num % even_divisor == 0)
      {
        ++cur_count;
        if (cur_count > max_count)
        {
          max_count = cur_count;
        }
      }
      else
      {
        cur_count = 0;
      }
    }

    std::size_t getResult() const
    {
      return max_count;
    }
  };
}

int main()
{
  const int error_input = 1;
  const int error_calc = 2;

  shcherbakov::MonIncCounter mon_inc;
  shcherbakov::EvenCountCounter evn_cnt;

  bool is_mon_ok = true;
  bool is_evn_ok = true;

  int num = 0;
  while (std::cin >> num)
  {
    if (num == 0)
    {
      break;
    }

    if (is_mon_ok)
    {
      try
      {
        mon_inc.count(num);
      }
      catch (const std::overflow_error &)
      {
        is_mon_ok = false;
      }
    }

    if (is_evn_ok)
    {
      try
      {
        evn_cnt.count(num);
      }
      catch (const std::overflow_error &)
      {
        is_evn_ok = false;
      }
    }
  }

  if (!std::cin || num != 0)
  {
    std::cerr << "Incorrect input\n";
    return error_input;
  }

  if (!is_mon_ok || !is_evn_ok)
  {
    if (!is_mon_ok)
    {
      std::cerr << "Calculation error\n";
    }
    else
    {
      std::cout << mon_inc.getResult() << "\n";
    }

    if (!is_evn_ok)
    {
      std::cerr << "Calculation error\n";
    }
    else
    {
      std::cout << evn_cnt.getResult() << "\n";
    }

    return error_calc;
  }

  std::cout << mon_inc.getResult() << "\n";
  std::cout << evn_cnt.getResult() << "\n";
  return 0;
}
