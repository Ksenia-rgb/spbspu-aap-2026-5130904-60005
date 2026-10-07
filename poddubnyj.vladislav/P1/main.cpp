#include <iostream>
#include <stdexcept>

const int kMaxLen = 10000;
const int kMinLenForLocalMax = 3;

int findMax(const int *arr, const int len) {
  if (len < 1) {
    throw std::out_of_range("Bro, length of arr must belong to the interval");
  }

  int maxValue = arr[0];
  for (int i = 0; i < len; ++i) {
    if (maxValue < arr[i]) {
      maxValue = arr[i];
    }
  }

  return maxValue;
}

int countLocalMax(const int *arr, const int len) {
  if (len < kMinLenForLocalMax) {
    throw std::out_of_range("Bro, length of arr must belong to the interval");
  }

  int count = 0;
  for (int i = 1; i < len - 1; ++i) {
    if ((arr[i - 1] < arr[i]) && (arr[i] > arr[i + 1])) {
      ++count;
    }
  }

  return count;
}

int main() {
  int arr[kMaxLen];
  int size = 0;
  int value = -1;

  try {
    while (value != 0) {
      std::cin >> value;
      if (std::cin.fail()) {
        throw std::invalid_argument("Bro, we were waiting whole integer");
      }

      if (value != 0) {
        if (size >= kMaxLen) {
          throw std::out_of_range(
              "Bro, length of arr must belong to the interval");
        }

        arr[size] = value;
        ++size;
      }
    }

    if (size > 0) {
      std::cout << "Maximum: " << findMax(arr, size) << std::endl;
    } else {
      throw std::out_of_range("Bro, length of arr must belong to the interval");
    }

    if (size >= kMinLenForLocalMax) {
      std::cout << "Count of local maximums: " << countLocalMax(arr, size)
                << std::endl;
    } else {
      throw std::out_of_range("Bro, length of arr must belong to the interval");
    }
  } catch (const std::invalid_argument &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch (const std::out_of_range &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
