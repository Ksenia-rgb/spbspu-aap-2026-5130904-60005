#include <cstddef>
#include <iostream>
#include <stdexcept>

namespace cheberiak {
  class SequenceAnalyzer {
    std::size_t count_ = 0, ans_for_15_ = 0, ans_for_6_ = 0, current_length_6_ = 1;
    int previous_ = 0, before_previous_ = 0;
    const std::size_t first_element_ = 1;
    const std::size_t second_element_ = 2;
    const std::size_t min_sequence_length_ = 3;

  public:
    void next(int n)
    {
      before_previous_ = previous_;
      previous_ = n;
      count_++;
    }
    void getAnswer()
    {
      if (count_ == 1) {
        ans_for_6_ = 1;
      }
      std::cout << ans_for_6_ << std::endl;
      if (count_ < min_sequence_length_) {
        throw std::runtime_error("Sequence is too short");
      }
      std::cout << ans_for_15_ << std::endl;
    }
    std::size_t longestNonDecreasing(int n)
    {
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
      return ans_for_6_;
    }

    std::size_t countPreviousSum(int n)
    {
      if (count_ >= second_element_) {
        if (static_cast< long long >(n) == static_cast< long long >(previous_) + before_previous_) {
          ans_for_15_ += 1;
        }
      }
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
  cheberiak::SequenceAnalyzer analyzer;
  try {
    while (std::cin >> n) {
      if (n == 0) {
        analyzer.getAnswer();
        break;
      }
      analyzer.longestNonDecreasing(n);
      analyzer.countPreviousSum(n);
      analyzer.next(n);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return exit_code_short_seq;
  }
  if (std::cin.fail()) {
    std::cerr << "Error: invalid input\n";
    return exit_code_incorrect_input;
  }
  return exit_code_good;
}