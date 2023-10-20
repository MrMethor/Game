#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "SpriteSheet.h"
#include "Map.h"
#include "Specs.h"


#pragma once
class Game {
private:
    const static int inputBufferSize = 8;
    SpriteSheet spritesheet = SpriteSheet("resources/spritesheet.png", 16, 16);

public:
    Specs specs;
    bool running = true;
    int inputBuffer[inputBufferSize];
    int inputBufferMouse[inputBufferSize];
    int inputBufferWheel[inputBufferSize];
    sf::RenderWindow window;
    Entity player = Entity(spritesheet, specs);
    Map map = Map(spritesheet, specs);
    Game();
    void createWindow();
};

