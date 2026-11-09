#include <iostream>
#include <cstring>
#include <cctype>
#include <new>

namespace kirillin
{
  const int ret_ok = 0;
  const int ret_data_error = 1;

  char *readDynamicString()
  {
    size_t capacity = 16;
    size_t len = 0;
    char *buffer = new (std::nothrow) char[capacity];
    if (buffer == nullptr)
    {
      return nullptr;
    }

    char c = 0;
    while (std::cin.get(c) && c != '\n')
    {
      if (len + 1 >= capacity)
      {
        capacity *= 2;
        char *new_buffer = new (std::nothrow) char[capacity];
        if (new_buffer == nullptr)
        {
          delete[] buffer;
          return nullptr;
        }
        std::strcpy(new_buffer, buffer);
        delete[] buffer;
        buffer = new_buffer;
      }
      buffer[len++] = c;
    }
    buffer[len] = '\0';
    return buffer;
  }

  void Variant3(const char *src, char *res)
  {
    if (!src || !res)
    {
      return;
    }
    size_t j = 0;

    for (size_t i = 0; src[i] != '\0'; ++i)
    {
      unsigned char uc = static_cast< unsigned char >(src[i]);

      if (!std::isalpha(uc))
      {
        res[j++] = src[i];
      }
    }
    res[j] = '\0';
  }

  int Variant7(const char *src)
  {
    int count = 0;
    if (!src)
    {
      return count;
    }

    int seen[256] = {0};

    for (size_t i = 0; src[i] != '\0'; ++i)
    {
      unsigned char uc = static_cast< unsigned char >(src[i]);

      if (std::isalpha(uc))
      {
        if (uc < 128)
        {
          if (seen[uc] == 0)
          {
            seen[uc] = 1;
            count++;
          }
        }
      }
    }
    return count;
  }
}

int main()
{
  char *str = kirillin::readDynamicString();
  if (!str)
  {
    std::cerr << "Memory allocation failed" << std::endl;
    return kirillin::ret_data_error;
  }

  size_t len = std::strlen(str);
  char *var3res = new (std::nothrow) char[len + 1];
  if (!var3res)
  {
    std::cerr << "Memory allocation failed" << std::endl;
    delete[] str;
    return kirillin::ret_data_error;
  }

  kirillin::Variant3(str, var3res);
  std::cout << var3res << std::endl;
  delete[] var3res;

  int var7res = kirillin::Variant7(str);
  std::cout << var7res << std::endl;

  delete[] str;

  return kirillin::ret_ok;
}
