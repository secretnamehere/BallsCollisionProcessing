#include "BallCollisionHandler.h"
#include "SFML/Graphics.hpp"

BallCollisionHandler::BallCollisionHandler(sf::RenderWindow* ipWindow)
  : _pWindow(ipWindow)
{
}

void BallCollisionHandler::ProcessCollisions(std::vector<Ball>& iaBalls)
{
  for (auto& ball : iaBalls) {
    auto pos = ball.GetPosition();
    auto radius = ball.GetRadius();
    auto dir = ball.GetDirection();
    if (pos.x + radius >= _pWindow->getSize().x || pos.x - radius <= 0)
      ball.SetDirection({ -dir.x, dir.y });
    if (pos.y + radius >= _pWindow->getSize().y || pos.y - radius <= 0)
      ball.SetDirection({ dir.x, -dir.y });
  }
}
