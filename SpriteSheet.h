#include <SFML/Graphics.hpp>

#pragma once
class SpriteSheet {
public:
	int tileSize = 0;
	int sheetSize = 0; // in tiles
	sf::Texture texture;
	SpriteSheet(std::string path, int sheetSize, int tileSize);
};