#include <shapes/shape.hpp>
#include <cmath>
#include <stdexcept>

namespace demyanov {
Rubber::Rubber(double bigRadius, double smallRadius, point_t bigCenter,
               point_t smallCenter)
    : bigRadius_(bigRadius), smallRadius_(smallRadius), bigCenter_(bigCenter),
      smallCenter_(smallCenter) {
  double dx = bigCenter_.x - smallCenter_.x;
  double dy = bigCenter_.y - smallCenter_.y;
  double distance = std::sqrt(dx * dx + dy * dy);

  if (bigRadius_ <= 0 || smallRadius_ <= 0 || smallRadius_ >= bigRadius_ ||
      distance == 0 || distance + smallRadius_ > bigRadius_) {
    throw std::invalid_argument("Invalid Rubber parameters");
  }
}

double Rubber::getArea() const {
  double R = bigRadius_;
  double r = smallRadius_;
  double s = 3.141592653589793 * (R * R - r * r);
  return s;
}

rectangle_t Rubber::getFrameRect() const {
  double width = 2 * bigRadius_;
  double height = 2 * bigRadius_;
  point_t pos = bigCenter_;

  return {width, height, pos};
}

void Rubber::move(double dx, double dy) {
  bigCenter_.x += dx;
  bigCenter_.y += dy;
  smallCenter_.x += dx;
  smallCenter_.y += dy;
}

void Rubber::move(const point_t &point) {
  double dx = point.x - smallCenter_.x;
  double dy = point.y - smallCenter_.y;
  move(dx, dy);
}

void Rubber::scale(double k) {
  if (k <= 0) {
    throw std::invalid_argument("Invalid scale coefficient");
  }

  bigRadius_ *= k;
  smallRadius_ *= k;
  double dx = bigCenter_.x - smallCenter_.x;
  double dy = bigCenter_.y - smallCenter_.y;
  bigCenter_.x = smallCenter_.x + dx * k;
  bigCenter_.y = smallCenter_.y + dy * k;
}
} // namespace demyanov
