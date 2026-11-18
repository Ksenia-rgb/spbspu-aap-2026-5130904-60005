#include <cstddef>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>

namespace tsulyauf {

  const std::size_t min_length = 2;
  const std::size_t max_count = std::numeric_limits< std::size_t >::max();
  const int divisor_minus_one = -1;

  const int invalid_input_code = 1;
  const int calculation_error_code = 2;

  const char* const overflow_message = "Overflow error!";

  class SequenceCounter {
  public:
    void operator()();
    void countUpToMinLength();
    bool hasElements() const;
    void checkMinLengthReached() const;
    std::size_t getCount() const;

  private:
    std::size_t counter_ = 0;
  };

  class DivisibleCount {
  public:
    void operator()(int curr);
    std::size_t operator()() const;

  private:
    int previous_ = 0;
    bool is_count_overflow_ = false;
    SequenceCounter matches_;
    SequenceCounter elements_;

    void addMatch();
  };

  class SecondMaxValue {
  public:
    void operator()(int curr);
    int operator()() const;

  private:
    int first_max_ = std::numeric_limits< int >::min();
    int second_max_ = std::numeric_limits< int >::min();
    SequenceCounter elements_;
  };

  int process(std::istream& input, std::ostream& output, std::ostream& err_output);

}

void tsulyauf::SequenceCounter::operator()()
{
  if (counter_ == max_count) {
    throw std::overflow_error(overflow_message);
  }
  ++counter_;
}

void tsulyauf::SequenceCounter::countUpToMinLength()
{
  if (counter_ < min_length) {
    ++counter_;
  }
}

bool tsulyauf::SequenceCounter::hasElements() const
{
  return counter_ > 0;
}

void tsulyauf::SequenceCounter::checkMinLengthReached() const
{
  if (counter_ < min_length) {
    throw std::invalid_argument("Sequence is too short!");
  }
}

std::size_t tsulyauf::SequenceCounter::getCount() const
{
  return counter_;
}

void tsulyauf::DivisibleCount::operator()(int curr)
{
  if (elements_.hasElements() && (previous_ == divisor_minus_one || curr % previous_ == 0)) {
    addMatch();
  }

  elements_.countUpToMinLength();

  previous_ = curr;
}

std::size_t tsulyauf::DivisibleCount::operator()() const
{
  elements_.checkMinLengthReached();

  if (is_count_overflow_) {
    throw std::overflow_error(overflow_message);
  }
  return matches_.getCount();
}

void tsulyauf::DivisibleCount::addMatch()
{
  try {
    matches_();
  } catch (const std::overflow_error&) {
    is_count_overflow_ = true;
  }
}

void tsulyauf::SecondMaxValue::operator()(int curr)
{
  if (!elements_.hasElements()) {
    first_max_ = curr;
  } else if (curr >= first_max_) {
    second_max_ = first_max_;
    first_max_ = curr;
  } else if (curr > second_max_) {
    second_max_ = curr;
  }

  elements_.countUpToMinLength();
}

int tsulyauf::SecondMaxValue::operator()() const
{
  elements_.checkMinLengthReached();

  return second_max_;
}

int tsulyauf::process(std::istream& input, std::ostream& output, std::ostream& err_output)
{
  DivisibleCount divisible_count;
  SecondMaxValue second_max_value;
  int curr = 0;

  while (input >> curr && curr != 0) {
    divisible_count(curr);
    second_max_value(curr);
  }

  if (!input) {
    err_output << "Sequence input error!\n";
    return invalid_input_code;
  }

  int code = 0;
  bool is_too_short_reported = false;

  try {
    output << divisible_count() << "\n";
  } catch (const std::invalid_argument& e) {
    err_output << e.what() << "\n";
    is_too_short_reported = true;
    code = calculation_error_code;
  } catch (const std::overflow_error& e) {
    err_output << e.what() << "\n";
    code = calculation_error_code;
  }

  try {
    output << second_max_value() << "\n";
  } catch (const std::invalid_argument& e) {
    if (!is_too_short_reported) {
      err_output << e.what() << "\n";
    }
    code = calculation_error_code;
  }

  return code;
}

int main()
{
  return tsulyauf::process(std::cin, std::cout, std::cerr);
}
