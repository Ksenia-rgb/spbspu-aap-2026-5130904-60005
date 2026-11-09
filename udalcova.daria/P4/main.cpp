#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>

namespace udalcova {
  void exclude_second(const char *first, const char *second, char *dst) noexcept
  {
    size_t k = 0;
    for (size_t i = 0; first[i] != '\0'; ++i) {
      if (std::strchr(second, first[i]) == nullptr) {
        dst[k] = first[i];
        ++k;
      }
    }
    dst[k] = '\0';
  }

  void upper(const char *src, char *dst) noexcept
  {
    size_t i = 0;
    for (; src[i] != '\0'; ++i) {
      dst[i] = static_cast< char >(std::tolower(static_cast< unsigned char >(src[i])));
    }
    dst[i] = '\0';
  }

  char *read_line(std::istream &in)
  {
    constexpr size_t cfpfcity = 16;
    size_t cap = cfpfcity;
    size_t len = 0;
    char *buf = static_cast< char * >(std::malloc(cap));
    char c = 0;
    for (; buf != nullptr && in.get(c) && c != '\n';) {
      if (len + 1 == cap) {
        cap *= 2;
        char *const tmp = static_cast< char * >(std::realloc(buf, cap));
        if (tmp == nullptr) {
          std::free(buf);
          return nullptr;
        }
        buf = tmp;
      }
      buf[len] = c;
      ++len;
    }
    if (buf != nullptr) {
      buf[len] = '\0';
    }
    return buf;
  }

  int process(std::istream &in, std::ostream &out, std::ostream &err)
  {
    if (in.peek() == std::istream::traits_type::eof()) {
      err << "Входные данные не предоставлены\n";
      return 1;
    }
    char *src = read_line(in);
    char *res = nullptr;
    if (src != nullptr) {
      res = static_cast< char * >(std::malloc(std::strlen(src) + 1));
    }
    if (res == nullptr) {
      err << "не удалось выделить памят\n";
      std::free(const_cast< char * >(src));
      return 1;
    }
    exclude_second(src, "abc", res);
    out << res << "\n";
    upper(src, res);
    out << res << "\n";

    std::free(res);
    std::free(const_cast< char * >(src));
    return 0;
  }
}

int main()
{
  return udalcova::process(std::cin, std::cout, std::cerr);
}

