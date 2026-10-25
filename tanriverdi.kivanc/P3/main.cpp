
#include <iostream>
#include <cstddef>

namespace tanriverdikivanc
{
  const int input_error = 1;
  const int calculation_error = 2;

  int run()
  {
    int value = 0;
    int previous = 0;
    int previous_previous = 0;

    std::size_t count = 0;
    std::size_t current_length = 0;
    std::size_t max_length = 0;
    std::size_t sum_dup_count = 0;

    bool has_previous = false;
    bool has_previous_previous = false;
    bool zero_found = false;

    while (std::cin >> value)
    {
      if (value == 0)
      {
        zero_found = true;
        break;
      }

      ++count;

      if (!has_previous)
      {
        current_length = 1;
      }
      else if (value <= previous)
      {
        ++current_length;
      }
      else
      {
        current_length = 1;
      }

      if (current_length > max_length)
      {
        max_length = current_length;
      }

      if (has_previous && has_previous_previous)
      {
        const long long sum =
          static_cast<long long>(previous_previous) + previous;

        if (static_cast<long long>(value) == sum)
        {
          ++sum_dup_count;
        }
      }

      previous_previous = previous;
      has_previous_previous = has_previous;

      previous = value;
      has_previous = true;
    }

    if (!zero_found)
    {
      std::cerr << "Invalid input sequence\n";
      return input_error;
    }

    std::cout << max_length << '\n';

    if (count < 3)
    {
      std::cerr << "Not enough elements for SUM-DUP\n";
      return calculation_error;
    }

    std::cout << sum_dup_count << '\n';
    return 0;
  }
}

int main()
{
  return tanriverdikivanc::run();
}
