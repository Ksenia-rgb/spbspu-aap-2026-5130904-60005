#include <cstddef>
#include <iostream>

namespace andrijchuk {
  using l_l = long long int;

  class LocMinCnt {
  public:
    void nextN(const l_l n) noexcept
    {
      if (k_ >= 2 && pr_prev_n_ > prev_n_ && prev_n_ < n) {
        count_++;
      }
      pr_prev_n_ = prev_n_;
      prev_n_ = n;
      k_++;
    }

    bool isValid() const noexcept
    {
      return k_ >= 1;
    }

    void print(std::ostream &out, std::ostream &err) const noexcept
    {
      if (isValid()) {
        out << count_ << "\n";
      } else {
        err << "Incorrect sequence for loc_min.\n";
      }
    }

  private:
    l_l count_ = 0;
    l_l prev_n_ = 0;
    l_l pr_prev_n_ = 0;
    std::size_t k_ = 0;
  };

  class SumDupCnt {
  public:
    void nextN(const l_l n) noexcept
    {
      if (k_ >= 2 && n == pr_prev_n_ + prev_n_) {
        count_++;
      }
      pr_prev_n_ = prev_n_;
      prev_n_ = n;
      k_++;
    }

    bool isValid() const noexcept
    {
      return k_ >= 3;
    }

    void print(std::ostream &out, std::ostream &err) const noexcept
    {
      if (isValid()) {
        out << count_ << "\n";
      } else {
        err << "Incorrect sequence for sum_dup.\n";
      }
    }

  private:
    l_l count_ = 0;
    l_l prev_n_ = 0;
    l_l pr_prev_n_ = 0;
    std::size_t k_ = 0;
  };

  int process(std::istream &in, std::ostream &out, std::ostream &err)
  {
    const int inp_err_exitcode1 = 1;
    const int seq_err_exitcode2 = 2;
    LocMinCnt loc_min;
    SumDupCnt sum_dup;
    l_l n = 0;
    while (in >> n && n != 0) {
      loc_min.nextN(n);
      sum_dup.nextN(n);
    }
    if (!in) {
      err << "Incorrect Arguments.\n";
      return inp_err_exitcode1;
    }
    loc_min.print(out, err);
    sum_dup.print(out, err);
    if (!loc_min.isValid() || !sum_dup.isValid()) {
      return seq_err_exitcode2;
    }
    return 0;
  }
}

int main()
{
  return andrijchuk::process(std::cin, std::cout, std::cerr);
}
