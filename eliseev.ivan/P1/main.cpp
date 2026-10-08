#include <iostream>

namespace eliseev
{
  int dibil(long long& a)
  {
    if (!(std::cin >> a))
    {
      return 1;
    }
    return 0;
  }
}

int main()
{
  using namespace eliseev;

  const long long smallch = -9223372036854775807LL - 1;
  const long long suk2 = 2;
  const int suk = 1;
  const int fail_code = 2;

  long long max1 = smallch;
  long long max2 = smallch;
  long long count = 0;

  long long prev = 0;
  bool has_prev = false;
  long long cur_len = 0;
  long long best_len = 0;

  while (true)
  {
    long long a = 0;
    if (dibil(a) != 0)
    {
      std::cerr << "enter a chislo \n";
      return suk;
    }
    if (a == 0)
    {
      break;
    }
    ++count;

    if (a > max1)
    {
      max2 = max1;
      max1 = a;
    }
    else if (a > max2 && a != max1)
    {
      max2 = a;
    }

    if (!has_prev)
    {
      cur_len = 1;
      has_prev = true;
    }
    else if (a >= prev)
    {
      ++cur_len;
    }
    else
    {
      cur_len = 1;
    }
    prev = a;
    if (cur_len > best_len)
    {
      best_len = cur_len;
    }
  }

  const bool sub_max_ok = (count >= suk2 && max2 != smallch);

  if (sub_max_ok)
  {
    std::cout << max2 << "\n";
  }
  else
  {
    std::cerr << "ERROR: small posledovatelnost\n";
  }

  std::cout << best_len << "\n";

  return sub_max_ok ? 0 : fail_code;
}
