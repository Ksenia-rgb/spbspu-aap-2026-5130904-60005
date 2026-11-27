#ifndef CONCAVE_HPP
#define CONCAVE_HPP

#include <shapes/shape.hpp>
#include <cstddef>

namespace evsyukov {

  class Concave: public Shape{
    public:
      Concave(const point_t &a, const point_t &b, const point_t &c, const point_t &d);
      double getArea() const noexcept override;
      rectangle_t getFrameRect() const noexcept override;
      void move(const point_t &p) noexcept override;
      void move(double dx, double dy) noexcept override;
      void scale(double k) override;
    private:
      point_t v_[4];
      size_t concave_;
  };

}

#endif // CONCAVE_HPP
