#include "Tile.h"

void Tile::loadTile(char id, SpriteSheet &spriteSheet) {
	this->id = id;
	int x = id % spriteSheet.getTileSize() * spriteSheet.GRID_SIZE;
	int y = id / spriteSheet.getTileSize() * spriteSheet.GRID_SIZE;
	sprite.setTexture(spriteSheet.texture);
	sprite.setTextureRect(sf::IntRect(x , y, x + spriteSheet.getTileSize(), y + spriteSheet.getTileSize()));
	sprite.setOrigin(spriteSheet.getTileSize() / 2, spriteSheet.getTileSize() / 2);
}