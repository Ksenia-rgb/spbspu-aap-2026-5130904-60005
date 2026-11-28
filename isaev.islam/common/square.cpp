#include "shapes/square.hpp"
#include "shapes/base-types.hpp"

#include <stdexcept>

isaev::Square::Square(const point_t & center, double side):
  center_(center),
  side_(side)
{
  if (side <= 0.0)
  {
    throw std::invalid_argument("Square: side must be positive");
  }
}

double isaev::Square::getArea() const
{
  return side_ * side_;
}

isaev::rectangle_t isaev::Square::getFrameRect() const
{
  return {side_, side_, center_};
}

void isaev::Square::move(const point_t & p)
{
  center_ = p;
}

void isaev::Square::move(double dx, double dy)
{
  center_.x += dx;
  center_.y += dy;
}

void isaev::Square::scale(double k)
{
  if (k <= 0.0)
  {
    throw std::invalid_argument("Square: scale factor must be positive");
  }
  side_ *= k;
}