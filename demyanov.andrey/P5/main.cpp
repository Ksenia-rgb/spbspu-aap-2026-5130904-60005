#include "shape.hpp"
#include <iostream>
#include <stdexcept>

using demyanov::Concave;
using demyanov::point_t;
using demyanov::Rectangle;
using demyanov::rectangle_t;
using demyanov::Rubber;
using demyanov::Shape;

void scaleShapes(Shape **shapes, point_t data[], point_t scalePoint, double k,
                 size_t count) {
  for (size_t i = 0; i < count; i++) {
    double dx = data[i].x - scalePoint.x;
    double dy = data[i].y - scalePoint.y;
    shapes[i]->move(scalePoint);
    shapes[i]->scale(k);
    shapes[i]->move(dx * k, dy * k);
  }
}

rectangle_t getTotalFrame(Shape **shapes, size_t count) {
  rectangle_t frame1 = shapes[0]->getFrameRect();
  double minX = frame1.pos.x - frame1.width / 2;
  double maxX = frame1.pos.x + frame1.width / 2;
  double minY = frame1.pos.y - frame1.height / 2;
  double maxY = frame1.pos.y + frame1.height / 2;

  for (size_t i = 1; i < count; i++) {
    rectangle_t frame2 = shapes[i]->getFrameRect();
    if ((frame2.pos.x - frame2.width / 2) < minX) {
      minX = frame2.pos.x - frame2.width / 2;
    }
    double rightX = frame2.pos.x + frame2.width / 2;
    if (rightX > maxX) {
      maxX = rightX;
    }
    double bottomY = frame2.pos.y - frame2.height / 2;
    if (bottomY < minY) {
      minY = bottomY;
    }
    double topY = frame2.pos.y + frame2.height / 2;
    if (topY > maxY) {
      maxY = topY;
    }
  }

  double height = maxY - minY;
  double width = maxX - minX;
  double pos_x = (minX + maxX) / 2;
  double pos_y = (minY + maxY) / 2;
  point_t pos_xy = {pos_x, pos_y};

  return rectangle_t{width, height, pos_xy};
}

int main() {
  point_t pos = {0.0, 0.0};
  Rectangle rectangle(5.0, 3.0, pos);

  double bigRadius = 5.0;
  double smallRadius = 2.0;
  point_t bigCenter = {0.0, 0.0};
  point_t smallCenter = {1.0, 0.0};
  Rubber rubber{bigRadius, smallRadius, bigCenter, smallCenter};

  point_t p1 = {1.0, 1.0};
  point_t p2 = {0.0, 0.0};
  point_t p3 = {4.0, 0.0};
  point_t p4 = {0.0, 4.0};
  Concave concave{p1, p2, p3, p4};

  Shape *shapes[3] = {&rectangle, &rubber, &concave};
  point_t data[3] = {pos, smallCenter, p1};

  double totalArea = 0.0;

  for (size_t i = 0; i < 3; ++i) {
    std::cout << shapes[i]->getArea() << "\n";
    totalArea += shapes[i]->getArea();
  }

  std::cout << totalArea << "\n";

  for (size_t j = 0; j < 3; ++j) {
    rectangle_t frameRectangle = shapes[j]->getFrameRect();
    std::cout << frameRectangle.width << " " << frameRectangle.height << " "
              << frameRectangle.pos.x << " " << frameRectangle.pos.y << " "
              << shapes[j]->getArea() << "\n";
  }

  rectangle_t totalFrame = getTotalFrame(shapes, 3);

  std::cout << totalFrame.width << " " << totalFrame.height << " "
            << totalFrame.pos.x << " " << totalFrame.pos.y << "\n";

  try {
    point_t scalePoint;
    std::cin >> scalePoint.x >> scalePoint.y;
    double k = 0.0;
    std::cin >> k;
    if (!std::cin) {
      throw std::invalid_argument("Invalid input");
    }
    if (k <= 0) {
      throw std::invalid_argument("Invalid scale coefficient");
    }
    scaleShapes(shapes, data, scalePoint, k, 3);

    totalArea = 0.0;
    for (size_t j = 0; j < 3; ++j) {
      rectangle_t frameRectangle = shapes[j]->getFrameRect();
      std::cout << frameRectangle.width << " " << frameRectangle.height << " "
                << frameRectangle.pos.x << " " << frameRectangle.pos.y << " "
                << shapes[j]->getArea() << "\n";
      totalArea += shapes[j]->getArea();
    }
    std::cout << totalArea << "\n";

  } catch (const std::invalid_argument &error) {
    std::cerr << error.what() << "\n";
    return 1;
  }

  totalFrame = getTotalFrame(shapes, 3);

  std::cout << totalFrame.width << " " << totalFrame.height << " "
            << totalFrame.pos.x << " " << totalFrame.pos.y << "\n";

  return 0;
}
