#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace psareva {
  class CountMax {
  public:
    CountMax();
    void operator()(int val);
    std::size_t operator()() const;
  private:
    int max_value_;
    std::size_t count_;
  };

  class MonDec {
  public:
    MonDec();
    void operator()(int val);
    std::size_t operator()() const noexcept;
  private:
    int prev_value_;
    std::size_t current_len_;
    std::size_t max_len_;
    bool has_data_;
  };

  int process(std::istream& in, std::ostream& out, std::ostream& err);
}

psareva::CountMax::CountMax():
  max_value_(0),
  count_(0)
{}

void psareva::CountMax::operator()(int val)
{
  if (count_ == 0 || val > max_value_) {
    max_value_ = val;
    count_ = 1;
  } else if (val == max_value_) {
    if (count_ == std::numeric_limits< std::size_t >::max()) {
      throw std::overflow_error("Counter overflow");
    }
    ++count_;
  }
}

std::size_t psareva::CountMax::operator()() const
{
  if (count_ == 0) {
    throw std::logic_error("Sequence is empty");
  }
  return count_;
}

psareva::MonDec::MonDec():
  prev_value_(0),
  current_len_(0),
  max_len_(0),
  has_data_(false)
{}

void psareva::MonDec::operator()(int val)
{
  if (!has_data_) {
    has_data_ = true;
    current_len_ = 1;
    max_len_ = 1;
    prev_value_ = val;
    return;
  }

  if (val <= prev_value_) {
    if (current_len_ == std::numeric_limits< std::size_t >::max()) {
      throw std::overflow_error("Counter overflow");
    }
    ++current_len_;
  } else {
    current_len_ = 1;
  }

  if (current_len_ > max_len_) {
    max_len_ = current_len_;
  }
  prev_value_ = val;
}

std::size_t psareva::MonDec::operator()() const noexcept
{
  return max_len_;
}

int psareva::process(std::istream& in, std::ostream& out, std::ostream& err)
{
  const int error_input = 1;
  const int error_calc = 2;
  const int success = 0;

  CountMax count_max;
  MonDec mon_dec;

  int val = 0;
  while ((in >> val) && (val != 0)) {
    try {
      count_max(val);
      mon_dec(val);
    } catch (const std::overflow_error&) {
      err << "Overflow error\n";
      return error_calc;
    }
  }

  if (!in) {
    err << "Incorrect input\n";
    return error_input;
  }

  bool has_calc_error = false;

  try {
    out << count_max() << "\n";
  } catch (const std::exception&) {
    err << "Too short sequence\n";
    has_calc_error = true;
  }

  try {
    out << mon_dec() << "\n";
  } catch (const std::exception&) {
    err << "Calculation error\n";
    has_calc_error = true;
  }

  if (has_calc_error) {
    return error_calc;
  }

  return success;
}

int main()
{
  return psareva::process(std::cin, std::cout, std::cerr);
}

