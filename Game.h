#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "SpriteSheet.h"
#include "Map.h"
#include "Specs.h"
#include "Controls.h"


#pragma once
class Game {
private:
    const static int inputBufferSize = 8;
    SpriteSheet spritesheet = SpriteSheet("resources/spritesheet.png", 16, 16);

public:
    Specs specs;
    Controls controls;
    bool running = true;
    int keyActions[256];
    int inputBuffer[inputBufferSize];
    int inputBufferMouse[inputBufferSize];
    int inputBufferWheel = 0;
    sf::RenderWindow window;
    Entity player = Entity(spritesheet, specs);
    Map map = Map(spritesheet, specs);
    Game();
    void createWindow();
    void loadControls();
};

