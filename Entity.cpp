#include <iostream>
#include "Entity.h"
#include <corecrt_math_defines.h>

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

void Entity::updateVelocity() {
    double maxVelX = 0;
    double maxVelY = 0;
    if (degree > -1) {
        double axis = degree * (M_PI / 180.0);
        maxVelX = std::ceil(sin(axis) * 100.0) / 100.0;
        maxVelY = std::ceil(cos(axis) * 100.0) / 100.0;

        velocityX += (velocityX < abs(maxVelX) && velocityX > -abs(maxVelX)) ? friction * maxVelX : 0;
        velocityY += (velocityY < abs(maxVelY) && velocityY > -abs(maxVelY)) ? friction * maxVelY : 0;
    }
    
    std::cout << "degree: " << degree << " maxVelX: " << maxVelX << " maxVelY: " << maxVelY << " velocityX: " << velocityX << " velocityY: " << velocityY << std::endl;

    if (velocityX > abs(maxVelX) || (velocityX > 0 && maxVelX < 0)) {
        std::cout << (velocityX > abs(maxVelX)) << std::endl;
        velocityX -= friction;
        if (velocityX < 0)
            velocityX = 0;
    }
    if (velocityX < -abs(maxVelX) || (velocityX < 0 && maxVelX > 0)) {
        velocityX += friction;
        if (velocityX > 0)
            velocityX = 0;
    }

    if (velocityY > abs(maxVelY) || (velocityY > 0 && maxVelY < 0.69)) {
        velocityY -= friction;
        if (velocityY < 0)
            velocityY = 0;
    }
    if (velocityY < -abs(maxVelY) || (velocityY < 0 && maxVelY > -0.69)) {
        velocityY += friction;
        if (velocityY > 0)
            velocityY = 0;
    }

    //std::cout << "degree: " << degree << " maxVelX: " << maxVelX << " maxVelY: " << maxVelY << " velocityX: " << velocityX << " velocityY: " << velocityY << std::endl;

    x += velocityX * speed / 60;
    y += velocityY * speed / 60;
} 

void Entity::setPosition(double x, double y){
    this->x = x;
    this->y = y;
}
