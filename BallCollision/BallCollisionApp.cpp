#include "BallCollisionApp.h"
#include "BallCollisionHandler.h"

BallCollisionApp::BallCollisionApp()
{
  _pWindow = new sf::RenderWindow(sf::VideoMode(WINDOW_X, WINDOW_Y), "Ball collision demo");
}

BallCollisionApp::~BallCollisionApp()
{
  if (_pWindow)
    delete _pWindow;
}

void BallCollisionApp::Init()
{
  auto numOfBalls = (rand() % (MAX_BALLS - MIN_BALLS) + MIN_BALLS);
  _aBalls.reserve(numOfBalls);

  for (int i = 0; i < numOfBalls; i++)
  {
    Ball newBall;
    newBall.SetPosition({ static_cast<float>(rand() % WINDOW_X), static_cast<float>(rand() % WINDOW_Y) });
    newBall.SetDirection({ static_cast<float>((-5 + (rand() % 10))) / 3.f, static_cast<float>((-5 + (rand() % 10)) / 3.f) });
    newBall.SetRadius(10 + rand() % 5);
    newBall.SetSpeed(30 + rand() % 30);
    _aBalls.push_back(newBall);
  }
}

void BallCollisionApp::StartRendering()
{
  srand(time(NULL));

  // window.setFramerateLimit(60);

  sf::Clock clock;
  float lastime = clock.restart().asSeconds();
  BallCollisionHandler collisionHandler(_pWindow);

  while (_pWindow->isOpen())
  {
    sf::Event event;
    while (_pWindow->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        _pWindow->close();
      }
    }

    float current_time = clock.getElapsedTime().asSeconds();
    float deltaTime = current_time - lastime;
    _fpsCounter.push(1.0f / (current_time - lastime));
    lastime = current_time;

    collisionHandler.ProcessCollisions(_aBalls);

    for (auto& ball : _aBalls)
      ball.Move(deltaTime);

    _pWindow->clear();

    for (auto& ball : _aBalls)
      ball.Draw(*_pWindow);


    DrawFPS(_fpsCounter.getAverage());
    _pWindow->display();
  }
}

void BallCollisionApp::DrawFPS(float iFps)
{
  char c[32];
  snprintf(c, 32, "FPS: %f", iFps);
  sf::String str(c);
  _pWindow->setTitle(str);
}
