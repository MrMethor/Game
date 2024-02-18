#include "Map.h"
#include <fstream>

void fileError(char* content);

Map::Map(SpriteSheet &spriteSheet) {
	this->spriteSheet = &spriteSheet;
}

// Loads map from a file
bool Map::loadMap(std::string path) {
	std::ifstream file (path);
	if (file.is_open()) {

		width = file.get();
		height = file.get();

		char* pContent = new char[width * height];

		for (int i = 0; i < width * height; i++) {
			if (!file) {
				fileError(pContent);
				return false;
			}
			pContent[i] = file.get();
		}

		Tile* pTiles = new Tile[width * height];
		for (int i = 0; i < width * height; i++)
			pTiles[i].loadTile(pContent[i], *spriteSheet);

		delete[] pContent;

		if (tiles != nullptr)
			delete[] tiles;
		tiles = pTiles;

		defined = true;
		return true;
	}
	else {
		std::cout << "The file could not have been loaded";
		return false;
	}
}

// Frees memory and prints error to the console
void fileError(char* content) {
	delete content;
	std::cout << "The file is not compatible";
}

bool Map::isDefined() const {
	return defined;
}
int Map::getWidth() const {
	return width;
}
int Map::getHeight() const {
	return height;
}