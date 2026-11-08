#include <cctype>
#include <cstddef>
#include <exception>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace evsyukov {

  const int k_error_exit_code = 1;
  const std::size_t k_alphabet_size = 26;
  const std::size_t k_table_size =
      static_cast<std::size_t>(std::numeric_limits<unsigned char>::max()) + 1;
  const std::size_t k_top_buffer_size = 4;
  const std::size_t k_missing_buffer_size = 27;
  const std::size_t k_top_count = 3;
  const std::size_t k_start_capacity = 16;

  char * FrequentTopThree(const char * src, char * dest) noexcept {
    std::size_t counts[k_table_size] = {};
    for (const char * p = src; *p != '\0'; ++p) {
      unsigned char code = static_cast<unsigned char>(*p);
      if (!std::isspace(code)) {
        counts[code] += 1;
      }
    }
    unsigned char chosen[k_table_size] = {};
    for (std::size_t round = 0; round < k_top_count; ++round) {
      std::size_t best_count = 0;
      std::size_t best_code = 0;
      for (std::size_t k = 0; k < k_table_size; ++k) {
        if (counts[k] > best_count) {
          best_count = counts[k];
          best_code = k;
        }
      }
      if (best_count == 0) {
        break;
      }
      chosen[best_code] = 1;
      counts[best_code] = 0;
    }

    std::size_t dest_size = 0;
    for (std::size_t k = 0; k < k_table_size; ++k) {
      if (chosen[k] != 0) {
        dest[dest_size] = static_cast<char>(k);
        ++dest_size;
      }
    }
    dest[dest_size] = '\0';
    return dest;
  }

  char * MissingLetters(const char * src, char * dest) noexcept {
    unsigned char seen[k_alphabet_size] = {};
    for (const char * p = src; *p != '\0'; ++p) {
      int lower_char = std::tolower(static_cast<unsigned char>(*p));
      if (lower_char >= 'a' && lower_char <= 'z') {
        seen[lower_char - 'a'] = 1;
      }
    }

    std::size_t dest_size = 0;
    for (std::size_t i = 0; i < k_alphabet_size; ++i) {
      if (seen[i] == 0) {
        dest[dest_size] = static_cast<char>('a' + i);
        ++dest_size;
      }
    }
    dest[dest_size] = '\0';
    return dest;
  }

  void Grow(char *& buf, std::size_t & capacity, const std::size_t size) {
    if (capacity > std::numeric_limits<std::size_t>::max() / 2) {
      throw std::runtime_error("runtime_error");
    }
    std::size_t new_capacity = capacity * 2;
    char * tmp = new char[new_capacity];
    for (std::size_t i = 0; i < size; ++i) {
      tmp[i] = buf[i];
    }
    delete[] buf;
    buf = tmp;
    capacity = new_capacity;
  }

  char * ReadLine() {
    char ch;
    std::size_t capacity = k_start_capacity;
    std::size_t size = 0;
    char * buf = new char[capacity];
    try {
      while (std::cin.get(ch) && ch != '\n') {
        if (size + 1 >= capacity) {
          Grow(buf, capacity, size);
        }
        buf[size++] = ch;
      }
      buf[size] = '\0';
    } catch (...) {
      delete[] buf;
      throw;
    }
    return buf;
  }

  int Run() {
    int exit_code = 0;
    char * line = nullptr;
    char top_result[k_top_buffer_size];
    char missing_result[k_missing_buffer_size];
    try {
      line = ReadLine();
      std::cout << FrequentTopThree(line, top_result) << '\n';
      std::cout << MissingLetters(line, missing_result) << '\n';
    } catch (const std::exception & e) {
      std::cerr << e.what() << '\n';
      exit_code = k_error_exit_code;
    }
    delete[] line;
    return exit_code;
  }

}

int main() {
  return evsyukov::Run();
}
