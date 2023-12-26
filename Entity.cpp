#include <iostream>
#include "Entity.h"
#include <corecrt_math_defines.h>

Entity::Entity(SpriteSheet& spriteSheet, Specs& specs) {
    this->specs = specs;
    int sheetX = 1 * spriteSheet.tileSize;
    int sheetY = 1 * spriteSheet.tileSize;
    sprite.setTexture(spriteSheet.texture);
    sprite.setTextureRect(sf::IntRect(sheetX, sheetY, sheetX + specs.lunit, sheetY + specs.lunit));
    sprite.setPosition(x, y);
    sprite.setOrigin(specs.lunit / 2, specs.lunit / 2);
    sprite.setScale(specs.scale, specs.scale);
}

void Entity::updateVelocity() {
    double maxVelX = 0;
    double maxVelY = 0;
    if (degree > -1) {
        double axis = degree * (M_PI / 180.0);
        maxVelX = std::ceil(sin(axis) * 100.0) / 100.0;
        maxVelY = std::ceil(cos(axis) * 100.0) / 100.0;
        
        if (velocityX < abs(maxVelX) && velocityX > -abs(maxVelX)) {
            if (velocityX <= abs(maxVelX) && velocityX + friction * maxVelX > abs(maxVelX))
				velocityX = maxVelX;
            else if (velocityX >= -abs(maxVelX) && velocityX + friction * maxVelX < -abs(maxVelX))
                velocityX = maxVelX;
            else
				velocityX += friction * maxVelX;
        }
        if (velocityY < abs(maxVelY) && velocityY > -abs(maxVelY)) {
            if (velocityY <= abs(maxVelY) && velocityY + friction * maxVelY > abs(maxVelY))
                velocityY = maxVelY;
            else if (velocityY >= -abs(maxVelY) && velocityY + friction * maxVelY < -abs(maxVelY))
                velocityY = maxVelY;
            else
                velocityY += friction * maxVelY;
        }
    }

    if (velocityX > abs(maxVelX) || (velocityX > 0 && maxVelX < 0)) {
        velocityX -= friction;
        if (velocityX < 0)
            velocityX = 0;
    }
    if (velocityX < -abs(maxVelX) || (velocityX < 0 && maxVelX > 0)) {
        velocityX += friction;
        if (velocityX > 0)
            velocityX = 0;
    }

    if (velocityY > abs(maxVelY) || (velocityY > 0 && maxVelY < 0)) {
        velocityY -= friction;
        if (velocityY < 0)
            velocityY = 0;
    }
    if (velocityY < -abs(maxVelY) || (velocityY < 0 && maxVelY > 0)) {
        velocityY += friction;
        if (velocityY > 0)
            velocityY = 0;
    }

    x += velocityX * speed / 60;
    y += velocityY * speed / 60;
} 

void Entity::setPosition(double x, double y){
    this->x = x;
    this->y = y;
}
