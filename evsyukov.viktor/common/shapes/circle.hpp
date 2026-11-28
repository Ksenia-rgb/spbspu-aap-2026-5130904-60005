#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "shape.hpp"

namespace evsyukov
{

  class Circle: public Shape
  {
  public:
    Circle(const point_t &center, double radius);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &p) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double k) override;

  private:
    point_t pos_;
    double radius_;
  };

}

#endif
