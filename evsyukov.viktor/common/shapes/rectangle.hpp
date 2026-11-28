#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "shape.hpp"
namespace evsyukov
{
  class Rectangle: public Shape
  {
  public:
    Rectangle(const point_t &center, double width, double height);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &p) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double k) override;

  private:
    point_t pos_;
    double width_;
    double height_;
  };
}

#endif
