#include <SFML/Graphics.hpp>
#include "SpriteSheet.h"

#pragma once
class Entity {
public:
    sf::Sprite sprite;

    Entity(SpriteSheet& spriteSheet);
    void updateVelocity(char movingX, char movingY);
    void setPosition(double x, double y);

    double getX() const;
    double getY() const;
    double getPreviousX() const;
    double getPreviousY() const;

private:
    double x = 0;
    double y = 0;
    double previousX = 0;
    double previousY = 0;

    double speed = 3;
    double friction = 0.1;
    int degree = -1;
    double velocityX = 0;
    double velocityY = 0;
};

