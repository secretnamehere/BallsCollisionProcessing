#pragma once

#include "Ball.h"

namespace sf {
  class RenderWindow;
}

class BallCollisionAxisComparator
{
public:
  BallCollisionAxisComparator() = default;
  void SetComparisonAxisIdx(unsigned int iIdx);
  bool HasProjectionsIntersection(const Ball& iFirst, const Ball& iSecond);
  bool operator() (const Ball& iFirst, const Ball& iSecond);

private:
  unsigned int _comparisonAxisIdx{ 0u };
};

class BallCollisionHandler 
{
public:
  explicit BallCollisionHandler(sf::RenderWindow* ipWindow);
  void ProcessCollisions(std::vector<Ball>& iaBalls);

private:
  bool HasBallsCollision(const Ball& iFirst, const Ball& iSecond);
  void DevideBalls(Ball& iFirst, Ball& iSecond);
  void HitBalls(Ball& iFirst, Ball& iSecond);

private:
  sf::RenderWindow* _pWindow{ nullptr };
  BallCollisionAxisComparator _comparator;

  static constexpr float _tolerance{ 2.f };
};