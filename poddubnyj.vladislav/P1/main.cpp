#include <iostream>
#include <stdexcept>

const int k_max_len = 10000;
const int k_min_len_for_local_max = 3;

int findMax(const int *arr, const int len)
{
  if (len < 1) {
    throw std::out_of_range("Bro, length of arr must belong to the interval");
  }

  int max_value = arr[0];
  for (int i = 0; i < len; ++i) {
    if (max_value < arr[i]) {
      max_value = arr[i];
    }
  }

  return max_value;
}

int countLocalMax(const int *arr, const int len)
{
  if (len < k_min_len_for_local_max) {
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

int main()
{
  int arr[k_max_len];
  int size = 0;
  int value = -1;

  try {
    while (value != 0) {
      std::cin >> value;
      if (std::cin.fail()) {
        throw std::invalid_argument("Bro, we were waiting whole integer");
      }

      if (value != 0) {
        if (size >= k_max_len) {
          throw std::out_of_range("Bro, length of arr must belong to the interval");
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

    if (size >= k_min_len_for_local_max) {
      std::cout << "Count of local maximums: " << countLocalMax(arr, size) << std::endl;
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
