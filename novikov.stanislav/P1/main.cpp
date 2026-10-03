#include <iostream>
#include <limits>

int main()
{
  const int code_invalid_input = 1;
  const int code_too_long = 2;
  const unsigned long long initial_len = 1;
  const unsigned long long max_value = std::numeric_limits< unsigned long long >::max();

  long long current = 0;
  if (!(std::cin >> current))
  {
    std::cerr << "Error: invalid input data\n";
    return code_invalid_input;
  }

  if (current == 0)
  {
    std::cout << 0 << "\n";
    return 0;
  }

  long long previous = current;
  unsigned long long current_len = initial_len;
  unsigned long long max_len = initial_len;

  while (std::cin >> current)
  {
    if (current == 0)
    {
      break;
    }

    if (current <= previous)
    {
      if (current_len < max_value)
      {
        current_len++;
      }
      else
      {
        std::cerr << "Error: current_len exceeded maximum value.\n";
        return code_too_long;
      }
    }
    else
    {
      current_len = initial_len;
    }

    if (current_len > max_len)
    {
      max_len = current_len;
    }

    previous = current;
  }

  if (std::cin.fail())
  {
    std::cerr << "Error: invalid input sequence\n";
    return code_invalid_input;
  }

  std::cout << max_len << "\n";
  return 0;
}
