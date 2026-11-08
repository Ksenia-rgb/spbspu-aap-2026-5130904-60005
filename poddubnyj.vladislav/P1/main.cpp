#include <cstddef>
#include <iostream>

namespace poddubnyj {

  const int SUCCESS = 0;
  const int NOT_SEQ = 1;
  const int IMPOSE_CALC = 2;

  class SeqStats {
  public:
    int run()
    {
      int x = 0;

      if (!(std::cin >> x)) {
        std::cerr << "error: it is not sequence" << "\n";
        return NOT_SEQ;
      }

      if (x == 0) {
        std::cout << 0;
        std::cerr << "error_maxi: empty sequence" << "\n";
        return IMPOSE_CALC;
      }

      curr_min_ = x;
      curr_max_ = x;
      cnt_min_ = 1;
      cnt_max_ = 1;

      bool correct_complete = false;

      while (std::cin >> x) {
        if (x == 0) {
          correct_complete = true;
          break;
        }
        process(x);
      }

      if (!correct_complete && std::cin.fail()) {
        std::cerr << "error: it is not sequence" << "\n";
        return NOT_SEQ;
      }

      print();
      return SUCCESS;
    }

  private:
    void process(int x)
    {
      if (x == curr_min_) {
        ++cnt_min_;
      }
      if (x == curr_max_) {
        ++cnt_max_;
      }
      if (x < curr_min_) {
        cnt_min_ = 1;
        curr_min_ = x;
      }
      if (x > curr_max_) {
        cnt_max_ = 1;
        curr_max_ = x;
      }
    }

    void print() const
    {
      std::cout << cnt_min_ << "\n";
      std::cout << cnt_max_ << "\n";
    }

    std::size_t cnt_min_ = 0;
    std::size_t cnt_max_ = 0;
    int curr_min_ = 0;
    int curr_max_ = 0;
  };

}

int main()
{
  poddubnyj::SeqStats stats;
  return stats.run();
}
