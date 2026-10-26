#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

namespace nekhorukov
{
  const int MAX = 10000;

  bool upperTri(const long long * const m, int r, int c)
  {
    int n = r < c ? r : c;
    for (int i = 0; i < n; ++i)
    {
      for (int j = 0; j < i; ++j)
      {
        if (m[i * c + j] != 0)
        {
          return false;
        }
      }
    }
    return true;
  }

  int rowsNoSame(const long long * const m, int r, int c)
  {
    int res = 0;
    for (int i = 0; i < r; ++i)
    {
      bool same = false;
      for (int j = 1; j < c; ++j)
      {
        if (m[i * c + j] == m[i * c + j - 1])
        {
          same = true;
          break;
        }
      }
      if (!same)
      {
        ++res;
      }
    }
    return res;
  }

  int run(const std::string & num, const std::string & in_name, const std::string & out_name)
  {
    if (num != "1" && num != "2")
    {
      std::cerr << "bad first arg\n";
      return 1;
    }

    std::ifstream fin(in_name);
    if (!fin)
    {
      std::cerr << "can't open input\n";
      return 2;
    }

    int r = 0;
    int c = 0;
    if (!(fin >> r >> c) || r < 0 || c < 0)
    {
      std::cerr << "bad header\n";
      return 2;
    }

    if (r == 0 || c == 0)
    {
      std::ofstream fout(out_name);
      if (!fout)
      {
        std::cerr << "can't open output\n";
        return 2;
      }
      fout << "false\n";
      fout << 0 << "\n";
      return 0;
    }

    const int total = r * c;

    if (num == "1")
    {
      if (total > MAX)
      {
        std::cerr << "too big\n";
        return 2;
      }
      
      long long buf[MAX];
      for (int i = 0; i < total; ++i)
      {
        if (!(fin >> buf[i]))
        {
          std::cerr << "bad data\n";
          return 2;
        }
      }
      
      std::ofstream fout(out_name);
      if (!fout)
      {
        std::cerr << "can't open output\n";
        return 2;
      }
      fout << (upperTri(buf, r, c) ? "true" : "false") << "\n";
      fout << rowsNoSame(buf, r, c) << "\n";
      return 0;
    }

    long long * buf = new (std::nothrow) long long[total];
    if (buf == nullptr)
    {
      std::cerr << "no memory\n";
      return 2;
    }

    bool ok = true;
    for (int i = 0; i < total; ++i)
    {
      if (!(fin >> buf[i]))
      {
        ok = false;
        break;
      }
    }

    if (!ok)
    {
      delete[] buf;
      std::cerr << "bad data\n";
      return 2;
    }

    std::ofstream fout(out_name);
    if (!fout)
    {
      delete[] buf;
      std::cerr << "can't open output\n";
      return 2;
    }
    fout << (upperTri(buf, r, c) ? "true" : "false") << "\n";
    fout << rowsNoSame(buf, r, c) << "\n";
    delete[] buf;
    return 0;
  }
}

int main(int argc, char * argv[])
{
  if (argc != 4)
  {
    std::cerr << "need 3 args\n";
    return 1;
  }
  return nekhorukov::run(argv[1], argv[2], argv[3]);
}
