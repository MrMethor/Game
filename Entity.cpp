#include <iostream>
#include "Entity.h"

Entity::Entity(SpriteSheet &spriteSheet, Specs &specs) {
    this->specs = specs;
    int sheetX = 1 * spriteSheet.tileSize;
    int sheetY = 1 * spriteSheet.tileSize;
    sprite.setTexture(spriteSheet.texture);
    sprite.setTextureRect(sf::IntRect(sheetX, sheetY, sheetX + specs.lunit, sheetY + specs.lunit));
    sprite.setPosition(x, y);
    sprite.setOrigin(specs.lunit / 2, specs.lunit / 2);
    sprite.setScale(specs.scale, specs.scale);
}

void Entity::move(int direction) {
    switch (direction) {
    case Left:
        if (velocityX > -velocityCap)
            velocityX -= 2;
        break;
    case Right:
        if (velocityX < velocityCap)
            velocityX += 2;
        break;
    case Down:
        if (velocityY < velocityCap)
            velocityY += 2;
        break;
    case Up:
        if (velocityY > -velocityCap)
            velocityY -= 2;
        break;
    }
}

void Entity::updateVelocity(double gamma) {
    x += velocityX * speed * gamma;
    y += velocityY * speed * gamma;

    if (velocityX > 0) {
        velocityX -= gamma;
        if (velocityX < gamma)
            velocityX = 0;
    }
    if (velocityX < 0) {
        velocityX += gamma;
        if (velocityX > -gamma)
            velocityX = 0;
    }

    if (velocityY > 0) {
        velocityY -= gamma;
        if (velocityY < gamma)
            velocityY = 0;
    }
    if (velocityY < 0) {
        velocityY += gamma;
        if (velocityY > -gamma)
            velocityY = 0;
    }
}

void Entity::setPosition(double x, double y){
    this->x = x;
    this->y = y;
}