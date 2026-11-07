#include <iostream>
#include <limits>

namespace isaev {
  const int max_int = std::numeric_limits< int >::max();
  const int single_length = 1;
  const int required_count = 2;
  const int error_exit_code = 1;
  const int overflow_exit_code = 2;

  class SequenceStats {
  public:
    void addNumber(long long int number) noexcept;
    void print(std::ostream& out, std::ostream& err) const noexcept;
    bool hasOverflow() const noexcept;
    int getNumbersCount() const noexcept;

  private:
    long long int previous_ = 0;
    long long int current_ = 0;
    int numbers_count_ = 0;
    int current_length_ = 0;
    int max_length_ = 0;
    int between_count_ = 0;
    bool length_overflow_ = false;
    bool count_overflow_ = false;

    static bool isIncreasing(long long int current, long long int next) noexcept;
    static bool isBetween(long long int previous, long long int current, long long int next) noexcept;
  };

  int process(std::istream& in, std::ostream& out, std::ostream& err);
}

bool isaev::SequenceStats::isIncreasing(long long int current, long long int next) noexcept
{
  return next >= current;
}

bool isaev::SequenceStats::isBetween(long long int previous, long long int current, long long int next) noexcept
{
  return (current < previous) && (current > next);
}

void isaev::SequenceStats::addNumber(long long int number) noexcept
{
  if (numbers_count_ == 0) {
    current_length_ = isaev::single_length;
    max_length_ = isaev::single_length;
  } else {
    if (isIncreasing(current_, number)) {
      if (current_length_ == isaev::max_int) {
        length_overflow_ = true;
      } else {
        ++current_length_;
      }
    } else {
      current_length_ = isaev::single_length;
    }

    if (current_length_ > max_length_) {
      max_length_ = current_length_;
    }

    if ((numbers_count_ == isaev::required_count) && isBetween(previous_, current_, number)) {
      if (between_count_ == isaev::max_int) {
        count_overflow_ = true;
      } else {
        ++between_count_;
      }
    }
  }

  previous_ = current_;
  current_ = number;
  if (numbers_count_ < isaev::required_count) {
    ++numbers_count_;
  }
}

void isaev::SequenceStats::print(std::ostream& out, std::ostream& err) const noexcept
{
  if (length_overflow_) {
    err << "overflow" << std::endl;
  } else {
    out << max_length_ << std::endl;
  }
  if (count_overflow_) {
    err << "overflow" << std::endl;
  } else {
    out << between_count_ << std::endl;
  }
}

bool isaev::SequenceStats::hasOverflow() const noexcept
{
  return length_overflow_ || count_overflow_;
}

int isaev::SequenceStats::getNumbersCount() const noexcept
{
  return numbers_count_;
}

int isaev::process(std::istream& in, std::ostream& out, std::ostream& err)
{
  isaev::SequenceStats stats;
  long long int number = 0;
  while ((in >> number) && (number != 0)) {
    stats.addNumber(number);
  }

  if (!in) {
    if (stats.getNumbersCount() < isaev::required_count) {
      err << "uncorrect data format" << std::endl;
    } else {
      err << "uncorrect symbols" << std::endl;
    }
    return isaev::error_exit_code;
  }

  stats.print(out, err);
  if (stats.hasOverflow()) {
    return isaev::overflow_exit_code;
  }
  return 0;
}

int main()
{
  return isaev::process(std::cin, std::cout, std::cerr);
}