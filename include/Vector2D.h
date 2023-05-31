#include <cmath>

struct Vector2D {
  double x, y;

  Vector2D(double x = 0.0, double y = 0.0) : x(x), y(y) {}

  Vector2D operator+ (const Vector2D& other) const {
    return { x + other.x, y + other.y };
  }

  Vector2D operator- (const Vector2D& other) const {
    return { x - other.x, y - other.y };
  }

  Vector2D operator* (double scalar) const {
    return { x * scalar , y * scalar };
  }

  Vector2D& operator += (const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  Vector2D& operator -= (const Vector2D& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Vector2D& operator *= (double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  double magnitude() const {
    return std::sqrt(x * x + y * y);
  }

  Vector2D normalized() const {
    double mag = magnitude();
    return Vector2D(x / mag, y / mag);
  }
};
