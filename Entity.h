#include <SFML/Graphics.hpp>
#include "SpriteSheet.h"
#include "Directions.h"
#include "Specs.h"

#pragma once
class Entity {
public:
    sf::Sprite sprite;
    double previousX = 0;
    double previousY = 0;
    double x = 0;
    double y = 0;
    double speed = 3;
    double friction = 0.1;
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

