#include <cstddef>
#include <iostream>
#include <stdexcept>

namespace cheberiak {
  class SequenceAnalyzer {
    std::size_t count = 0, ans_for_15 = 0, ans_for_6 = 0, current_length_6 = 1;
    int previous = 0, before_previous = 0;
    const std::size_t first_element = 1;
    const std::size_t second_element = 2;
    const std::size_t min_sequence_length = 3;

  public:
    void next(int n)
    {
      before_previous = previous;
      previous = n;
      count++;
    }
    void getAnswer()
    {
      if (count == 1) {
        ans_for_6 = 1;
      }
      std::cout << ans_for_6 << std::endl;
      if (count < min_sequence_length) {
        throw std::runtime_error("Sequence is too short");
      }
      std::cout << ans_for_15 << std::endl;
    }
    std::size_t longestNonDecreasing(int n)
    {
      if (count >= first_element) {
        if (n < previous) {
          current_length_6 = 1;
        } else {
          current_length_6 += 1;
        }
        if (current_length_6 > ans_for_6) {
          ans_for_6 = current_length_6;
        }
      }
      return ans_for_6;
    }

    std::size_t countPreviousSum(int n)
    {
      if (count >= second_element) {
        if (static_cast< long long >(n) == static_cast< long long >(previous) + before_previous) {
          ans_for_15 += 1;
        }
      }
      return ans_for_15;
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
