#include "shapes/polygon.hpp"
#include "shapes/base-types.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace
{
  using isaev::point_t;

  constexpr double epsilon = 1e-12;

  double getSignedArea(const std::vector< point_t > & v)
  {
    const point_t & o = v[0];
    double sum = 0.0;
    const std::size_t n = v.size();
    for (std::size_t i = 0; i < n; ++i)
    {
      const point_t & a = v[i];
      const point_t & b = v[(i + 1) % n];
      sum += (a.x - o.x) * (b.y - o.y) - (b.x - o.x) * (a.y - o.y);
    }
    return sum / 2.0;
  }

  point_t getCenter(const std::vector< point_t > & v)
  {
    const point_t & o = v[0];
    const double area = getSignedArea(v);
    double cx = 0.0;
    double cy = 0.0;
    const std::size_t n = v.size();
    for (std::size_t i = 0; i < n; ++i)
    {
      const point_t & a = v[i];
      const point_t & b = v[(i + 1) % n];
      const double ax = a.x - o.x;
      const double ay = a.y - o.y;
      const double bx = b.x - o.x;
      const double by = b.y - o.y;
      const double cross = ax * by - bx * ay;
      cx += (ax + bx) * cross;
      cy += (ay + by) * cross;
    }
    return {o.x + cx / (6.0 * area), o.y + cy / (6.0 * area)};
  }

  void validate(const std::vector< point_t > & v)
  {
    for (const point_t & p: v)
    {
      if (!std::isfinite(p.x) || !std::isfinite(p.y))
      {
        throw std::invalid_argument("Polygon: invalid or overflowing coordinates");
      }
    }
    const double area = getSignedArea(v);
    if (!std::isfinite(area) || area == 0.0)
    {
      throw std::invalid_argument("Polygon: invalid or overflowing area");
    }
    const point_t c = getCenter(v);
    if (!std::isfinite(c.x) || !std::isfinite(c.y))
    {
      throw std::invalid_argument("Polygon: invalid or overflowing center");
    }
  }
}

isaev::Polygon::Polygon(const point_t * vertices, std::size_t count)
{
  if (vertices == nullptr || count < 3)
  {
    throw std::invalid_argument("Polygon: at least 3 vertices are required");
  }
  std::vector< point_t > tmp(vertices, vertices + count);
  validate(tmp);
  if (std::abs(getSignedArea(tmp)) < epsilon)
  {
    throw std::invalid_argument("Polygon: area must be non-zero");
  }
  vertices_.swap(tmp);
}

double isaev::Polygon::getArea() const
{
  return std::abs(getSignedArea(vertices_));
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
  const point_t c = getCenter(vertices_);
  move(p.x - c.x, p.y - c.y);
}

void isaev::Polygon::move(double dx, double dy)
{
  std::vector< point_t > tmp(vertices_);
  for (point_t & v: tmp)
  {
    v.x += dx;
    v.y += dy;
  }
  validate(tmp);
  vertices_.swap(tmp);
}

void isaev::Polygon::scale(double k)
{
  if (!(k > 0.0) || !std::isfinite(k))
  {
    throw std::invalid_argument("Polygon: scale factor must be positive and finite");
  }
  const point_t c = getCenter(vertices_);
  std::vector< point_t > tmp(vertices_);
  for (point_t & v: tmp)
  {
    v.x = c.x + (v.x - c.x) * k;
    v.y = c.y + (v.y - c.y) * k;
  }
  validate(tmp);
  vertices_.swap(tmp);
}