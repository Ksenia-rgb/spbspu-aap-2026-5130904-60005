#include "shapes/base-types.hpp"
#include <shapes/rectangle.hpp>
#include <shapes/circle.hpp>
#include <shapes/concave.hpp>
#include <cstddef>
#include <iostream>
#include <stdexcept>

namespace evsyukov
{
  namespace
  {
    void printFrame(const rectangle_t &r)
    {
      std::cout << "Width: " << r.width << ", height: " << r.height << ", center: (" << r.pos.x << ", " << r.pos.y
                << ")\n";
    }
    void printInfo(const Shape *const *shapes, const char *const *names, size_t n)
    {
      double total = 0.0;
      rectangle_t first = shapes[0]->getFrameRect();
      double min_x = first.pos.x - first.width / 2.0;
      double max_x = first.pos.x + first.width / 2.0;
      double min_y = first.pos.y - first.height / 2.0;
      double max_y = first.pos.y + first.height / 2.0;
      for (size_t i = 0; i < n; ++i)
      {
        double area = shapes[i]->getArea();
        rectangle_t fr = shapes[i]->getFrameRect();
        std::cout << names[i] << ": area = " << area << "\n frame: ";
        printFrame(fr);
        total += area;
        double left = fr.pos.x - fr.width / 2.0;
        double right = fr.pos.x + fr.width / 2.0;
        double bottom = fr.pos.y - fr.height / 2.0;
        double top = fr.pos.y + fr.height / 2.0;
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
      rectangle_t f_r_result = {max_x - min_x, max_y - min_y, {(min_x + max_x) / 2.0, (min_y + max_y) / 2.0}};
      std::cout << "Total frame: ";
      printFrame(f_r_result);
    }
    void scaleAll(Shape **shapes, size_t n, const point_t &p, double k)
    {
      for (size_t i = 0; i < n; ++i)
      {
        point_t befor = shapes[i]->getFrameRect().pos;
        shapes[i]->move(p);
        point_t after = shapes[i]->getFrameRect().pos;
        shapes[i]->scale(k);
        shapes[i]->move((befor.x - after.x) * k, (befor.y - after.y) * k);
      }
    }
  }
}

int main()
{
  using namespace evsyukov;
  try
  {
    Rectangle rect({0.0, 0.0}, 4.0, 2.0);
    Circle circ({3.0, 3.0}, 1.5);
    Concave conc({0.0, 0.0}, {4.0, 0.0}, {1.0, 1.0}, {0.0, 4.0});
    Shape *shapes[] = {&rect, &circ, &conc};
    const char *names[] = {"Rectangle", "Circle", "Concave"};
    const size_t count = 3;
    std::cout << "Before scaling:\n";
    printInfo(shapes, names, count);
    point_t p = {0.0, 0.0};
    double k = 0.0;
    std::cin >> p.x >> p.y >> k;
    if (!std::cin || k <= 0)
    {
      std::cerr << "Error: invalid scaling parameters\n";
      return 1;
    }
    scaleAll(shapes, count, p, k);
    std::cout << "After scaling:\n";
    printInfo(shapes, names, count);
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  return 0;
}

