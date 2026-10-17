#include <iostream>
#include <limits>

int main()
{
  const int invalid_input = 1;
  const int calculation_error = 2;
  const int success = 0;
  long long int first = -1;
  if (!(std::cin >> first))
  {
    std::cerr << "Error: invalid input" << std::endl;
    return invalid_input;
  }
  if (first == 0)
  {
    std::cout << "[EQL-SEQ] = " << 0 << std::endl;
    std::cerr << "Error: sequence is too short to [LOC-MAX]" << std::endl;
    return calculation_error;
  }
  long long int second = -1;
  if (!(std::cin >> second))
  {
    std::cerr << "Error: invalid input" << std::endl;
    return invalid_input;
  }
  unsigned long long int count = 0;
  unsigned long long int eql_c = 1;
  unsigned long long int eql_max = 1;
  if (second != 0)
  {
    long long int third = -1;
    while (std::cin >> third)
    {
      if (first == second)
      {
        if (eql_c == std::numeric_limits< unsigned long long int >::max())
        {
          std::cerr << "" << std::endl;
          return calculation_error;
        }
        eql_c++;
      }
      else
      {
        if (eql_max < eql_c)
        {
          eql_max = eql_c;
        }
        eql_c = 1;
      }
      if (third == 0)
      {
        break;
      }
      if (first < second && second > third)
      {
        if (count == std::numeric_limits< unsigned long long int >::max())
        {
          std::cerr << "" << std::endl;
          return calculation_error;
        }
        count++;
      }
      first = second;
      second = third;
    }
    if (std::cin.fail())
    {
      std::cerr << "Error: invalid input!" << std::endl;
      return invalid_input;
    }
  }
  if (eql_max < eql_c)
  {
    eql_max = eql_c;
  }
  std::cout << "[EQL-SEQ] = " << eql_max << std::endl;
  std::cout << "[LOC-MAX] = " << count << std::endl;
  return success;
}
