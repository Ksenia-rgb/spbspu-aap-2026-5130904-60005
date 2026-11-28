#include "shapes/rectangle.hpp"
#include <stdexcept>

namespace evsyukov
{

  Rectangle::Rectangle(const point_t &center, double width, double height):
    pos_(center),
    width_(width),
    height_(height)
  {
    if (width <= 0.0 || height <= 0.0)
    {
      throw std::invalid_argument("Rectangle: woght or height must be positive");
    }
  }

  double Rectangle::getArea() const noexcept
  {
    return width_ * height_;
  }

  rectangle_t Rectangle::getFrameRect() const noexcept
  {
    return {width_, height_, pos_};
  }

  void Rectangle::move(const point_t &p) noexcept
  {
    pos_ = p;
  }

  void Rectangle::move(double dx, double dy) noexcept
  {
    pos_.x += dx;
    pos_.y += dy;
  }

  void Rectangle::scale(double k)
  {
    if (k <= 0)
    {
      throw std::invalid_argument("Rectangle: scale factor must be positive");
    }
    width_ *= k;
    height_ *= k;
  }
}

