#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <stdexcept>

#include "../common/shapes/base-types.hpp"
#include "../common/shapes/shape.hpp"
#include "../common/shapes/rectangle.hpp"
#include "../common/shapes/square.hpp"
#include "../common/shapes/polygon.hpp"

namespace isaev
{
  const int bad_ec = 1;
  const int good_ec = 0;

  const std::size_t shapes_count_v = 5;
  const double max_value_v = 1e6;
  const int precision_v = 3;

  const point_t rect_center_p{0.0, 0.0};
  const double rect_width_v = 4.0;
  const double rect_height_v = 2.0;
  const point_t rect_left_bottom_p{-3.0, -1.0};
  const point_t rect_right_top_p{1.0, 2.5};
  const point_t square_center_p{5.0, 5.0};
  const double square_side_v = 3.0;
  const point_t polygon_vertices_a[] = {{1.0, 1.0}, {5.0, 1.0}, {6.0, 4.0}, {3.0, 6.0}, {0.0, 4.0}};
  const std::size_t polygon_vertices_count_v = 5;
  const std::size_t triangle_vertices_count_v = 3;

  std::ostream& operator<<(std::ostream& out, const rectangle_t& rect);

  double readDouble(std::istream& input);
  point_t readPoint(std::istream& input);
  void checkScaleParams(const point_t& anchor, double scale_coef);
  void scaleAbout(Shape& s, const point_t& anchor, double scale_coef);
  void scaleShapes(Shape* const* shapes, std::size_t k, const point_t& anchor, double scale_coef);
  double calcSumArea(const Shape* const* shapes, std::size_t k);
  rectangle_t formTotalFrameRect(const Shape* const* shapes, std::size_t k);
  void displayShapesSpecs(std::ostream& out, const char* title, const Shape* const* shapes, std::size_t k);
  int process(std::istream& input, std::ostream& out, std::ostream& err_out);
}

int main()
{
  return isaev::process(std::cin, std::cout, std::cerr);
}

std::ostream& isaev::operator<<(std::ostream& out, const rectangle_t& rect)
{
  out << "width = " << rect.width << ", height = " << rect.height;
  return out << ", center = (" << rect.pos.x << ", " << rect.pos.y << ")";
}

double isaev::readDouble(std::istream& input)
{
  double res = 0.0;
  input >> res;
  if (!input)
  {
    throw std::runtime_error("Invalid input");
  }
  return res;
}

isaev::point_t isaev::readPoint(std::istream& input)
{
  return point_t{readDouble(input), readDouble(input)};
}

void isaev::checkScaleParams(const point_t& anchor, double scale_coef)
{
  if (!(scale_coef > 0.0))
  {
    throw std::invalid_argument("Scale factor must be positive");
  }
  if (std::abs(anchor.x) > max_value_v || std::abs(anchor.y) > max_value_v || scale_coef > max_value_v)
  {
    throw std::invalid_argument("Values must satisfy |x|, |y| <= 1e6 and scale factor <= 1e6");
  }
}

void isaev::scaleAbout(Shape& s, const point_t& anchor, double scale_coef)
{
  const point_t before = s.getFrameRect().pos;
  s.move(anchor);
  const point_t after = s.getFrameRect().pos;
  s.scale(scale_coef);
  s.move((before.x - after.x) * scale_coef, (before.y - after.y) * scale_coef);
}

void isaev::scaleShapes(Shape* const* shapes, std::size_t k, const point_t& anchor, double scale_coef)
{
  for (std::size_t i = 0; i < k; i++)
  {
    scaleAbout(*shapes[i], anchor, scale_coef);
  }
}

double isaev::calcSumArea(const Shape* const* shapes, std::size_t k)
{
  double res = 0.0;
  for (std::size_t i = 0; i < k; i++)
  {
    res += shapes[i]->getArea();
  }
  return res;
}

isaev::rectangle_t isaev::formTotalFrameRect(const Shape* const* shapes, std::size_t k)
{
  if (shapes == nullptr || k == 0)
  {
    throw std::invalid_argument("Empty shapes array");
  }
  double min_x = 0.0;
  double min_y = 0.0;
  double max_x = 0.0;
  double max_y = 0.0;
  for (std::size_t i = 0; i < k; i++)
  {
    const rectangle_t frame = shapes[i]->getFrameRect();
    const double left = frame.pos.x - frame.width / 2.0;
    const double right = frame.pos.x + frame.width / 2.0;
    const double bottom = frame.pos.y - frame.height / 2.0;
    const double top = frame.pos.y + frame.height / 2.0;
    min_x = (i == 0) ? left : std::min(min_x, left);
    max_x = (i == 0) ? right : std::max(max_x, right);
    min_y = (i == 0) ? bottom : std::min(min_y, bottom);
    max_y = (i == 0) ? top : std::max(max_y, top);
  }
  const double w = max_x - min_x;
  const double h = max_y - min_y;
  return rectangle_t{w, h, {min_x + w / 2.0, min_y + h / 2.0}};
}

void isaev::displayShapesSpecs(std::ostream& out, const char* title, const Shape* const* shapes, std::size_t k)
{
  if (shapes == nullptr || k == 0)
  {
    throw std::invalid_argument("Empty shapes array");
  }
  out << "=== " << title << " ===\n";
  for (std::size_t i = 0; i < k; i++)
  {
    out << "Shape " << i + 1 << ":\n";
    out << " area = " << shapes[i]->getArea() << "\n";
    out << " frame: " << shapes[i]->getFrameRect() << "\n";
  }
  out << "Total area = " << calcSumArea(shapes, k) << "\n";
  out << "Total frame: " << formTotalFrameRect(shapes, k) << "\n";
}

int isaev::process(std::istream& input, std::ostream& out, std::ostream& err_out)
{
  try
  {
    Rectangle rect_a(rect_center_p, rect_width_v, rect_height_v);
    Rectangle rect_b(rect_left_bottom_p, rect_right_top_p);
    Square square_s(square_center_p, square_side_v);
    Polygon polygon_p(polygon_vertices_a, polygon_vertices_count_v);
    Polygon triangle_p(polygon_vertices_a, triangle_vertices_count_v);
    Shape* shapes_arr[shapes_count_v]{&rect_a, &rect_b, &square_s, &polygon_p, &triangle_p};
    out << std::fixed << std::setprecision(precision_v);
    out << "Input anchor point x and y" << std::endl;
    const point_t anchor_p = readPoint(input);
    out << "Input scale multiplier" << std::endl;
    const double scale_coef = readDouble(input);
    checkScaleParams(anchor_p, scale_coef);
    displayShapesSpecs(out, "Before scaling", shapes_arr, shapes_count_v);
    scaleShapes(shapes_arr, shapes_count_v, anchor_p, scale_coef);
    displayShapesSpecs(out, "After scaling", shapes_arr, shapes_count_v);
  }
  catch (const std::invalid_argument& e)
  {
    err_out << "Invalid argument: " << e.what() << std::endl;
    return bad_ec;
  }
  catch (const std::runtime_error& e)
  {
    err_out << "Runtime error: " << e.what() << std::endl;
    return bad_ec;
  }
  catch (const std::exception& e)
  {
    err_out << "Error: " << e.what() << std::endl;
    return bad_ec;
  }
  return good_ec;
}
