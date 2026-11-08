#include <iostream>
#include <istream>
#include <limits>
#include <ostream>

namespace {
const int input_error_code = 1;
const int calculation_error_code = 2;
}

class SequenceCharacteristics {
public:
  void nextN(const int n) noexcept
  {
    if (has_previous_) {
      if (n > previous_) {
        increment(increases_, is_err_increases_);
      }
      if ((previous_ < 0 && n > 0) || (previous_ > 0 && n < 0)) {
        increment(sign_changes_, is_err_sign_changes_);
      }
    }
    previous_ = n;
    has_previous_ = true;
  }

  void print(std::ostream &out, std::ostream &err) const
  {
    if (!is_err_increases_) {
      out << increases_ << '\n';
    } else {
      err << "INC-SEQ error\n";
    }
    if (!is_err_sign_changes_) {
      out << sign_changes_ << '\n';
    } else {
      err << "SGN-CHG error\n";
    }
  }

  bool hasError() const noexcept
  {
    return is_err_increases_ || is_err_sign_changes_;
  }

private:
  static void increment(int &counter, bool &is_err) noexcept
  {
    if (counter == std::numeric_limits<int>::max()) {
      is_err = true;
    } else {
      ++counter;
    }
  }

  int previous_ = 0;
  bool has_previous_ = false;
  int increases_ = 0;
  int sign_changes_ = 0;
  bool is_err_increases_ = false;
  bool is_err_sign_changes_ = false;
};

int process(std::istream &in, std::ostream &out, std::ostream &err)
{
  SequenceCharacteristics c;
  int n = 0;
  while (in >> n && n != 0) {
    c.nextN(n);
  }
  if (!in) {
    err << "Invalid input\n";
    return input_error_code;
  }
  c.print(out, err);
  if (c.hasError()) {
    return calculation_error_code;
  }
  return 0;
}

int main()
{
  return process(std::cin, std::cout, std::cerr);
}
