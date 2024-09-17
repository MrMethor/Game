#include "SFML/Graphics.hpp"
#include "SpriteSheet.h"

#pragma once
class Tile {
public:
	sf::Sprite sprite;
	void loadTile(char id, SpriteSheet& spriteSheet);

private:
	int id = 0;
};