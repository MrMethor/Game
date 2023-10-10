#include <SFML/Graphics.hpp>
#include "SpriteSheet.h"
#include "Directions.h"

#pragma once
class Player {
public:
    sf::Sprite sprite;
    double x = 0;
    double y = 0;
    double speed = 20;
	Player(SpriteSheet &spriteSheet);
	void move(int direction);
};

