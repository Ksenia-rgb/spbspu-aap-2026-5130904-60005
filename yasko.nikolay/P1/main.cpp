#include <iostream>

int main()
{
  const int invalid_input = 1;
  const int too_short = 2;
  const int succes = 0;
  long long first = -1;
  if (!(std::cin >> first))
  {
    std::cerr << "Error: invalid input";
    return invalid_input;
  }
  if (first == 0)
  {
    std::cerr << "Error: sequence is too short";
    return too_short;
  }
  long long second = -1;
  if (!(std::cin >> second))
  {
    std::cerr << "Error: invalid input";
    return invalid_input;
  }
  unsigned long long count = 0;
  if (second != 0)
  {
    long long third = -1;
    while (std::cin >> third)
    {
      if (third == 0)
      {
        break;
      }
      if (first < second && second > third)
      {
        count++;
      }
      first = second;
      second = third;
    }
    if (std::cin.fail())
    {
      std::cerr << "invalid input";
      return invalid_input;
    }
  }
  std::cout << count << std::endl;
  return succes;
}
