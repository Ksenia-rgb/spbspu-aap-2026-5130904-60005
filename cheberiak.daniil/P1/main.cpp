#include <cstddef>
#include <iostream>
#include <stdexcept>

namespace cheberiak {
  class MonInc {
    const std::size_t first_element_ = 1;
  public:
    std::size_t count_ = 0, ans_for_6_ = 0, current_length_6_ = 1;
    int previous_ = 0;
    std::size_t longestNonDecreasing(int n)
    {
      if (count_ == 0) {
        ans_for_6_ = 1;
      }
      if (count_ >= first_element_) {
        if (n < previous_) {
          current_length_6_ = 1;
        } else {
          current_length_6_ += 1;
        }
        if (current_length_6_ > ans_for_6_) {
          ans_for_6_ = current_length_6_;
        }
      }
      count_++;
      previous_ = n;
      return ans_for_6_;
    }
  };

  class SumDup {
    const std::size_t second_element_ = 2;
  public:
    std::size_t count_ = 0, ans_for_15_ = 0;
    int previous_ = 0, before_previous_ = 0;
    std::size_t countPreviousSum(int n)
    {
      if (count_ >= second_element_) {
        if (static_cast< long long >(n) == static_cast< long long >(previous_) + before_previous_) {
          ans_for_15_ += 1;
        }
      }
      count_++;
      before_previous_ = previous_;
      previous_ = n;
      return ans_for_15_;
    }
  };
}

int main()
{
  int n = 0;
  const int exit_code_good = 0;
  const int exit_code_incorrect_input = 1;
  const int exit_code_short_seq = 2;
  const std::size_t min_sequence_length_ = 3;
  cheberiak::SumDup SumDup;
  cheberiak::MonInc MonInc;
  while (std::cin >> n) {
    if (n == 0) {
      break;
    }
    MonInc.longestNonDecreasing(n);
    SumDup.countPreviousSum(n);
  }
  if (std::cin.fail()) {
    std::cerr << "Error: invalid input\n";
    return exit_code_incorrect_input;
  }
  std::cout << MonInc.ans_for_6_ << std::endl;
  if (SumDup.count_ < min_sequence_length_) {
    std::cerr << "Sequence is too short";
    return exit_code_short_seq;
  }
  std::cout << SumDup.ans_for_15_ << std::endl;
  return exit_code_good;
}
