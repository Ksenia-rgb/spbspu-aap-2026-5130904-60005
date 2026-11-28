#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <cstddef>
#include <vector>

#include "shape.hpp"
#include "base-types.hpp"

namespace isaev
{
  class Polygon final: public Shape
  {
  public:
    Polygon(const point_t * vertices, std::size_t count);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(const point_t & p) override;
    void move(double dx, double dy) override;
    void scale(double k) override;
  private:
    std::vector< point_t > vertices_;
    double getSignedArea() const;
    point_t getCenter() const;
  };
}

#endif