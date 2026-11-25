#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace shcherbakov
{
  void checkLength(std::size_t length)
  {
    if (length == std::numeric_limits< std::size_t >::max())
    {
      throw std::overflow_error("Sequence is too long");
    }
  }

  template< class Counter >
  void updateCounter(Counter &counter, bool &is_ok, int num)
  {
    if (is_ok)
    {
      try
      {
        counter(num);
      }
      catch (const std::overflow_error &)
      {
        is_ok = false;
      }
    }
  }

  void printResult(std::ostream &out, std::ostream &err, bool is_ok, std::size_t result)
  {
    if (is_ok)
    {
      out << result << "\n";
    }
    else
    {
      err << "Calculation error\n";
    }
  }

  class MonIncCounter
  {
  public:
    void operator()(int num)
    {
      count(num);
    }

    void count(int num)
    {
      checkLength(cur_len_);

      if (is_first_)
      {
        cur_len_ = 1;
        max_len_ = 1;
        prev_num_ = num;
        is_first_ = false;
        return;
      }

      if (num >= prev_num_)
      {
        ++cur_len_;
      }
      else
      {
        cur_len_ = 1;
      }

      if (cur_len_ > max_len_)
      {
        max_len_ = cur_len_;
      }
      prev_num_ = num;
    }

    std::size_t getResult() const noexcept
    {
      return max_len_;
    }

  private:
    std::size_t cur_len_ = 0;
    std::size_t max_len_ = 0;
    int prev_num_ = 0;
    bool is_first_ = true;
  };

  class EvenCountCounter
  {
  public:
    void operator()(int num)
    {
      count(num);
    }

    void count(int num)
    {
      checkLength(cur_count_);

      const int even_divisor = 2;
      if (num % even_divisor != 0)
      {
        cur_count_ = 0;
        return;
      }

      ++cur_count_;
      if (cur_count_ > max_count_)
      {
        max_count_ = cur_count_;
      }
    }

    std::size_t getResult() const noexcept
    {
      return max_count_;
    }

  private:
    std::size_t cur_count_ = 0;
    std::size_t max_count_ = 0;
  };

  class SequenceAnalyzer
  {
  public:
    void operator()(int num)
    {
      nextN(num);
    }

    void nextN(int num)
    {
      updateCounter(mon_inc_, is_mon_ok_, num);
      updateCounter(evn_cnt_, is_evn_ok_, num);
    }

    int print(std::ostream &out, std::ostream &err) const
    {
      const int error_calc = 2;
      const int success = 0;

      printResult(out, err, is_mon_ok_, mon_inc_.getResult());
      printResult(out, err, is_evn_ok_, evn_cnt_.getResult());

      if (!is_mon_ok_ || !is_evn_ok_)
      {
        return error_calc;
      }
      return success;
    }

  private:
    MonIncCounter mon_inc_;
    EvenCountCounter evn_cnt_;
    bool is_mon_ok_ = true;
    bool is_evn_ok_ = true;
  };

  int process(std::istream &in, std::ostream &out, std::ostream &err)
  {
    const int error_input = 1;
    SequenceAnalyzer analyzer;
    int num = 0;
    while (in >> num && num != 0)
    {
      analyzer(num);
    }

    if (!in || num != 0)
    {
      err << "Incorrect input\n";
      return error_input;
    }

    return analyzer.print(out, err);
  }
}

int main()
{
  return shcherbakov::process(std::cin, std::cout, std::cerr);
}
