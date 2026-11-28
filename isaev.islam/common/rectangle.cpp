#include "shapes/rectangle.hpp"
#include "shapes/base-types.hpp"

#include <stdexcept>

isaev::Rectangle::Rectangle(const point_t & center, double width, double height):
  rect_{width, height, center}
{
  if (width <= 0.0 || height <= 0.0)
  {
    throw std::invalid_argument("Rectangle: width and height must be positive");
  }
}

isaev::Rectangle::Rectangle(const point_t & leftBottom, const point_t & rightTop):
  rect_{rightTop.x - leftBottom.x, rightTop.y - leftBottom.y,
    {(leftBottom.x + rightTop.x) / 2.0, (leftBottom.y + rightTop.y) / 2.0}}
{
  if (rect_.width <= 0.0 || rect_.height <= 0.0)
  {
    throw std::invalid_argument("Rectangle: right-top must be above and to the right of left-bottom");
  }
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
  rect_.pos = p;
}

void isaev::Rectangle::move(double dx, double dy)
{
  rect_.pos.x += dx;
  rect_.pos.y += dy;
}

void isaev::Rectangle::scale(double k)
{
  if (k <= 0.0)
  {
    throw std::invalid_argument("Rectangle: scale factor must be positive");
  }
  rect_.width *= k;
  rect_.height *= k;
}