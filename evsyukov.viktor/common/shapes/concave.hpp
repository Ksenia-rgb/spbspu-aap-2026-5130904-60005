#ifndef CONCAVE_HPP
#define CONCAVE_HPP

#include <cstddef>
#include "shape.hpp"

namespace evsyukov
{

  class Concave: public Shape
  {
  public:
    Concave(const point_t &a, const point_t &b, const point_t &c, const point_t &d);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &p) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double k) override;

  private:
    static constexpr size_t k_vertex_count = 4;
    point_t v_[k_vertex_count];
    size_t concave_;
  };

}

#endif
