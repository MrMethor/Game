#include <iostream>
#include "Player.h"

Player::Player(SpriteSheet &spriteSheet) {
    sprite.setTexture(spriteSheet.texture);
    sprite.setTextureRect(sf::IntRect(16, 16, 32, 32));
    sprite.setPosition(x, y);
    sprite.setOrigin(8, 8);
    sprite.setScale(5, 5);
}

void Player::move(int direction) {
    switch (direction) {
    case Left:
        x -= speed;
        break;
    case Right:
        x += speed;
        break;
    case Down:
        y += speed;
        break;
    case Up:
        y -= speed;
        break;
    }
}