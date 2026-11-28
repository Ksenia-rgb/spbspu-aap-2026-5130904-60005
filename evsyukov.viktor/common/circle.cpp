#include "shapes/circle.hpp"
#include <stdexcept>

namespace evsyukov
{
  const double k_pi = 3.14159265358979323846;
  const double k_diameter_factor = 2.0;
  Circle::Circle(const point_t &center, double radius):
    pos_(center),
    radius_(radius)
  {
    if (radius <= 0.0)
    {
      throw std::invalid_argument("Circle: radius must be positive");
    }
  }
  double Circle::getArea() const noexcept
  {
    return k_pi * radius_ * radius_;
  }
  rectangle_t Circle::getFrameRect() const noexcept
  {
    return {k_diameter_factor * radius_, k_diameter_factor * radius_, pos_};
  }
  void Circle::move(const point_t &p) noexcept
  {
    pos_ = p;
  }
  void Circle::move(double dx, double dy) noexcept
  {
    pos_.x += dx;
    pos_.y += dy;
  }
  void Circle::scale(double k)
  {
    if (k <= 0)
    {
      throw std::invalid_argument("Circle: scale factor must be positive");
    }
    radius_ *= k;
  }
}
