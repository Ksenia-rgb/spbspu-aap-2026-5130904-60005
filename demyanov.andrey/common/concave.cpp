#include <shapes/shape.hpp>
#include <cmath>
#include <stdexcept>

namespace demyanov
{
  Concave::Concave(point_t p1, point_t p2, point_t p3, point_t p4):
    p1_(p1),
    p2_(p2),
    p3_(p3),
    p4_(p4)
  {}

  double Concave::getArea() const
  {
    const double area = std::abs(p1_.x * p2_.y + p2_.x * p3_.y + p3_.x * p4_.y + p4_.x * p1_.y - p1_.y * p2_.x
                            - p2_.y * p3_.x - p3_.y * p4_.x - p4_.y * p1_.x)
        / 2;
    return area;
  }

  rectangle_t Concave::getFrameRect() const
  {
    double min_x = p1_.x;
    if (min_x > p2_.x)
    {
      min_x = p2_.x;
    }
    if (min_x > p3_.x)
    {
      min_x = p3_.x;
    }
    if (min_x > p4_.x)
    {
      min_x = p4_.x;
    }

    double min_y = p1_.y;
    if (min_y > p2_.y)
    {
      min_y = p2_.y;
    }
    if (min_y > p3_.y)
    {
      min_y = p3_.y;
    }
    if (min_y > p4_.y)
    {
      min_y = p4_.y;
    }

    double max_x = p1_.x;
    if (max_x < p2_.x)
    {
      max_x = p2_.x;
    }
    if (max_x < p3_.x)
    {
      max_x = p3_.x;
    }
    if (max_x < p4_.x)
    {
      max_x = p4_.x;
    }

    double max_y = p1_.y;
    if (max_y < p2_.y)
    {
      max_y = p2_.y;
    }
    if (max_y < p3_.y)
    {
      max_y = p3_.y;
    }
    if (max_y < p4_.y)
    {
      max_y = p4_.y;
    }

    const double width = max_x - min_x;
    const double height = max_y - min_y;
    const point_t pos = {(min_x + max_x) / 2, (min_y + max_y) / 2};

    return rectangle_t{width, height, pos};
  }

  void Concave::move(double dx, double dy)
  {
    p1_.x += dx;
    p1_.y += dy;

    p2_.x += dx;
    p2_.y += dy;

    p3_.x += dx;
    p3_.y += dy;

    p4_.x += dx;
    p4_.y += dy;
  }

  void Concave::move(const point_t &point)
  {
    const double dx = point.x - p1_.x;
    const double dy = point.y - p1_.y;
    move(dx, dy);
  }

  void Concave::scale(double k)
  {
    if (k <= 0)
    {
      throw std::invalid_argument("Invalid scale coefficient");
    }

    const double dx2 = p2_.x - p1_.x;
    const double dy2 = p2_.y - p1_.y;
    p2_.x = p1_.x + dx2 * k;
    p2_.y = p1_.y + dy2 * k;

    const double dx3 = p3_.x - p1_.x;
    const double dy3 = p3_.y - p1_.y;
    p3_.x = p1_.x + dx3 * k;
    p3_.y = p1_.y + dy3 * k;

    const double dx4 = p4_.x - p1_.x;
    const double dy4 = p4_.y - p1_.y;
    p4_.x = p1_.x + dx4 * k;
    p4_.y = p1_.y + dy4 * k;
  }
}
