#include "Entity.h"
#include <corecrt_math_defines.h>

Entity::Entity(SpriteSheet& spriteSheet) {
    int pixelX = 1 * spriteSheet.getTileSize();
    int pixelY = 1 * spriteSheet.getTileSize();
    sprite.setTexture(spriteSheet.texture);
    sprite.setTextureRect(sf::IntRect(pixelX, pixelY, pixelX + spriteSheet.getTileSize(), pixelY + spriteSheet.getTileSize()));
    sprite.setPosition(x, y);
    sprite.setOrigin(spriteSheet.getTileSize() / 2, spriteSheet.getTileSize() / 2);
}

// Updates the sprite's velocity and position
void Entity::updateVelocity(char movingX, char movingY) {

    previousX = x;
    previousY = y;

    double maxVelX = 0;
    double maxVelY = 0;
    double maxVelAbsX = 0;
    double maxVelAbsY = 0;

    if (movingX == 0 && movingY == 0)
        degree = -1;
    else {
        degree = atan2(movingX, movingY) * (180.0 / M_PI);
        degree = degree < 0 ? degree + 360 : degree;
    }

    // Based on the way player is moving, adds velocity in that direction
    if (degree > -1) {
        double axis = degree * (M_PI / 180.0);
        maxVelX = sin(axis);
        maxVelY = cos(axis);
        maxVelAbsX = abs(maxVelX);
        maxVelAbsY = abs(maxVelY);
        
        if (velocityX < maxVelAbsX && velocityX > -maxVelAbsX) {
            if (velocityX <= maxVelAbsX && velocityX + friction * maxVelX > maxVelAbsX)
				velocityX = maxVelX;
            else if (velocityX >= -maxVelAbsX && velocityX + friction * maxVelX < -maxVelAbsX)
                velocityX = maxVelX;
            else
				velocityX += friction * maxVelX;
        }
        if (velocityY < maxVelAbsY && velocityY > -maxVelAbsY) {
            if (velocityY <= maxVelAbsY && velocityY + friction * maxVelY > maxVelAbsY)
                velocityY = maxVelY;
            else if (velocityY >= -maxVelAbsY && velocityY + friction * maxVelY < -maxVelAbsY)
                velocityY = maxVelY;
            else
                velocityY += friction * maxVelY;
        }
    }

    // If player is not moving or is moving faster then movement speed allows him, it slows down velocity
    if (velocityX > maxVelAbsX || (velocityX > 0 && maxVelX < 0)) {
        velocityX -= friction;
        if (velocityX < 0)
            velocityX = 0;
    }
    if (velocityX < -maxVelAbsX || (velocityX < 0 && maxVelX > 0)) {
        velocityX += friction;
        if (velocityX > 0)
            velocityX = 0;
    }

    if (velocityY > maxVelAbsY || (velocityY > 0 && maxVelY < 0)) {
        velocityY -= friction;
        if (velocityY < 0)
            velocityY = 0;
    }
    if (velocityY < -maxVelAbsY || (velocityY < 0 && maxVelY > 0)) {
        velocityY += friction;
        if (velocityY > 0)
            velocityY = 0;
    }

    // Finally adds velocity to position
    x += velocityX * speed / 60;
    y += velocityY * speed / 60;
} 

// Updates the sprite's position
void Entity::setPosition(double x, double y){
    this-> x = x;
    this-> y = y;
}

// Getters
double Entity::getX() const {
    return x;
}
double Entity::getY() const {
    return y;
}
double Entity::getPreviousX() const {
    return previousX;
}
double Entity::getPreviousY() const {
    return previousY;
}
