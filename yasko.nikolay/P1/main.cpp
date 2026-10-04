#include <iostream>

int main()
{
  long long first;
  if (!(std::cin >> first))
  {
    std::cerr << "Error: invalid input";
    return 1;
  }
  if (first == 0)
  {
    std::cerr << "Error: sequence is too short";
    return 2;
  }
  long long second;
  if (!(std::cin >> second))
  {
    std::cerr << "Error: invalid input";
    return 1;
  }
  unsigned long long count = 0;
  if (second != 0)
  {
    long long third = 0;
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
      return 1;
    }
  }
  std::cout << count << std::endl;
  return 0;
}
