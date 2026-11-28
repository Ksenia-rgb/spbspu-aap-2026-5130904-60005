#include <shapes/shape.hpp>
#include <cmath>
#include <stdexcept>

namespace demyanov
{
  Rubber::Rubber(double big_radius, double small_radius, point_t big_center, point_t small_center):
    big_radius_(big_radius),
    small_radius_(small_radius),
    big_center_(big_center),
    small_center_(small_center)
  {
    const double dx = big_center_.x - small_center_.x;
    const double dy = big_center_.y - small_center_.y;
    const double distance = std::sqrt(dx * dx + dy * dy);

    if (big_radius_ <= 0 || small_radius_ <= 0 || small_radius_ >= big_radius_ || distance == 0
        || distance + small_radius_ > big_radius_)
    {
      throw std::invalid_argument("Invalid Rubber parameters");
    }
  }

  double Rubber::getArea() const
  {
    constexpr double pi = 3.141592653589793;
    const double area = pi * (big_radius_ * big_radius_ - small_radius_ * small_radius_);
    return area;
  }

  rectangle_t Rubber::getFrameRect() const
  {
    const double width = 2 * big_radius_;
    const double height = 2 * big_radius_;
    const point_t pos = big_center_;

    return {width, height, pos};
  }

  void Rubber::move(double dx, double dy)
  {
    big_center_.x += dx;
    big_center_.y += dy;
    small_center_.x += dx;
    small_center_.y += dy;
  }

  void Rubber::move(const point_t &point)
  {
    const double dx = point.x - small_center_.x;
    const double dy = point.y - small_center_.y;
    move(dx, dy);
  }

  void Rubber::scale(double k)
  {
    if (k <= 0)
    {
      throw std::invalid_argument("Invalid scale coefficient");
    }

    big_radius_ *= k;
    small_radius_ *= k;

    const double dx = big_center_.x - small_center_.x;
    const double dy = big_center_.y - small_center_.y;
    big_center_.x = small_center_.x + dx * k;
    big_center_.y = small_center_.y + dy * k;
  }
}
