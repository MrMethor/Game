#include "Tile.h"
#include "SpriteSheet.h"
#include <iostream>
#include "Specs.h"

#pragma once
class Map {

public:
	SpriteSheet *pSpritesheet;
	bool defined = false;
	int width = 0;
	int height = 0;
	Tile* tiles = nullptr;
	Map(SpriteSheet &spriteSheet, Specs &specs);
	bool loadMap(std::string path);

private:
	Specs specs;
};