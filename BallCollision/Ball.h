#pragma once

#include "SFML/Graphics.hpp"

class Ball
{
public:
  Ball() = default;
  Ball(const sf::Vector2f& iPosition, const sf::Vector2f& iDirection, float iRadius = 1.f, float iSpeed = 0.f);

  void UpdateRepresentation();
  void Draw(sf::RenderWindow& iWindow);

  void SetSpeed(float iSpeed);
  void SetRadius(float iRadius);
  void SetDirection(const sf::Vector2f& iDirection);
  void SetPosition(const sf::Vector2f& iPosition);

  float GetSpeed() const;
  float GetRadius() const;
  const sf::Vector2f& GetDirection() const;
  const sf::Vector2f& GetPosition() const;

private:
  sf::Vector2f _centerPosition;
  sf::Vector2f _direction;
  float _radius{ 1.f };
  float _speed{ 0.f };

  sf::CircleShape _SFMLRepresentation;
};

