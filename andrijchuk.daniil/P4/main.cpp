#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <new>

namespace andrijchuk

{
  const int memory_error_exitcode1 = 1;
  const int error_exitcode2 = 2;
  const char rpl_sym1 = 'c';
  const char rpl_sym2 = 'b';

  std::size_t strLen(const char *cstr)
  {
    std::size_t len = 0;
    while (cstr[len] != '\0') {
      len++;
    }
    return len;
  }

  char *readStr()
  {
    std::size_t cstr_len = 0;
    char c = 0;
    char *cstr = nullptr;
    while (std::cin.get(c) && c != '\n') {
      char *const temp = static_cast< char * >(std::malloc(cstr_len + 2));
      if (temp == nullptr) {
        std::free(cstr);
        throw std::bad_alloc();
      }
      for (std::size_t i = 0; i < cstr_len; i++) {
        temp[i] = cstr[i];
      }
      std::free(cstr);
      cstr = temp;
      cstr[cstr_len++] = c;
    }
    if (cstr == nullptr) {
      cstr = static_cast< char * >(std::malloc(1));
      if (cstr == nullptr) {
        throw std::bad_alloc();
      }
    }
    cstr[cstr_len] = '\0';
    return cstr;
  }

  char *strForRplSym(std::size_t cstr_len)
  {
    char *const rpl_cstr = static_cast< char * >(std::malloc(cstr_len + 1));
    if (rpl_cstr == nullptr) {
      throw std::bad_alloc();
    }
    return rpl_cstr;
  }

  char *rplSym(char *rpl_cstr, const char *cstr, char sym1, char sym2)
  {
    std::size_t i = 0;
    for (i = 0; cstr[i] != '\0'; i++) {
      if (cstr[i] == sym1) {
        rpl_cstr[i] = sym2;
      } else {
        rpl_cstr[i] = cstr[i];
      }
    }
    rpl_cstr[i] = '\0';
    return rpl_cstr;
  }

  int seqSym(const char *cstr)
  {
    for (std::size_t i = 0; cstr[i] != '\0' && cstr[i + 1] != '\0'; i++) {
      if (cstr[i] == cstr[i + 1]) {
        return 1;
      }
    }
    return 0;
  }
}

int main()
{
  namespace ad = andrijchuk;
  std::size_t cstr_len = 0;
  char *cstr = nullptr;
  char *rpl_cstr = nullptr;
  int seq_sym_flag = 0;
  try {
    cstr = ad::readStr();
    cstr_len = ad::strLen(cstr);

    if (cstr_len == 0) {
      std::cerr << "Empty input";
      std::free(cstr);
      return ad::error_exitcode2;
    }

    rpl_cstr = ad::strForRplSym(cstr_len);
    ad::rplSym(rpl_cstr, cstr, ad::rpl_sym1, ad::rpl_sym2);
    seq_sym_flag = ad::seqSym(cstr);
  } catch (const std::bad_alloc &) {
    std::cerr << "Memory error" << '\n';
    std::free(cstr);
    std::free(rpl_cstr);
    return ad::memory_error_exitcode1;
  }
  std::cout << rpl_cstr << '\n';
  std::cout << seq_sym_flag << '\n';
  std::free(cstr);
  std::free(rpl_cstr);
  return 0;
}
