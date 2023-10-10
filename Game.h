#include <SFML/Graphics.hpp>
#include "Player.h"
#include "SpriteSheet.h"

#pragma once
class Game {
private:
    const static int inputBufferSize = 8;
    SpriteSheet spritesheet = SpriteSheet("resources/spritesheet.png");

public:
    bool running = true;
    int inputBuffer[inputBufferSize];
    sf::RenderWindow window;
    Player player = Player(spritesheet);
    Game();
};

