#include "BallCollisionHandler.h"
#include "SFML/Graphics.hpp"

void BallCollisionAxisComparator::SetComparisonAxisIdx(unsigned int iIdx)
{
  _comparisonAxisIdx = iIdx % 2;
}

bool BallCollisionAxisComparator::HasProjectionsIntersection(const Ball& iFirst, const Ball& iSecond)
{
  float firstMax = _comparisonAxisIdx == 0 ? iFirst.GetPosition().x + iFirst.GetRadius() : iFirst.GetPosition().y + iFirst.GetRadius();
  float secondMin = _comparisonAxisIdx == 0 ? iSecond.GetPosition().x - iSecond.GetRadius() : iSecond.GetPosition().y - iSecond.GetRadius();

  return firstMax > secondMin;
}

bool BallCollisionAxisComparator::operator()(const Ball& iFirst, const Ball& iSecond)
{
  float firstMin = _comparisonAxisIdx == 0 ? iFirst.GetPosition().x - iFirst.GetRadius() : iFirst.GetPosition().y - iFirst.GetRadius();
  float secondMin = _comparisonAxisIdx == 0 ? iSecond.GetPosition().x - iSecond.GetRadius() : iSecond.GetPosition().y - iSecond.GetRadius();

  return firstMin < secondMin;
}

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

  if (iaBalls.empty())
    return;

  std::sort(iaBalls.begin(), iaBalls.end(), _comparator);
  sf::Vector2f sum(0.f, 0.f);
  sf::Vector2f sumSqr(0.f, 0.f);

  for (size_t i = 0; i < iaBalls.size(); ++i) {
    auto& curBall = iaBalls[i];
    const auto& pos = curBall.GetPosition();
    sum += pos;
    sumSqr += sf::Vector2f{ pos.x * pos.x, pos.y * pos.y };

    for (size_t j = i + 1; j < iaBalls.size(); ++j) {
      auto& otherBall = iaBalls[j];
      if (!_comparator.HasProjectionsIntersection(curBall, otherBall))
        break;

      if (HasBallsCollision(curBall, otherBall)) {


        curBall.SetRandomColor();
        otherBall.SetRandomColor();
      }
    }
  }

  auto size = iaBalls.size();
  sf::Vector2f sqrMean(std::pow(sum.x / size, 2), std::pow(sum.y / size, 2));
  sf::Vector2f meanOfSqr(sumSqr.x / size, sumSqr.y / size);

  sf::Vector2f sampVariance{ meanOfSqr - sqrMean };
  unsigned int maxVarIdx = sampVariance.x > sampVariance.y ? 0 : 1;

  _comparator.SetComparisonAxisIdx(maxVarIdx);
}

bool BallCollisionHandler::HasBallsCollision(const Ball& iFirst, const Ball& iSecond)
{
  const auto sumOfRadiuses = iFirst.GetRadius() + iSecond.GetRadius();
  const auto centersSegment = iFirst.GetPosition() - iSecond.GetPosition();
  const auto length = std::sqrt(std::pow(centersSegment.x, 2) + std::pow(centersSegment.y, 2));

  return length < sumOfRadiuses + static_cast<double>(_tolerance);
}

void BallCollisionHandler::DevideBalls(Ball& iFirst, Ball& iSecond)
{
}

void BallCollisionHandler::HitBalls(Ball& iFirst, Ball& iSecond)
{
}
