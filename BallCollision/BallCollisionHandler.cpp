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
  if (iaBalls.empty())
    return;

  std::sort(iaBalls.begin(), iaBalls.end(), _comparator);
  float sumx{ 0.f };
  float sumy{ 0.f };
  float sumSqrx{ 0.f };
  float sumSqry{ 0.f };

  for (size_t i = 0; i < iaBalls.size(); ++i) {
    auto& curBall = iaBalls[i];
    const auto& pos = curBall.GetPosition();
    sumx += pos.x; sumy += pos.y;
    sumSqrx += pos.x * pos.x; sumSqry += pos.y * pos.y;

    HandleWindowCollision(curBall);
    for (size_t j = i + 1; j < iaBalls.size(); ++j) {
      auto& otherBall = iaBalls[j];
      if (!_comparator.HasProjectionsIntersection(curBall, otherBall))
        break;

      HandleCollision(curBall, otherBall);
    }
  }

  auto size = iaBalls.size();
  auto sqrMeanx = std::pow(sumx / size, 2);
  auto sqrMeany = std::pow(sumy / size, 2);
  auto meanOfSqrx = sumSqrx / size;
  auto meanOfSqry = sumSqry / size;
  unsigned int maxVarIdx = (meanOfSqrx - sqrMeanx) > (meanOfSqry - sqrMeany) ? 0 : 1;

  _comparator.SetComparisonAxisIdx(maxVarIdx);
}

void BallCollisionHandler::ProcessCollisionsNaive(std::vector<Ball>& iaBalls)
{
  if (iaBalls.empty())
    return;

  for (size_t i = 0; i < iaBalls.size(); ++i) {
    auto& curBall = iaBalls[i];
    HandleWindowCollision(curBall);
    for (size_t j = i + 1; j < iaBalls.size(); ++j) {
      auto& otherBall = iaBalls[j];
      HandleCollision(curBall, otherBall);
    }
  }
}

void BallCollisionHandler::HandleCollision(Ball& iFirst, Ball& iSecond)
{
  const auto sumOfRadiuses = iFirst.GetRadius() + iSecond.GetRadius();
  const auto centersSegment = iFirst.GetPosition() - iSecond.GetPosition();
  const auto length = static_cast<float>(std::sqrt(std::pow(centersSegment.x, 2) + std::pow(centersSegment.y, 2)));

  auto hasCollision =  length < sumOfRadiuses + static_cast<double>(_tolerance);

  if (hasCollision) {
    auto normal = iSecond.GetPosition() - iFirst.GetPosition();
    auto normalLen = static_cast<float>(std::sqrt(std::pow(normal.x, 2) + std::pow(normal.y, 2)));
    normal = sf::Vector2f(normal.x / normalLen, normal.y / normalLen);

    DevideBalls(iFirst, iSecond, length, normal);
    HitBalls(iFirst, iSecond, length, normal);

    iFirst.SetRandomColor();
    iSecond.SetRandomColor();
  }
}

void BallCollisionHandler::HandleWindowCollision(Ball& iBall)
{
  auto& pos = iBall.GetPosition();
  auto& dir = iBall.GetDirection();
  auto radius = iBall.GetRadius();
  auto size = _pWindow->getSize();

  if (pos.x + radius + _tolerance >= size.x) {
    iBall.SetPosition({ size.x - _tolerance - radius, pos.y });
    iBall.SetDirection({ -dir.x, dir.y });
  }
  if (pos.x - radius - _tolerance <= 0) {
    iBall.SetPosition({ radius + _tolerance, pos.y });
    iBall.SetDirection({ -dir.x, dir.y });
  }
  if (pos.y + radius + _tolerance >= size.y) {
    iBall.SetPosition({ pos.x, size.y - _tolerance - radius });
    iBall.SetDirection({ dir.x, -dir.y });
  }
  if (pos.y - radius - _tolerance <= 0) {
    iBall.SetPosition({ pos.x, radius + _tolerance });
    iBall.SetDirection({ dir.x, -dir.y });
  }
}

void BallCollisionHandler::DevideBalls(Ball& iFirst, Ball& iSecond, float iCentersDst, const sf::Vector2f& iNormal)
{
  const auto sumOfRadiuses = iFirst.GetRadius() + iSecond.GetRadius();
  float overlap = (sumOfRadiuses - iCentersDst + _tolerance) * 0.5f;

  iFirst.ShiftPosition(-iNormal * overlap);
  iSecond.ShiftPosition(iNormal * overlap);
}

void BallCollisionHandler::HitBalls(Ball& iFirst, Ball& iSecond, float iCentersDst, const sf::Vector2f& iNormal)
{
  const auto s1 = iFirst.GetSpeed(); const auto s2 = iSecond.GetSpeed();
  const auto m1 = iFirst.GetMass(); const auto m2 = iSecond.GetMass();
  const auto& d1 = iFirst.GetDirection();  const auto& d2 = iSecond.GetDirection();

  // tangent
  float tx = -iNormal.y;
  float ty = iNormal.x;

  // tangent projection
  float tp1 = d1.x * s1 * tx + d1.y * s1 * ty;
  float tp2 = d2.x * s2 * tx + d2.y * s2 * ty;

  // normal projection
  float np1 = d1.x * s1 * iNormal.x + d1.y * s1 * iNormal.y;
  float np2 = d2.x * s2 * iNormal.x + d2.y * s2 * iNormal.y;

  // impulses conservation
  float imp1 = (np1 * (m1 - m2) + 2.0f * m2 * np2) / (m1 + m2);
  float imp2 = (np2 * (m2 - m1) + 2.0f * m1 * np1) / (m1 + m2);

  float newVel1x = tx * tp1 + iNormal.x * imp1;
  float newVel1y = ty * tp1 + iNormal.y * imp1;
  float newVel2x = tx * tp2 + iNormal.x * imp2;
  float newVel2y = ty * tp2 + iNormal.y * imp2;

  float speed1 = static_cast<float>(std::sqrt(std::pow(newVel1x, 2) + std::pow(newVel1y, 2)));
  float speed2 = static_cast<float>(std::sqrt(std::pow(newVel2x, 2) + std::pow(newVel2y, 2)));

  iFirst.SetDirection({ newVel1x / speed1, newVel1y / speed1 });
  iSecond.SetDirection({ newVel2x / speed2, newVel2y / speed2 });

  iFirst.SetSpeed(speed1);
  iSecond.SetSpeed(speed2);
}
