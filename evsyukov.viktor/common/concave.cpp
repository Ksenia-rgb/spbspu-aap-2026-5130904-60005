#include <shapes/concave.hpp>
#include <stdexcept>
#include <cmath>
namespace evsyukov
{

  namespace
  {
    double cross(const point_t &a, const point_t &b, const point_t &p)
    {
      return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
    }
    bool isStrictlyInside(const point_t &a, const point_t &b, const point_t &c, const point_t &p)
    {
      double d1 = cross(a, b, p);
      double d2 = cross(b, c, p);
      double d3 = cross(c, a, p);
      return (d1 > 0 && d2 > 0 && d3 > 0) || (d1 < 0 && d2 < 0 && d3 < 0);
    }
    double min(double x, double y)
    {
      if (x < y)
      {
        return x;
      }
      else
      {
        return y;
      }
    }
    double max(double x, double y)
    {
      if (x > y)
      {
        return x;
      }
      else
      {
        return y;
      }
    }

  }
  Concave::Concave(const point_t &a, const point_t &b, const point_t &c, const point_t &d):
    v_{a, b, c, d},
    concave_(4)
  {
    for (size_t i = 0; i < 4; ++i)
    {
      if (isStrictlyInside(v_[(i + 1) % 4], v_[(i + 2) % 4], v_[(i + 3) % 4], v_[i]))
      {
        concave_ = i;
        break;
      }
    }
    if (concave_ == 4)
    {
      throw std::invalid_argument("Concave: quadrilateral must be non-convex");
    }
  }
  double Concave::getArea() const noexcept
  {
    double sum = 0;
    for (size_t i = 0; i < 4; i++)
    {
      size_t j = (i + 1) % 4;
      sum += v_[i].x * v_[j].y - v_[j].x * v_[i].y;
    }
    return std::abs(sum) / 2.0;
  }
  rectangle_t Concave::getFrameRect() const noexcept
  {
    double min_x = min(min(v_[0].x, v_[1].x), min(v_[2].x, v_[3].x));
    double max_x = max(max(v_[0].x, v_[1].x), max(v_[2].x, v_[3].x));
    double min_y = min(min(v_[0].y, v_[1].y), min(v_[2].y, v_[3].y));
    double max_y = max(max(v_[0].y, v_[1].y), max(v_[2].y, v_[3].y));
    return {max_x - min_x, max_y - min_y, {(min_x + max_x) / 2.0, (min_y + max_y) / 2.0}};
  }
  void Concave::move(double dx, double dy) noexcept
  {
    for (size_t i = 0; i < 4; i++)
    {
      v_[i].x += dx;
      v_[i].y += dy;
    }
  }
  void Concave::move(const point_t &p) noexcept
  {
    double dx = p.x - v_[concave_].x;
    double dy = p.y - v_[concave_].y;
    move(dx, dy);
  }
  void Concave::scale(double k)
  {
    if (k <= 0)
    {
      throw std::invalid_argument("Concave: scale factor must be posotive");
    }
    point_t c = v_[concave_];
    for (size_t i = 0; i < 4; i++)
    {
      v_[i].x = c.x + k * (v_[i].x - c.x);
      v_[i].y = c.y + k * (v_[i].y - c.y);
    }
  }
}
