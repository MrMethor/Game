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
    double speed = 0.2 / 10;
    int velocityX = 0;
    int velocityY = 0;
    int velocityCap = 4;
	Entity(SpriteSheet &spriteSheet, Specs &specs);
	void move(int direction);
    void setPosition(double x, double y);
    void update();

private:
    Specs specs;
};

