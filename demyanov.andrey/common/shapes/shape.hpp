#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <base-types.hpp>

namespace demyanov {
class Shape {
public:
  virtual double getArea() const = 0;
  virtual rectangle_t getFrameRect() const = 0;
  virtual void move(double dx, double dy) = 0;
  virtual void move(const point_t &) = 0;
  virtual void scale(double) = 0;
  virtual ~Shape() = default;
};

class Rectangle : public Shape {
public:
  Rectangle(double width, double height, point_t pos);
  double getArea() const override;
  rectangle_t getFrameRect() const override;
  void move(double dx, double dy) override;
  void move(const point_t &) override;
  void scale(double) override;

private:
  rectangle_t rect_;
};

class Rubber : public Shape {
public:
  Rubber(double big_radius, double small_radius, point_t big_center,
         point_t small_center);
  double getArea() const override;
  rectangle_t getFrameRect() const override;
  void move(double dx, double dy) override;
  void move(const point_t &) override;
  void scale(double) override;

private:
  double big_radius_;
  double small_radius_;
  point_t big_center_;
  point_t small_center_;
};

class Concave : public Shape {
public:
  Concave(point_t p1, point_t p2, point_t p3, point_t p4);
  double getArea() const override;
  rectangle_t getFrameRect() const override;
  void move(double dx, double dy) override;
  void move(const point_t &) override;
  void scale(double) override;

private:
  point_t p1_;
  point_t p2_;
  point_t p3_;
  point_t p4_;
};
}

#endif
