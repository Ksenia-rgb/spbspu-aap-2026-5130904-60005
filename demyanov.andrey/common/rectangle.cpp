#include "shapes/shape.hpp"
#include <stdexcept>

namespace demyanov
{
  Rectangle::Rectangle(double width, double height, point_t pos):
    rect_{width, height, pos}
  {
    if (width <= 0)
    {
      throw std::invalid_argument("Error: width cannot be <= 0");
    }
    if (height <= 0)
    {
      throw std::invalid_argument("Error: height cannot be <= 0");
    }
  }

  double Rectangle::getArea() const
  {
    const double s = rect_.width * rect_.height;
    return s;
  }

  rectangle_t Rectangle::getFrameRect() const
  {
    return rect_;
  }

  void Rectangle::move(double dx, double dy)
  {
    rect_.pos.x += dx;
    rect_.pos.y += dy;
  }

  void Rectangle::move(const point_t &point)
  {
    rect_.pos.x = point.x;
    rect_.pos.y = point.y;
  }

  void Rectangle::scale(double k)
  {
    if (k <= 0)
    {
      throw std::invalid_argument("Error: k cannot be <= 0");
    }
    rect_.width *= k;
    rect_.height *= k;
  }
}
