#include <SFML/Graphics.hpp>
#include "SpriteSheet.h"
#include "Directions.h"
#include "Specs.h"

#pragma once
class Entity {
public:
    sf::Sprite sprite;
    double x = 0;
    double y = 0;
    double speed = 5;
    double friction = 1;
    double velocityX = 0;
    double velocityY = 0;
    int degree = -1;
	Entity(SpriteSheet &spriteSheet, Specs &specs);
	void move(int direction);
    void setPosition(double x, double y);
    void addFriction(double &velocity);
    void updateVelocity();

private:
    Specs specs;
};

