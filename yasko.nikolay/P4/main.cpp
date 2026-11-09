#include <cctype>
#include <cstring>
#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <limits>

namespace yasko
{
  const int succes_ec = 0;
  const int memory_ec = 1;
  const int invalid_input_ec = 1;
  const std::size_t initial_capacity = 16;
  char *readString(std::istream &inp, std::size_t &len);
  char *newStringLower(char *data, char *new_data);
  int repeatingDigits(char *data);

}
char *yasko::readString(std::istream &inp, std::size_t &len)
{
  len = 0;
  std::size_t capacity = initial_capacity;
  char *s_buf = static_cast< char * >(std::malloc(capacity));
  char c = '\0';
  if (s_buf == nullptr)
  {
    return nullptr;
  }
  while (inp.get(c) && c != '\n')
  {
    if (len == capacity - 1)
    {
      if (capacity > std::numeric_limits< std::size_t >::max() - 10)
      {
        len = 0;
        std::free(s_buf);
        return nullptr;
      }
      const std::size_t new_capacity = capacity + 10;
      char *const new_buf = static_cast< char * >(std::malloc(new_capacity));
      if (new_buf == nullptr)
      {
        std::free(s_buf);
        return nullptr;
      }
      std::memcpy(new_buf, s_buf, len);
      std::free(s_buf);
      s_buf = new_buf;
      capacity = new_capacity;
    }
    s_buf[len++] = c;
  }
  s_buf[len] = '\0';
  return s_buf;
}
char *yasko::newStringLower(char *data, char *new_data)
{
  std::size_t j = 0;
  for (std::size_t i = 0; data[i] != '\0'; i++)
  {
    if (std::isupper(static_cast< unsigned char >(data[i])))
    {
      new_data[j++] = static_cast< char >(std::tolower(static_cast< unsigned char >(data[i])));
    }
    else
    {
      new_data[j++] = data[i];
    }
  }
  new_data[j] = '\0';
  return new_data;
}
int yasko::repeatingDigits(char *data)
{
  int count_num[10]{};
  for (std::size_t i = 0; data[i] != '\0'; i++)
  {
    if (std::isdigit(static_cast< unsigned char >(data[i])))
    {
      const int num = static_cast< int >(data[i] - '0');
      count_num[num]++;
      if (count_num[num] > 1)
      {
        return 1;
      }
    }
  }
  return 0;
}
int main()
{
  std::size_t len = 0;
  char *const data = yasko::readString(std::cin, len);
  if (data == nullptr)
  {
    std::cerr << "Memory error\n";
    return yasko::memory_ec;
  }
  if (len == 0 && !std::cin)
  {
    std::free(data);
    std::cerr << "Invalid input\n";
    return yasko::invalid_input_ec;
  }
  char *const new_data = static_cast< char * >(std::malloc(len + 1));
  if (new_data == nullptr)
  {
    std::cerr << "Memory error\n";
    std::free(data);
    return yasko::memory_ec;
  }
  yasko::newStringLower(data, new_data);
  std::cout << "[UPP-LOW] = " << new_data << std::endl;
  const int repeat_digits = yasko::repeatingDigits(data);
  if (repeat_digits == 1)
  {
    std::cout << "[REP-DGT] = " << 1 << std::endl;
  }
  else
  {
    std::cout << "[REP-DGT] = " << 0 << std::endl;
  }
  std::free(data);
  std::free(new_data);
  return yasko::succes_ec;
}
