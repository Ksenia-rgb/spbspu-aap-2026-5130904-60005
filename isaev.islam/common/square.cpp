#include "shapes/square.hpp"
#include "shapes/base-types.hpp"

#include <cmath>
#include <stdexcept>

namespace
{
  void validate(const isaev::point_t & center, double side)
  {
    const double area = side * side;
    const bool finite = std::isfinite(center.x) && std::isfinite(center.y) &&
      std::isfinite(side) && std::isfinite(area);
    if (!finite || !(side > 0.0) || !(area > 0.0))
    {
      throw std::invalid_argument("Square: invalid or overflowing parameters");
    }
  }
}

isaev::Square::Square(const point_t & center, double side):
  center_(center),
  side_(side)
{
  validate(center_, side_);
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
  validate(p, side_);
  center_ = p;
}

void isaev::Square::move(double dx, double dy)
{
  const point_t c{center_.x + dx, center_.y + dy};
  validate(c, side_);
  center_ = c;
}

void isaev::Square::scale(double k)
{
  if (!(k > 0.0) || !std::isfinite(k))
  {
    throw std::invalid_argument("Square: scale factor must be positive and finite");
  }
  const double side = side_ * k;
  validate(center_, side);
  side_ = side;
}