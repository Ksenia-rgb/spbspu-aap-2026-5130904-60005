#include "shapes/rectangle.hpp"
#include "shapes/base-types.hpp"

#include <cmath>
#include <stdexcept>

namespace
{
  void validate(const isaev::rectangle_t & r)
  {
    const double area = r.width * r.height;
    const bool finite = std::isfinite(r.pos.x) && std::isfinite(r.pos.y) &&
      std::isfinite(r.width) && std::isfinite(r.height) && std::isfinite(area);
    if (!finite || !(r.width > 0.0) || !(r.height > 0.0) || !(area > 0.0))
    {
      throw std::invalid_argument("Rectangle: invalid or overflowing parameters");
    }
  }
}

isaev::Rectangle::Rectangle(const point_t & center, double width, double height):
  rect_{width, height, center}
{
  validate(rect_);
}

isaev::Rectangle::Rectangle(const point_t & leftBottom, const point_t & rightTop):
  rect_{rightTop.x - leftBottom.x, rightTop.y - leftBottom.y,
    {(leftBottom.x + rightTop.x) / 2.0, (leftBottom.y + rightTop.y) / 2.0}}
{
  validate(rect_);
}

double isaev::Rectangle::getArea() const
{
  return rect_.width * rect_.height;
}

isaev::rectangle_t isaev::Rectangle::getFrameRect() const
{
  return rect_;
}

void isaev::Rectangle::move(const point_t & p)
{
  rectangle_t r = rect_;
  r.pos = p;
  validate(r);
  rect_ = r;
}

void isaev::Rectangle::move(double dx, double dy)
{
  rectangle_t r = rect_;
  r.pos.x += dx;
  r.pos.y += dy;
  validate(r);
  rect_ = r;
}

void isaev::Rectangle::scale(double k)
{
  if (!(k > 0.0) || !std::isfinite(k))
  {
    throw std::invalid_argument("Rectangle: scale factor must be positive and finite");
  }
  rectangle_t r = rect_;
  r.width *= k;
  r.height *= k;
  validate(r);
  rect_ = r;
}
