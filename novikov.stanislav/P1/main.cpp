#include <iostream>
#include <limits>

namespace novikov
{
  int processSequence(std::istream& in, std::ostream& out, std::ostream& err)
  {
    const int code_invalid_input = 1;
    const int code_too_long = 2;
    const unsigned long long initial_len = 1;
    const unsigned long long max_value = std::numeric_limits< unsigned long long >::max();

    long long current = 0;
    if (!(in >> current))
    {
      err << "Error: invalid input data" << std::endl;
      return code_invalid_input;
    }

    if (current == 0)
    {
      out << 0 << std::endl;
      return 0;
    }

    long long prev = current;
    unsigned long long current_len = initial_len;
    unsigned long long max_len = initial_len;

    while (in >> current)
    {
      if (current == 0)
      {
        break;
      }

      if (current <= prev)
      {
        if (current_len < max_value)
        {
          current_len++;
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

      prev = current;
    }

    if (in.fail())
    {
      err << "Error: invalid input sequence" << std::endl;
      return code_invalid_input;
    }

    if (max_len == max_value)
    {
      err << "Error: sequence is too long" << std::endl;
      return code_too_long;
    }

    out << max_len << std::endl;
    return 0;
  }
}

int main()
{
  return novikov::processSequence(std::cin, std::cout, std::cerr);
}
