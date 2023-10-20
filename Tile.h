#include "SFML/Graphics.hpp"
#include "SpriteSheet.h"
#include "Specs.h"

#pragma once
class Tile {
	
public:
	sf::Sprite sprite;
	int id = 0;
	void loadTile(char id, SpriteSheet &spriteSheet, Specs &specs);
private:
	Specs specs;
};