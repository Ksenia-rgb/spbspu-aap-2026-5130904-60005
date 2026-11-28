#include "../common/shapes/rectangle.hpp"
#include "../common/shapes/circle.hpp"
#include "../common/shapes/concave.hpp"
#include <cstddef>
#include <iostream>

namespace evsyukov
{
  namespace
  {
    const double k_half = 2.0;
    void printFrame(const rectangle_t &r)
    {
      std::cout << "Width: " << r.width << ", height: " << r.height << ", center: (" << r.pos.x << ", " << r.pos.y
                << ")\n";
    }
    void printInfo(const Shape *const *shapes, const char *const *names, size_t n)
    {
      double total = 0.0;
      const rectangle_t first = shapes[0]->getFrameRect();
      double min_x = first.pos.x - first.width / k_half;
      double max_x = first.pos.x + first.width / k_half;
      double min_y = first.pos.y - first.height / k_half;
      double max_y = first.pos.y + first.height / k_half;
      for (size_t i = 0; i < n; ++i)
      {
        const double area = shapes[i]->getArea();
        const rectangle_t fr = shapes[i]->getFrameRect();
        std::cout << names[i] << ": area = " << area << "\n frame: ";
        printFrame(fr);
        total += area;
        const double left = fr.pos.x - fr.width / 2.0;
        const double right = fr.pos.x + fr.width / 2.0;
        const double bottom = fr.pos.y - fr.height / 2.0;
        const double top = fr.pos.y + fr.height / 2.0;
        if (left < min_x)
        {
          min_x = left;
        }
        if (right > max_x)
        {
          max_x = right;
        }
        if (bottom < min_y)
        {
          min_y = bottom;
        }
        if (top > max_y)
        {
          max_y = top;
        }
      }
      std::cout << "Total area: " << total << "\n";
      const rectangle_t f_r_result
          = {max_x - min_x, max_y - min_y, {(min_x + max_x) / k_half, (min_y + max_y) / k_half}};
      std::cout << "Total frame: ";
      printFrame(f_r_result);
    }
    void scaleAll(Shape **shapes, size_t n, const point_t &p, double k)
    {
      for (size_t i = 0; i < n; ++i)
      {
        const point_t befor = shapes[i]->getFrameRect().pos;
        shapes[i]->move(p);
        const point_t after = shapes[i]->getFrameRect().pos;
        shapes[i]->scale(k);
        shapes[i]->move((befor.x - after.x) * k, (befor.y - after.y) * k);
      }
    }
  }
}

int main()
{
  using namespace evsyukov;
  constexpr double k_rect_width = 4.0;
  constexpr double k_rect_height = 2.0;
  constexpr double k_circle_x = 3.0;
  constexpr double k_circle_y = 3.0;
  constexpr double k_circle_radius = 1.5;
  constexpr double k_concave_far = 4.0;
  constexpr size_t k_shape_count = 3;
  try
  {
    Rectangle rect({0.0, 0.0}, k_rect_width, k_rect_height);
    Circle circ({k_circle_x, k_circle_y}, k_circle_radius);
    Concave conc({0.0, 0.0}, {k_concave_far, 0.0}, {1.0, 1.0}, {0.0, k_concave_far});
    Shape *shapes[] = {&rect, &circ, &conc};
    const char *names[] = {"Rectangle", "Circle", "Concave"};

    std::cout << "Before scaling:\n";
    printInfo(shapes, names, k_shape_count);

    point_t p = {0.0, 0.0};
    double k = 0.0;
    std::cin >> p.x >> p.y >> k;
    if (!std::cin || k <= 0.0)
    {
      std::cerr << "Error: invalid scaling parameters\n";
      return 1;
    }
    scaleAll(shapes, k_shape_count, p, k);
    std::cout << "After scaling:\n";
    printInfo(shapes, names, k_shape_count);
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  return 0;
}
