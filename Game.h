#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "SpriteSheet.h"
#include "Map.h"
#include "Specs.h"
#include "Controls.h"

#pragma once
class Game {

public:
    sf::RenderWindow window;
    bool running = true;

    Game();
    void update();
    void render(double interpolationModifier, int fps);

    double getDesiredUPS() const;

private:
    Specs specs;
    Controls controls;
    SpriteSheet spritesheet = SpriteSheet("res/spritesheet.png");
    sf::View view;
    Entity player = Entity(spritesheet);
    Map map = Map(spritesheet);
    const static int INPUT_BUFFER_SIZE = 8;
    int inputBuffer[INPUT_BUFFER_SIZE];
    int inputBufferMouse[INPUT_BUFFER_SIZE];
    int inputBufferWheel = 0;
    int keyActions[256];
    double cameraOffsetX = 0;
    double cameraOffsetY = 0;

    void createWindow();
    void zoom(double speed);
};

