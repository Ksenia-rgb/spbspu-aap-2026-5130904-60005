#include <iostream>

int main() {
  int max_len = 0;
  int current_len = 0;
  int prev = 0;
  bool has_prev = false;
  int current;
  bool zero_found = false;

  while (std::cin >> current) {
    if (current == 0) {
      zero_found = true;
      break;
    }

    if (!has_prev) {
      current_len = 1;
      has_prev = true;
    } else {
      if (current >= prev) {
        current_len++;
      } else {
        if (current_len > max_len) {
          max_len = current_len;
        }
        current_len = 1;
      }
    }
    prev = current;
  }

  if (std::cin.fail() && !std::cin.eof()) {
    std::cerr << "Error: Invalid input data. Expected integers." << std::endl;
    return 1;
  }

  if (!zero_found && !std::cin.eof()) {
    std::cerr << "Error: Invalid input data. Sequence must end with 0."
              << std::endl;
    return 1;
  }

  if (current_len > max_len) {
    max_len = current_len;
  }

  std::cout << max_len << std::endl;

  return 0;
}