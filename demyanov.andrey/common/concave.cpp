#include <shapes/shape.hpp>
#include <cmath>
#include <stdexcept>

namespace demyanov {
Concave::Concave(point_t p1, point_t p2, point_t p3, point_t p4)
    : p1_(p1), p2_(p2), p3_(p3), p4_(p4) {}

double Concave::getArea() const {
  double s =
      std::abs(p1_.x * p2_.y + p2_.x * p3_.y + p3_.x * p4_.y + p4_.x * p1_.y -
               p1_.y * p2_.x - p2_.y * p3_.x - p3_.y * p4_.x - p4_.y * p1_.x) /
      2;
  return s;
}

rectangle_t Concave::getFrameRect() const {
  double minX = p1_.x;
  if (minX > p2_.x) {
    minX = p2_.x;
  }
  if (minX > p3_.x) {
    minX = p3_.x;
  }
  if (minX > p4_.x) {
    minX = p4_.x;
  }

  double minY = p1_.y;
  if (minY > p2_.y) {
    minY = p2_.y;
  }
  if (minY > p3_.y) {
    minY = p3_.y;
  }
  if (minY > p4_.y) {
    minY = p4_.y;
  }

  double maxX = p1_.x;
  if (maxX < p2_.x) {
    maxX = p2_.x;
  }
  if (maxX < p3_.x) {
    maxX = p3_.x;
  }
  if (maxX < p4_.x) {
    maxX = p4_.x;
  }

  double maxY = p1_.y;
  if (maxY < p2_.y) {
    maxY = p2_.y;
  }
  if (maxY < p3_.y) {
    maxY = p3_.y;
  }
  if (maxY < p4_.y) {
    maxY = p4_.y;
  }

  double width = maxX - minX;
  double height = maxY - minY;

  point_t pos = {0, 0};
  pos.x = (minX + maxX) / 2;
  pos.y = (minY + maxY) / 2;

  return rectangle_t{width, height, pos};
}

void Concave::move(double dx, double dy) {
  p1_.x += dx;
  p1_.y += dy;

  p2_.x += dx;
  p2_.y += dy;

  p3_.x += dx;
  p3_.y += dy;

  p4_.x += dx;
  p4_.y += dy;
}

void Concave::move(const point_t &point) {
  double dx = point.x - p1_.x;
  double dy = point.y - p1_.y;
  move(dx, dy);
}

void Concave::scale(double k) {
  if (k <= 0) {
    throw std::invalid_argument("Invalid scale coefficient");
  }
  double dx = p2_.x - p1_.x;
  double newX = p1_.x + dx * k;

  double dy = p2_.y - p1_.y;
  double newY = p1_.y + dy * k;

  p2_.x = newX;
  p2_.y = newY;

  dx = p3_.x - p1_.x;
  newX = p1_.x + dx * k;

  dy = p3_.y - p1_.y;
  newY = p1_.y + dy * k;

  p3_.x = newX;
  p3_.y = newY;

  dx = p4_.x - p1_.x;
  newX = p1_.x + dx * k;

  dy = p4_.y - p1_.y;
  newY = p1_.y + dy * k;

  p4_.x = newX;
  p4_.y = newY;
}
} // namespace demyanov
