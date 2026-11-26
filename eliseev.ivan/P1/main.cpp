#include <iostream>
#include <limits>
#include <stdexcept>

namespace eliseev
{
  const int ERROR_INVALID_INPUT = 1;
  const int ERROR_SMALL_SEQUENCE = 2;

  class ISeqProperty
  {
  public:
    virtual ~ISeqProperty() = default;

    void operator()(std::ostream& os) const
    {
      result(os);
    }

    void operator()(int i)
    {
      next(i);
    }

  private:
    virtual void result(std::ostream& os) const = 0;
    virtual void next(int i) = 0;
  };

  class SubMax: public ISeqProperty
  {
  public:
    SubMax():
      max1_(std::numeric_limits< long long >::min()),
      max2_(std::numeric_limits< long long >::min()),
      count_(0)
    {}

  private:
    void result(std::ostream& os) const override
    {
      if (count_ < 2 || max2_ == std::numeric_limits< long long >::min())
      {
        throw std::runtime_error("ERROR: small posledovatelnost");
      }
      os << max2_ << "\n";
    }

    void next(int a) override
    {
      ++count_;
      if (a > max1_)
      {
        max2_ = max1_;
        max1_ = a;
      }
      else if (a > max2_)
      {
        max2_ = a;
      }
    }

    long long max1_;
    long long max2_;
    size_t count_;
  };

  class CntMax: public ISeqProperty
  {
  public:
    CntMax():
      max_(std::numeric_limits< long long >::min()),
      count_(0),
      max_count_(0)
    {}

  private:
    void result(std::ostream& os) const override
    {
      if (count_ == 0)
      {
        throw std::runtime_error("ERROR: empty sequence");
      }
      os << max_count_ << "\n";
    }

    void next(int a) override
    {
      ++count_;
      if (a > max_)
      {
        max_ = a;
        max_count_ = 1;
      }
      else if (a == max_)
      {
        ++max_count_;
      }
    }

    long long max_;
    size_t count_;
    size_t max_count_;
  };
}

int main()
{
  using namespace eliseev;

  try
  {
    SubMax sub_max;
    CntMax cnt_max;
    const size_t props_size = 2;
    ISeqProperty* props[props_size] = {&sub_max, &cnt_max};

    int value = 0;
    bool input_error = false;

    while (std::cin >> value && value != 0)
    {
      for (size_t i = 0; i < props_size; ++i)
      {
        (*props[i])(value);
      }
    }

    if (!std::cin)
    {
      input_error = true;
    }

    if (input_error)
    {
      std::cerr << "ERROR: invalid input\n";
      return ERROR_INVALID_INPUT;
    }

    int exit_code = 0;

    try
    {
      (*props[0])(std::cout);
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << "\n";
      exit_code = ERROR_SMALL_SEQUENCE;
    }

    try
    {
      (*props[1])(std::cout);
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << "\n";
      exit_code = ERROR_SMALL_SEQUENCE;
    }

    return exit_code;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return ERROR_INVALID_INPUT;
  }
}
