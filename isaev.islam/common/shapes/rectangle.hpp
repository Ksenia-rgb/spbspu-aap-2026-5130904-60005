#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "shape.hpp"
#include "base-types.hpp"

namespace isaev
{
  class Rectangle final: public Shape
  {
  public:
    Rectangle(const point_t & center, double width, double height);
    Rectangle(const point_t & leftBottom, const point_t & rightTop);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(const point_t & p) override;
    void move(double dx, double dy) override;
    void scale(double k) override;
  private:
    rectangle_t rect_;
  };
}

#endif
