#include <shapes/shape.hpp>
#include <iostream>
#include <stdexcept>

using demyanov::Concave;
using demyanov::point_t;
using demyanov::Rectangle;
using demyanov::rectangle_t;
using demyanov::Rubber;
using demyanov::Shape;

constexpr size_t shape_count = 3;

void scale_shapes(Shape **shapes, const point_t data[], point_t scale_point, double k, size_t count)
{
  for (size_t i = 0; i < count; i++)
  {
    const double dx = data[i].x - scale_point.x;
    const double dy = data[i].y - scale_point.y;
    shapes[i]->move(scale_point);
    shapes[i]->scale(k);
    shapes[i]->move(dx * k, dy * k);
  }
}

rectangle_t get_total_frame(Shape **shapes, size_t count)
{
  const rectangle_t frame1 = shapes[0]->getFrameRect();
  double min_x = frame1.pos.x - frame1.width / 2;
  double max_x = frame1.pos.x + frame1.width / 2;
  double min_y = frame1.pos.y - frame1.height / 2;
  double max_y = frame1.pos.y + frame1.height / 2;

  for (size_t i = 1; i < count; i++)
  {
    const rectangle_t frame2 = shapes[i]->getFrameRect();
    if ((frame2.pos.x - frame2.width / 2) < min_x)
    {
      min_x = frame2.pos.x - frame2.width / 2;
    }
    const double right_x = frame2.pos.x + frame2.width / 2;
    if (right_x > max_x)
    {
      max_x = right_x;
    }
    const double bottom_y = frame2.pos.y - frame2.height / 2;
    if (bottom_y < min_y)
    {
      min_y = bottom_y;
    }
    const double top_y = frame2.pos.y + frame2.height / 2;
    if (top_y > max_y)
    {
      max_y = top_y;
    }
  }

  const double height = max_y - min_y;
  const double width = max_x - min_x;
  const double pos_x = (min_x + max_x) / 2;
  const double pos_y = (min_y + max_y) / 2;
  const point_t pos_xy = {pos_x, pos_y};

  return rectangle_t{width, height, pos_xy};
}

int main()
{
  constexpr double rectangle_width = 5.0;
  constexpr double rectangle_height = 3.0;
  constexpr double big_radius = 5.0;
  constexpr double small_radius = 2.0;

  const point_t pos = {0.0, 0.0};
  Rectangle rectangle(rectangle_width, rectangle_height, pos);

  const point_t big_center = {0.0, 0.0};
  const point_t small_center = {1.0, 0.0};
  Rubber rubber{big_radius, small_radius, big_center, small_center};

  const point_t p1 = {1.0, 1.0};
  const point_t p2 = {0.0, 0.0};
  const point_t p3 = {4.0, 0.0};
  const point_t p4 = {0.0, 4.0};
  Concave concave{p1, p2, p3, p4};

  Shape *shapes[shape_count] = {&rectangle, &rubber, &concave};
  const point_t data[shape_count] = {pos, small_center, p1};

  double total_area = 0.0;

  for (size_t i = 0; i < shape_count; ++i)
  {
    std::cout << shapes[i]->getArea() << "\n";
    total_area += shapes[i]->getArea();
  }

  std::cout << total_area << "\n";

  for (size_t j = 0; j < shape_count; ++j)
  {
    const rectangle_t frame_rectangle = shapes[j]->getFrameRect();
    std::cout << frame_rectangle.width << " " << frame_rectangle.height << " " << frame_rectangle.pos.x << " "
              << frame_rectangle.pos.y << " " << shapes[j]->getArea() << "\n";
  }

  rectangle_t total_frame = get_total_frame(shapes, shape_count);

  std::cout << total_frame.width << " " << total_frame.height << " " << total_frame.pos.x << " " << total_frame.pos.y
            << "\n";

  try
  {
    point_t scale_point;
    std::cin >> scale_point.x >> scale_point.y;
    double k = 0.0;
    std::cin >> k;
    if (!std::cin)
    {
      throw std::invalid_argument("Invalid input");
    }
    if (k <= 0)
    {
      throw std::invalid_argument("Invalid scale coefficient");
    }
    scale_shapes(shapes, data, scale_point, k, shape_count);

    total_area = 0.0;
    for (size_t j = 0; j < shape_count; ++j)
    {
      const rectangle_t frame_rectangle = shapes[j]->getFrameRect();
      std::cout << frame_rectangle.width << " " << frame_rectangle.height << " " << frame_rectangle.pos.x << " "
                << frame_rectangle.pos.y << " " << shapes[j]->getArea() << "\n";
      total_area += shapes[j]->getArea();
    }
    std::cout << total_area << "\n";
  }
  catch (const std::invalid_argument &error)
  {
    std::cerr << error.what() << "\n";
    return 1;
  }

  total_frame = get_total_frame(shapes, shape_count);

  std::cout << total_frame.width << " " << total_frame.height << " " << total_frame.pos.x << " " << total_frame.pos.y
            << "\n";

  return 0;
}
