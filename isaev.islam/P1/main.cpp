#include <cstddef>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>

namespace isaev
{
  const std::size_t max_size = std::numeric_limits< std::size_t >::max();
  const std::size_t single_length = 1;
  const int required_count = 2;
  const int error_exit_code = 1;
  const int overflow_exit_code = 2;

  class IncreasingRunCounter
  {
  public:
    IncreasingRunCounter();
    void operator()(long long int next);
    std::size_t operator()() const noexcept;

  private:
    long long int previous_;
    std::size_t current_length_;
    std::size_t max_length_;
    bool has_previous_;
  };

  class BetweenCounter
  {
  public:
    BetweenCounter();
    void operator()(long long int next);
    std::size_t operator()() const noexcept;

  private:
    long long int previous_;
    long long int current_;
    std::size_t count_;
    int numbers_count_;

    static bool isBetween(long long int previous, long long int current, long long int next) noexcept;
  };

  int process(std::istream& in, std::ostream& out, std::ostream& err);
}

isaev::IncreasingRunCounter::IncreasingRunCounter():
  previous_(0),
  current_length_(0),
  max_length_(0),
  has_previous_(false)
{}

void isaev::IncreasingRunCounter::operator()(long long int next)
{
  if (has_previous_ && (next >= previous_))
  {
    if (current_length_ == isaev::max_size)
    {
      throw std::overflow_error("Length of the increasing run overflow");
    }
    ++current_length_;
  }
  else
  {
    current_length_ = isaev::single_length;
  }

  if (current_length_ > max_length_)
  {
    max_length_ = current_length_;
  }
  previous_ = next;
  has_previous_ = true;
}

std::size_t isaev::IncreasingRunCounter::operator()() const noexcept
{
  return max_length_;
}

isaev::BetweenCounter::BetweenCounter():
  previous_(0),
  current_(0),
  count_(0),
  numbers_count_(0)
{}

bool isaev::BetweenCounter::isBetween(long long int previous, long long int current, long long int next) noexcept
{
  return (current < previous) && (current > next);
}

void isaev::BetweenCounter::operator()(long long int next)
{
  if ((numbers_count_ == isaev::required_count) && isBetween(previous_, current_, next))
  {
    if (count_ == isaev::max_size)
    {
      throw std::overflow_error("Count of the elements overflow");
    }
    ++count_;
  }

  previous_ = current_;
  current_ = next;
  if (numbers_count_ < isaev::required_count)
  {
    ++numbers_count_;
  }
}

std::size_t isaev::BetweenCounter::operator()() const noexcept
{
  return count_;
}

int isaev::process(std::istream& in, std::ostream& out, std::ostream& err)
{
  isaev::IncreasingRunCounter run_counter;
  isaev::BetweenCounter between_counter;
  int read_count = 0;
  long long int number = 0;

  try
  {
    while ((in >> number) && (number != 0))
    {
      run_counter(number);
      between_counter(number);
      if (read_count < isaev::required_count)
      {
        ++read_count;
      }
    }
  }
  catch (const std::overflow_error&)
  {
    err << "overflow" << std::endl;
    return isaev::overflow_exit_code;
  }

  if (!in)
  {
    if (read_count < isaev::required_count)
    {
      err << "uncorrect data format" << std::endl;
    }
    else
    {
      err << "uncorrect symbols" << std::endl;
    }
    return isaev::error_exit_code;
  }

  out << run_counter() << std::endl;
  out << between_counter() << std::endl;
  return 0;
}

int main()
{
  return isaev::process(std::cin, std::cout, std::cerr);
}
