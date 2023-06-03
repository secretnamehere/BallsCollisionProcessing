#pragma once

#include "SFML/Graphics.hpp"
#include "MiddleAverageFilter.h"
#include "BallCollisionHandler.h"
#include "Ball.h"

constexpr int WINDOW_X = 1024;
constexpr int WINDOW_Y = 768;
constexpr int MAX_BALLS = 150;
constexpr int MIN_BALLS = 50;

Math::MiddleAverageFilter<float, 100> fpscounter;

void draw_ball(sf::RenderWindow& iWindow, Ball& iBall)
{
  iBall.Draw(iWindow);
}

void move_ball(Ball& iBall, float iDeltaTime)
{
  auto dir = iBall.GetDirection();
  auto speed = iBall.GetSpeed();
  auto pos = iBall.GetPosition();

  float dx = dir.x * speed * iDeltaTime;
  float dy = dir.y * speed * iDeltaTime;
  iBall.SetPosition({ pos.x += dx, pos.y += dy });
}

void draw_fps(sf::RenderWindow& window, float fps)
{
    char c[32];
    snprintf(c, 32, "FPS: %f", fps);
    sf::String str(c);
    window.setTitle(str);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Ball collision demo");
    srand(time(NULL));

    std::vector<Ball> balls;

    // randomly initialize balls
    for (int i = 0; i < (rand() % (MAX_BALLS - MIN_BALLS) + MIN_BALLS); i++)
    {
        Ball newBall;
        newBall.SetPosition({ static_cast<float>(rand() % WINDOW_X), static_cast<float>(rand() % WINDOW_Y) });
        newBall.SetDirection({ static_cast<float>((-5 + (rand() % 10))) / 3.f, static_cast<float>((-5 + (rand() % 10)) / 3.f) });
        newBall.SetRadius(10 + rand() % 5);
        newBall.SetSpeed(30 + rand() % 30);
        balls.push_back(newBall);
    }

   // window.setFramerateLimit(60);

    sf::Clock clock;
    float lastime = clock.restart().asSeconds();
    BallCollisionHandler handler(&window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float current_time = clock.getElapsedTime().asSeconds();
        float deltaTime = current_time - lastime;
        fpscounter.push(1.0f / (current_time - lastime));
        lastime = current_time;

        /// <summary>
        /// TODO: PLACE COLLISION CODE HERE 
        /// объекты создаются в случайном месте на плоскости со случайным вектором скорости, имеют радиус R
        /// Объекты движутся кинетически. Пространство ограниченно границами окна
        /// Напишите обработчик столкновений шаров между собой и краями окна. Как это сделать эффективно?
        /// Массы пропорцианальны площадям кругов, описывающих объекты 
        /// Как можно было-бы улучшить текущую архитектуру кода?
        /// Данный код является макетом, вы можете его модифицировать по своему усмотрению

        handler.ProcessCollisions(balls);

        for (auto& ball : balls) {
            move_ball(ball, deltaTime);
        }

        window.clear();
        for (auto& ball : balls) { //No need to copy the ball in loop
            draw_ball(window, ball);
        }

		draw_fps(window, fpscounter.getAverage());
		window.display();
    }
    return 0;
}
