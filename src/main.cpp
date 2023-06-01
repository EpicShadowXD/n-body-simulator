#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>

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
    if (&body != &other) {

      Vector2D direction = other.position - body.position;
      double distance = direction.magnitude();

      if (distance > 0.0) {
        double forceMagnitude = (gravitationalConstant * body.mass * other.mass) / (distance * distance);
        Vector2D force = direction.normalized() * forceMagnitude + 0.0;
        acceleration += force * (1.0 / body.mass);
      }
    }
  }
  body.velocity += acceleration * deltaTime;
}

void updatePosition(Body& body, double deltaTime) {
  body.position += body.velocity * deltaTime;
}

void spawnBodies(std::vector<Body>& bodies, std::size_t n, std::size_t screenWidth, std::size_t screenHeight, double minMass, double maxMass) {
  std::srand(std::time(nullptr));

  for (std::size_t i = 0; i < n; ++i) {
    double x = std::rand() % screenWidth;
    double y = std::rand() % screenHeight;
    double mass = minMass + (static_cast<double>(std::rand()) / RAND_MAX) * (maxMass - minMass);

    bodies.emplace_back(Vector2D(x, y), Vector2D(), mass);
  }
}

int main() {
  const std::size_t WIDTH = 800,
                    HEIGHT = 600;

  constexpr double gravitationalConstant = 6.674e-3;
  const double deltaTime = 0.05;

  const double positionScale = 0.5;
  const double velocityScale = 1.0;

  /*
  std::vector<Body> bodies = {
    Body(Vector2D(0.0 / positionScale, 0.0 / positionScale), Vector2D(0.1 / velocityScale, 0.1 / velocityScale), 100),
    Body(Vector2D(100.0 / positionScale, 0.0 / positionScale), Vector2D(-0.05 / velocityScale, 0.05 / velocityScale), 200),
  };
  */ 
  std::vector<Body> bodies;
  spawnBodies(bodies, 10, WIDTH, HEIGHT, 6, 10);

  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "N-Body Simulator");
  sf::CircleShape circle(10.0f);
  circle.setFillColor(sf::Color::Red);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    for (Body& body : bodies) {
      // TODO: fix no effect function here
      updateVelocity(body, bodies, gravitationalConstant, deltaTime);
      updatePosition(body, deltaTime);
    }

    window.clear();

    for (const Body& body : bodies) {
      circle.setPosition(static_cast<float>(body.position.x), static_cast<float>(body.position.y));
      circle.setRadius(body.mass);
      window.draw(circle);
    }

    window.display();
  }
 
  std::cout << "Exited\n";
}

