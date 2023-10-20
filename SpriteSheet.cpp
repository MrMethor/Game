#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(std::string path, int sheetSize, int tileSize) {
    this->sheetSize = sheetSize;
    this->tileSize = tileSize;
    texture.loadFromFile(path);
}