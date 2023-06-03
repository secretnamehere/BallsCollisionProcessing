#pragma once

#include "Ball.h"

namespace sf {
  class RenderWindow;
}

class BallCollisionHandler 
{
public:
  explicit BallCollisionHandler(sf::RenderWindow* ipWindow);
  void ProcessCollisions(std::vector<Ball>& iaBalls);

private:
  sf::RenderWindow* _pWindow{ nullptr };
};