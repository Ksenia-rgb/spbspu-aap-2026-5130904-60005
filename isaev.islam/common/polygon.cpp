#include "shapes/polygon.hpp"
#include "shapes/base-types.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace
{
  constexpr double epsilon = 1e-12;
}

isaev::Polygon::Polygon(const point_t * vertices, std::size_t count)
{
  if (vertices == nullptr || count < 3)
  {
    throw std::invalid_argument("Polygon: at least 3 vertices are required");
  }
  vertices_.assign(vertices, vertices + count);
  if (std::abs(getSignedArea()) < epsilon)
  {
    throw std::invalid_argument("Polygon: area must be non-zero");
  }
}

double isaev::Polygon::getSignedArea() const
{
  double sum = 0.0;
  const std::size_t n = vertices_.size();
  for (std::size_t i = 0; i < n; ++i)
  {
    const point_t & a = vertices_[i];
    const point_t & b = vertices_[(i + 1) % n];
    sum += a.x * b.y - b.x * a.y;
  }
  return sum / 2.0;
}

isaev::point_t isaev::Polygon::getCenter() const
{
  const double area = getSignedArea();
  double cx = 0.0;
  double cy = 0.0;
  const std::size_t n = vertices_.size();
  for (std::size_t i = 0; i < n; ++i)
  {
    const point_t & a = vertices_[i];
    const point_t & b = vertices_[(i + 1) % n];
    const double cross = a.x * b.y - b.x * a.y;
    cx += (a.x + b.x) * cross;
    cy += (a.y + b.y) * cross;
  }
  return {cx / (6.0 * area), cy / (6.0 * area)};
}

double isaev::Polygon::getArea() const
{
  return std::abs(getSignedArea());
}

isaev::rectangle_t isaev::Polygon::getFrameRect() const
{
  double minX = vertices_[0].x;
  double maxX = vertices_[0].x;
  double minY = vertices_[0].y;
  double maxY = vertices_[0].y;
  for (const point_t & v: vertices_)
  {
    minX = std::min(minX, v.x);
    maxX = std::max(maxX, v.x);
    minY = std::min(minY, v.y);
    maxY = std::max(maxY, v.y);
  }
  return {maxX - minX, maxY - minY, {(minX + maxX) / 2.0, (minY + maxY) / 2.0}};
}

void isaev::Polygon::move(const point_t & p)
{
  const point_t c = getCenter();
  move(p.x - c.x, p.y - c.y);
}

void isaev::Polygon::move(double dx, double dy)
{
  for (point_t & v: vertices_)
  {
    v.x += dx;
    v.y += dy;
  }
}

void isaev::Polygon::scale(double k)
{
  if (k <= 0.0)
  {
    throw std::invalid_argument("Polygon: scale factor must be positive");
  }
  const point_t c = getCenter();
  for (point_t & v: vertices_)
  {
    v.x = c.x + (v.x - c.x) * k;
    v.y = c.y + (v.y - c.y) * k;
  }
}