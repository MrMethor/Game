#include "SpriteSheet.h"
#include <iostream>

// Checks if the provided Sprite Sheet is valid and sets it up
SpriteSheet::SpriteSheet(std::string path) {
    texture.loadFromFile(path);
    if (texture.getSize().x == texture.getSize().y)
        if (texture.getSize().x % GRID_SIZE == 0)
            if ((texture.getSize().x / GRID_SIZE) % 2 == 0)
                defined = true;
    if (defined)
        tileSize = texture.getSize().x / GRID_SIZE;
    else
        std::cout << "There has been a problem with loading up the spritesheet file";
}

//Getters
bool SpriteSheet::isDefined() const {
    return defined;
}
int SpriteSheet::getTileSize() const {
    return tileSize;
}

