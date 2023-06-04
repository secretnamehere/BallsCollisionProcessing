#pragma once
#include "Ball.h"
#include "MiddleAverageFilter.h"

class BallCollisionApp
{
public:
  BallCollisionApp();
  virtual ~BallCollisionApp();

  void Init();
  void StartRendering();

private:
  void DrawFPS(float iFps);

private:
  sf::RenderWindow* _pWindow{ nullptr };
  std::vector<Ball> _aBalls;
  Math::MiddleAverageFilter<float, 100> _fpsCounter;

  static constexpr int WINDOW_X = 1024;
  static constexpr int WINDOW_Y = 768;
  static constexpr int MAX_BALLS = 300;
  static constexpr int MIN_BALLS = 50;
};

