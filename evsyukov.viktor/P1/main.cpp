#include <iostream>

int main()
{
  const int invalid_input_code = 1;

  int current = 0;
  int previous = 0;
  bool has_previous = false;
  int increases = 0;
  int sign_changes = 0;

  while ((std::cin >> current) && current != 0) {
    if (has_previous) {
      if (current > previous) {
        ++increases;
      }
      if ((previous < 0 && current > 0) || (previous > 0 && current < 0)) {
        ++sign_changes;
      }
    }
    previous = current;
    has_previous = true;
  }

  if (std::cin.fail()) {
    std::cerr << "Invalid input\n";
    return invalid_input_code;
  }

  std::cout << increases << "\n" << sign_changes << "\n";
  return 0;
}
