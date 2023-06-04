#pragma once

#include "SFML/Graphics.hpp"
#include <utility>

class Ball /*in prod it would be smth like : public IInteractionable*/
{
public:
  Ball() = default;
  explicit Ball(const sf::Vector2f& iPosition, const sf::Vector2f& iDirection, float iRadius = 1.f, float iSpeed = 0.f);

  void Draw(sf::RenderWindow& iWindow);
  void ShiftPosition(const sf::Vector2f& iShift);
  void Move(float iDeltaTime);

  void SetSpeed(float iSpeed);
  void SetRadius(float iRadius);
  void SetDirection(const sf::Vector2f& iDirection);
  void SetPosition(const sf::Vector2f& iPosition);
  void SetRandomColor();

  float GetSpeed() const;
  float GetRadius() const;
  const sf::Vector2f& GetDirection() const;
  const sf::Vector2f& GetPosition() const;
  float GetMass() const;

private:
  void UpdateRepresentation();

private:
  sf::Vector2f _centerPosition;
  sf::Vector2f _direction;
  float _radius{ 1.f };
  float _speed{ 0.f };
  float _density{ 1.f };

  sf::CircleShape _SFMLRepresentation;
};

