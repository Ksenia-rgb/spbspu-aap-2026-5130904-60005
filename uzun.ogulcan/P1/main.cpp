#include <cstddef>
#include <iostream>

namespace uzun
{
  constexpr int EXIT_NOT_SEQUENCE = 1;
  constexpr int EXIT_NOT_ENOUGH_DATA = 2;

  struct Stats
  {
    std::size_t count;
    int max;
    int sub;
    std::size_t max_count;
  };

  void update(Stats &s, int x)
  {
    if (s.count == 0)
    {
      s.max = x;
      s.max_count = 1;
    }
    else if (x > s.max)
    {
      s.sub = s.max;
      s.max = x;
      s.max_count = 1;
    }
    else
    {
      if (x == s.max)
      {
        ++s.max_count;
      }
      if (s.count == 1 || x > s.sub)
      {
        s.sub = x;
      }
    }
    ++s.count;
  }

  bool readSequence(std::istream &in, Stats &s)
  {
    int value = 0;
    while ((in >> value) && (value != 0))
    {
      update(s, value);
    }
    return !in.fail();
  }

  int printResults(const Stats &s, std::ostream &out, std::ostream &err)
  {
    int result = 0;

    if (s.count < 2)
    {
      err << "SUB-MAX: sequence is too short\n";
      result = EXIT_NOT_ENOUGH_DATA;
    }
    else
    {
      out << s.sub << '\n';
    }

    if (s.count == 0)
    {
      err << "CNT-MAX: sequence is empty\n";
      result = EXIT_NOT_ENOUGH_DATA;
    }
    else
    {
      out << s.max_count << '\n';
    }

    return result;
  }
}

int main()
{
  uzun::Stats stats = {0, 0, 0, 0};

  if (!uzun::readSequence(std::cin, stats))
  {
    std::cerr << "Input is not a sequence\n";
    return uzun::EXIT_NOT_SEQUENCE;
  }

  return uzun::printResults(stats, std::cout, std::cerr);
}
