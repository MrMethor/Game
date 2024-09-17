#include "Tile.h"
#include "SpriteSheet.h"
#include <iostream>

#pragma once
class Map {
public:
	Tile* tiles = nullptr;

	Map(SpriteSheet& spriteSheet);
	bool loadMap(std::string path);

	bool isDefined() const;
	int getWidth() const;
	int getHeight() const;

private:
	SpriteSheet *spriteSheet;
	bool defined = false;
	int width = 0;
	int height = 0;
};