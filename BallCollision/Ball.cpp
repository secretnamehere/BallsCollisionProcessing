#include "Ball.h"
#include <math.h>

Ball::Ball(const sf::Vector2f& iPosition, const sf::Vector2f& iDirection, float iRadius, float iSpeed)
  : _centerPosition(iPosition)
{
  SetDirection(iDirection);
  SetRadius(iRadius);
  SetSpeed(iSpeed);
}

void Ball::UpdateRepresentation()
{
  _SFMLRepresentation.setRadius(_radius);
  _SFMLRepresentation.setPosition(_centerPosition - sf::Vector2f(_radius, _radius));
}

void Ball::Draw(sf::RenderWindow& iWindow)
{
  UpdateRepresentation();
  iWindow.draw(_SFMLRepresentation);
}

void Ball::SetSpeed(float iSpeed)
{
  _speed = std::abs(iSpeed);
}

void Ball::SetRadius(float iRadius)
{
  _radius = std::abs(iRadius);
}

void Ball::SetDirection(const sf::Vector2f& iDirection)
{
  const auto length = std::sqrt(std::pow(iDirection.x, 2) + std::pow(iDirection.y, 2));
  _direction = length == 0 ? sf::Vector2f() : sf::Vector2f(iDirection.x / length, iDirection.y / length);
}

void Ball::SetPosition(const sf::Vector2f& iPosition)
{
  _centerPosition = iPosition;
}

float Ball::GetSpeed() const
{
  return _speed;
}

float Ball::GetRadius() const
{
  return _radius;
}

const sf::Vector2f& Ball::GetDirection() const
{
  return _direction;
}

const sf::Vector2f& Ball::GetPosition() const
{
  return _centerPosition;
}
