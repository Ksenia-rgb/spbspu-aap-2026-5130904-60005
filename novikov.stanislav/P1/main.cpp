#include <iostream>
#include <limits>
#include <stdexcept>

namespace novikov
{
  const unsigned long long initial_len = 1;
  const long long even_divisor = 2;
  const int code_invalid_input = 1;
  const int code_calculation_error = 2;
  const char* const invalid_input_message = "Error: Invalid input. Please enter a valid integer.\n";

  bool addChecked(unsigned long long& value, unsigned long long addend);

  class MonotonicLengthCounter
  {
  public:
    MonotonicLengthCounter();

    void operator()(long long next);
    unsigned long long operator()() const;

  private:
    unsigned long long mon_cur_;
    unsigned long long mon_max_;
    long long prev_;
    bool has_prev_;
  };

  class EvenLengthCounter
  {
  public:
    EvenLengthCounter();

    void operator()(long long next);
    unsigned long long operator()() const;

  private:
    unsigned long long even_cur_;
    unsigned long long even_max_;
  };

  class SequenceAnalyzer
  {
  public:
    SequenceAnalyzer();

    void addNumber(long long next);
    int print(std::ostream& out, std::ostream& err) const;

  private:
    MonotonicLengthCounter mon_;
    EvenLengthCounter even_;
    bool mon_overflow_;
    bool even_overflow_;
  };

  int process(std::istream& in, std::ostream& out, std::ostream& err);
}

bool novikov::addChecked(unsigned long long& value, const unsigned long long addend)
{
  if (value > (std::numeric_limits< unsigned long long >::max() - addend))
  {
    return false;
  }
  value += addend;
  return true;
}

novikov::MonotonicLengthCounter::MonotonicLengthCounter():
  mon_cur_(0),
  mon_max_(0),
  prev_(0),
  has_prev_(false)
{}

void novikov::MonotonicLengthCounter::operator()(const long long next)
{
  if (!has_prev_)
  {
    mon_cur_ = initial_len;
    mon_max_ = initial_len;
    has_prev_ = true;
  }
  else
  {
    if (next <= prev_)
    {
      if (!addChecked(mon_cur_, initial_len))
      {
        throw std::overflow_error("monotonic length overflow");
      }
    }
    else
    {
      mon_cur_ = initial_len;
    }

    if (mon_cur_ > mon_max_)
    {
      mon_max_ = mon_cur_;
    }
  }
  prev_ = next;
}

unsigned long long novikov::MonotonicLengthCounter::operator()() const
{
  return mon_max_;
}

novikov::EvenLengthCounter::EvenLengthCounter():
  even_cur_(0),
  even_max_(0)
{}

void novikov::EvenLengthCounter::operator()(const long long next)
{
  if ((next % even_divisor) == 0)
  {
    if (!addChecked(even_cur_, initial_len))
    {
      throw std::overflow_error("even length overflow");
    }
  }
  else
  {
    even_cur_ = 0;
  }

  if (even_cur_ > even_max_)
  {
    even_max_ = even_cur_;
  }
}

unsigned long long novikov::EvenLengthCounter::operator()() const
{
  return even_max_;
}

novikov::SequenceAnalyzer::SequenceAnalyzer():
  mon_overflow_(false),
  even_overflow_(false)
{}

void novikov::SequenceAnalyzer::addNumber(const long long next)
{
  if (!mon_overflow_)
  {
    try
    {
      mon_(next);
    }
    catch (const std::overflow_error&)
    {
      mon_overflow_ = true;
    }
  }

  if (!even_overflow_)
  {
    try
    {
      even_(next);
    }
    catch (const std::overflow_error&)
    {
      even_overflow_ = true;
    }
  }
}

int novikov::SequenceAnalyzer::print(std::ostream& out, std::ostream& err) const
{
  if (!mon_overflow_)
  {
    out << mon_() << "\n";
  }
  else
  {
    err << "Error: Overflow occurred while calculating monotonic length.\n";
  }

  if (!even_overflow_)
  {
    out << even_() << "\n";
  }
  else
  {
    err << "Error: Overflow occurred while calculating even length.\n";
  }

  if (mon_overflow_ || even_overflow_)
  {
    return code_calculation_error;
  }

  return 0;
}

int novikov::process(std::istream& in, std::ostream& out, std::ostream& err)
{
  long long current = 0;
  if (!(in >> current))
  {
    err << invalid_input_message;
    return code_invalid_input;
  }

  if (current == 0)
  {
    out << 0 << "\n" << 0 << "\n";
    return 0;
  }

  SequenceAnalyzer analyzer;
  analyzer.addNumber(current);

  while ((in >> current) && (current != 0))
  {
    analyzer.addNumber(current);
  }

  if (in.fail())
  {
    err << invalid_input_message;
    return code_invalid_input;
  }

  return analyzer.print(out, err);
}

int main()
{
  return novikov::process(std::cin, std::cout, std::cerr);
}
