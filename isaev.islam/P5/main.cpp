#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "../common/shapes/base-types.hpp"
#include "../common/shapes/shape.hpp"
#include "../common/shapes/rectangle.hpp"
#include "../common/shapes/square.hpp"
#include "../common/shapes/polygon.hpp"

namespace
{
  using isaev::point_t;
  using isaev::rectangle_t;
  using isaev::Shape;

  void freeShapes(Shape ** shapes, std::size_t count)
  {
    for (std::size_t i = 0; i < count; ++i)
    {
      delete shapes[i];
      shapes[i] = nullptr;
    }
  }

  double getTotalArea(const Shape * const * shapes, std::size_t count)
  {
    double total = 0.0;
    for (std::size_t i = 0; i < count; ++i)
    {
      total += shapes[i]->getArea();
    }
    return total;
  }

  rectangle_t getTotalFrameRect(const Shape * const * shapes, std::size_t count)
  {
    rectangle_t first = shapes[0]->getFrameRect();
    double minX = first.pos.x - first.width / 2.0;
    double maxX = first.pos.x + first.width / 2.0;
    double minY = first.pos.y - first.height / 2.0;
    double maxY = first.pos.y + first.height / 2.0;
    for (std::size_t i = 1; i < count; ++i)
    {
      rectangle_t r = shapes[i]->getFrameRect();
      minX = std::min(minX, r.pos.x - r.width / 2.0);
      maxX = std::max(maxX, r.pos.x + r.width / 2.0);
      minY = std::min(minY, r.pos.y - r.height / 2.0);
      maxY = std::max(maxY, r.pos.y + r.height / 2.0);
    }
    return {maxX - minX, maxY - minY, {(minX + maxX) / 2.0, (minY + maxY) / 2.0}};
  }

  void printFrameRect(std::ostream & out, const rectangle_t & r)
  {
    out << "width = " << r.width << ", height = " << r.height;
    out << ", center = (" << r.pos.x << ", " << r.pos.y << ")";
  }

  void printShapes(std::ostream & out, const Shape * const * shapes, std::size_t count, const char * title)
  {
    out << "=== " << title << " ===\n";
    for (std::size_t i = 0; i < count; ++i)
    {
      out << "Shape " << i + 1 << ":\n";
      out << "  area = " << shapes[i]->getArea() << "\n";
      out << "  frame: ";
      printFrameRect(out, shapes[i]->getFrameRect());
      out << "\n";
    }
    out << "Total area = " << getTotalArea(shapes, count) << "\n";
    out << "Total frame: ";
    printFrameRect(out, getTotalFrameRect(shapes, count));
    out << "\n";
  }

  void scaleAboutPoint(Shape ** shapes, std::size_t count, const point_t & p, double k)
  {
    if (k <= 0.0)
    {
      throw std::invalid_argument("scale factor must be positive");
    }
    for (std::size_t i = 0; i < count; ++i)
    {
      const point_t before = shapes[i]->getFrameRect().pos;
      shapes[i]->move(p);
      const point_t after = shapes[i]->getFrameRect().pos;
      shapes[i]->scale(k);
      shapes[i]->move((before.x - after.x) * k, (before.y - after.y) * k);
    }
  }
}

int main()
{
  using namespace isaev;

  constexpr std::size_t count = 5;
  Shape * shapes[count] = {};

  try
  {
    const point_t polygonVertices[] = {{1.0, 1.0}, {5.0, 1.0}, {6.0, 4.0}, {3.0, 6.0}, {0.0, 4.0}};
    shapes[0] = new Rectangle(point_t{0.0, 0.0}, 4.0, 2.0);
    shapes[1] = new Rectangle(point_t{-3.0, -1.0}, point_t{1.0, 2.5});
    shapes[2] = new Square(point_t{5.0, 5.0}, 3.0);
    shapes[3] = new Polygon(polygonVertices, sizeof(polygonVertices) / sizeof(polygonVertices[0]));
    shapes[4] = new Polygon(polygonVertices, 3);
  }
  catch (const std::exception & e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    freeShapes(shapes, count);
    return 1;
  }

  std::cout << std::fixed << std::setprecision(3);

  std::cout << "Enter scaling point (x y) and scale factor k: ";
  point_t p{0.0, 0.0};
  double k = 0.0;
  if (!(std::cin >> p.x >> p.y >> k))
  {
    std::cerr << "Error: invalid input\n";
    freeShapes(shapes, count);
    return 1;
  }
  if (k <= 0.0)
  {
    std::cerr << "Error: scale factor must be positive\n";
    freeShapes(shapes, count);
    return 1;
  }

  printShapes(std::cout, shapes, count, "Before scaling");

  try
  {
    scaleAboutPoint(shapes, count, p, k);
  }
  catch (const std::exception & e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    freeShapes(shapes, count);
    return 1;
  }

  printShapes(std::cout, shapes, count, "After scaling");

  freeShapes(shapes, count);
  return 0;
}
