#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <cstdlib>
#include <ctime>

#include "Vector2D.h"

// TODO: look up smarter way to specify array size in template parameters than to include this as global
constexpr std::size_t NR_OF_BODIES = 10;

struct Body {
  Vector2D position, velocity;
  double mass;
  bool isActive;

  Body(const Vector2D& position = Vector2D(),
       const Vector2D& velocity = Vector2D(),
       double mass = 0.0,
       bool isActive = true) : position(position), velocity(velocity), mass(mass), isActive(isActive) {}
};

void updateVelocity(Body& body, const std::array<Body, NR_OF_BODIES>& bodies, double gravitationalConstant, double deltaTime) {
  Vector2D acceleration;

  for (const Body& other : bodies) {
    if (&body == &other) continue;

    Vector2D direction = other.position - body.position;
    double distance = direction.magnitude();

    if (distance > 0.0) {
      double forceMagnitude = (gravitationalConstant * body.mass * other.mass) / (distance * distance);
      Vector2D force = direction.normalized() * forceMagnitude + 0.0;
      acceleration += force * (1.0 / body.mass);
    }
  }

  body.velocity += acceleration * deltaTime;
}

void updatePosition(Body& body, double DELTA_TIME) {
  body.position += body.velocity * DELTA_TIME;
}

void spawnBodies(std::array<Body, NR_OF_BODIES>& bodies, std::size_t screenWidth, std::size_t screenHeight, double minMass, double maxMass) {
  std::srand(std::time(nullptr));

  for (std::size_t i = 0; i < NR_OF_BODIES; ++i) {
    double x = std::rand() % screenWidth;
    double y = std::rand() % screenHeight;
    double mass = minMass + (static_cast<double>(std::rand()) / RAND_MAX) * (maxMass - minMass);

    bodies[i] = {Vector2D(x, y), Vector2D(), mass};
  }
}

[[nodiscard]]
bool checkCollision(const Body& body1, const Body& body2) {
  double distance = (body1.position - body2.position).magnitude();
  double sumOfRadii = body1.mass + body2.mass;

  return distance <= sumOfRadii;
}

void mergeBodies(Body& body, Body& other) {
  constexpr double lossFactor = 0.0;

  double newMass = sqrt(body.mass * body.mass + other.mass * other.mass) * (1 - lossFactor);
          
  // Calculate the new position as the center of mass
  Vector2D newPosition = (body.position * body.mass + other.position * other.mass) * (1.0 / (body.mass + other.mass));

  // Calculate the new velocity as the weighted average of velocities
  Vector2D newVelocity = (body.velocity * body.mass + other.velocity * other.mass) * (1.0 / (body.mass + other.mass));

  // Update the collided body
  body.position = newPosition;
  body.velocity = newVelocity;
  body.mass = newMass;

  // Disable the second body
  other.mass = 0.0;
  other.isActive = false;
}

int main() {
  constexpr std::size_t WIDTH = 800,
                        HEIGHT = 600;

  constexpr double GRAVITATIONAL_CONSTANT = 6.674e-3;
  constexpr double DELTA_TIME = 0.05;

  // Setup bodies "backend"
  std::array<Body, NR_OF_BODIES> bodies;
  spawnBodies(bodies, WIDTH, HEIGHT, 3, 15);
  
  // Setup window and objects
  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "N-Body Simulator");

  sf::CircleShape circle;
  circle.setFillColor(sf::Color::Red);

  // Hot loop
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    // TODO: reimplement collision here cuz shit went kaboom and forgot to save
    for (std::size_t i = 0; i < bodies.size(); ++i) {
      Body& body = bodies[i];
      if (!body.isActive) continue;

      updateVelocity(body, bodies, GRAVITATIONAL_CONSTANT, DELTA_TIME);
      updatePosition(body, DELTA_TIME);

      for (std::size_t j = i + 1; j < bodies.size(); ++j) {
        Body& other = bodies[j];
        if (!other.isActive) continue;

        if (checkCollision(body, other))
          mergeBodies(body, other);
      }
    }

    window.clear();

    for (const Body& body : bodies) {
      if (!body.isActive) continue;

      circle.setPosition(static_cast<float>(body.position.x), static_cast<float>(body.position.y));
      circle.setRadius(static_cast<float>(body.mass));
      window.draw(circle);
    }

    window.display();
  }

  std::cout << "Exited\n";
}
