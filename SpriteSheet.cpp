#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(std::string path) {
    texture.loadFromFile(path);
}