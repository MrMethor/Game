#include "Tile.h"

void Tile::loadTile(char id, SpriteSheet &spriteSheet, Specs &specs) {
	this->specs = specs;
	this->id = id;
	int x = id % spriteSheet.sheetSize * spriteSheet.tileSize;
	int y = id / spriteSheet.sheetSize * spriteSheet.tileSize;
	sprite.setTexture(spriteSheet.texture);
	sprite.setTextureRect(sf::IntRect(x , y, x + specs.lunit, y + specs.lunit));
	sprite.setOrigin(specs.lunit / 2, specs.lunit / 2);
	sprite.setScale(specs.scale, specs.scale);
}