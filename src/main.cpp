#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <memory>

#include "Vector2D.h"

struct Body {
  Vector2D position, velocity;
  double mass;

  Body(const Vector2D& position = Vector2D(),
       const Vector2D& velocity = Vector2D(),
       double mass = 0.0) : position(position), velocity(velocity), mass(mass) {}
};

void updateVelocity(Body& body, const std::vector<Body>& bodies, double gravitationalConstant, double deltaTime) {
  Vector2D acceleration;

  for (const Body& other : bodies) {
    while (&body == &other) continue;
    
    Vector2D direction = other.position - body.position;

    double distance = direction.magnitude();
    double forceMagnitude = (gravitationalConstant * body.mass * other.mass) / (distance * distance);

    Vector2D force = direction.normalized() * forceMagnitude;
    acceleration += force * (1 / body.mass);
  }

  body.velocity += acceleration * deltaTime;
}

void updatePosition(Body& body, double deltaTime) {
  body.position += body.velocity * deltaTime;
}

int main() {
  constexpr double gravitationalConstant = 6.674e-11;
  const double deltaTime = 0.1;

  std::vector<Body> bodies = {
    Body(Vector2D(0.0, 0.0), Vector2D(0.0, 0.0), 1),
    Body(Vector2D(100.0, 0.0), Vector2D(0.0, 0.1), 2),
  };

  sf::RenderWindow window(sf::VideoMode(800, 600), "N-Body Simulator");
  sf::CircleShape circle(10.0f);
  circle.setFillColor(sf::Color::Red);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    for (Body& body : bodies) {
      updateVelocity(body, bodies, gravitationalConstant, deltaTime);
      updatePosition(body, deltaTime);
    }

    window.clear();

    for (const Body& body : bodies) {
      circle.setPosition(static_cast<float>(body.position.x), static_cast<float>(body.position.y));
      window.draw(circle);
    }

    window.display();
  }
  
  return 0;
}
