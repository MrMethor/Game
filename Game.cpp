#include "Game.h"
#include "Actions.h"
#include <corecrt_math_defines.h>
#include <fstream>

// Constructor creates the Window and declares the Input Buffer
Game::Game() {
    createWindow();
    for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
        inputBuffer[i] = -1;
    for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
        inputBufferMouse[i] = -1;
    map.loadMap("resources/overworld.map");
    player.setPosition(map.getWidth() / 2, map.getHeight() / 2);
    //lightMap.create(window.getSize().x, window.getSize().y);
    specs.spriteScale = spritesheet.getTileSize() / 16 * specs.scale;
    updateSprites();
}

// Creates and sets up the window
void Game::createWindow() {
    specs.defaultFont.loadFromFile("resources/arial.ttf");
    if (specs.fullscreen)
        window.create(sf::VideoMode(specs.fullscreenWidth, specs.fullscreenHeight, 32), "", sf::Style::Fullscreen);
    else
        window.create(sf::VideoMode(specs.width, specs.height, 32), "Game", sf::Style::Titlebar | sf::Style::Close);
    window.setActive();
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(false);
    sf::Image icon;
    icon.loadFromFile("resources/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void Game::update() {
    // Adds keys to the input buffer
    std::cout << specs.spriteScale << "\n";
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::KeyPressed:
            for (int i = 0; i < sizeof(inputBuffer) / sizeof(inputBuffer[0]); i++) {
                if (inputBuffer[i] == -1) {
                    inputBuffer[i] = event.key.code;
                    break;
                }
            }
            break;
        case sf::Event::KeyReleased:
            for (int i = 0; i < sizeof(inputBuffer) / sizeof(inputBuffer[0]); i++) {
                if (inputBuffer[i] == event.key.code)
                    inputBuffer[i] = -1;
            }
            break;
        case sf::Event::MouseButtonPressed:
            for (int i = 0; i < sizeof(inputBufferMouse) / sizeof(inputBufferMouse[0]); i++) {
                if (inputBufferMouse[i] == -1) {
                    inputBufferMouse[i] = event.key.code;
                    break;
                }
            }
            break;
        case sf::Event::MouseButtonReleased:
            for (int i = 0; i < sizeof(inputBufferMouse) / sizeof(inputBufferMouse[0]); i++) {
                if (inputBufferMouse[i] == event.key.code)
                    inputBufferMouse[i] = -1;
            }
            break;
        case sf::Event::MouseWheelMoved:
            inputBufferWheel += event.mouseWheel.delta;
            break;
        case sf::Event::Closed:
            window.close();
            running = false;
        }
    }

    // Compares the input buffer to the keymap and applies the actions based on the keymap
    char movingX = 0;
    char movingY = 0;
    for (int i = 0; i < sizeof(inputBuffer) / sizeof(inputBuffer[0]); i++) {
        if (inputBuffer[i] != -1) {
            switch (controls.keymap[inputBuffer[i]]) {
            case moveLeft:
                movingX--;
                break;
            case moveRight:
                movingX++;
                break;
            case moveDown:
                movingY++;
                break;
            case moveUp:
                movingY--;
                break;
            case fullscreen:
                specs.fullscreen = !specs.fullscreen;
                createWindow();
                for (int i = 0; i < sizeof(inputBuffer) / sizeof(inputBuffer[0]); i++)
                    inputBuffer[i] = -1;
                break;
            case openMenu:
                window.close();
                running = false;
            }
        }
    }

    // MOUSE WORK IN PROGRESS
    for (int i = 0; i < sizeof(inputBufferMouse) / sizeof(inputBufferMouse[0]); i++) {
        switch (inputBufferMouse[i]) {
        case sf::Mouse::Left:
            //M1
            break;
        case sf::Mouse::Right:
            //M2
            break;
        case sf::Mouse::Middle:
            //M3
            break;
        }
    }

    // Mouse wheel input
    while (inputBufferWheel > 0) {
        zoom(true);
        inputBufferWheel--;
    }
    while (inputBufferWheel < 0) {
        zoom(false);
        inputBufferWheel++;
    }

    player.updateVelocity(movingX, movingY);
}

// Zooms the camera in or out
void Game::zoom(bool in) {

    double zoomSpeed = 0.1;
    int maxScale = 10;
    int minScale = 5;
    if (in && specs.scale < maxScale)
        specs.scale += zoomSpeed;
    else if (!in && specs.scale > minScale)
        specs.scale -= zoomSpeed;
    else
        return;

    updateSprites();
}

void Game::updateSprites() {
    player.sprite.setScale(specs.spriteScale, specs.spriteScale);
    if (map.isDefined()) {
        for (int y = 0; y < map.getHeight(); y++) {
            for (int x = 0; x < map.getWidth(); x++)
                map.tiles[x + y * map.getWidth()].sprite.setScale(specs.spriteScale, specs.spriteScale);
        }
    }
    specs.spriteScale = spritesheet.getTileSize() / 16.0 * specs.scale;
}


// Rander Method
void Game::render(double interpolationModifier, int fps) {

    window.clear();

    // Calculates the interpolation between frames        
    double interpolationX = player.getPreviousX() + (player.getX() - player.getPreviousX()) * interpolationModifier;
    double interpolationY = player.getPreviousY() + (player.getY() - player.getPreviousY()) * interpolationModifier;

    cameraOffset(interpolationX, interpolationY);
    renderMap(interpolationX, interpolationY);
    renderPlayer();
    //game.renderLightmap();

    sf::Text textFPS;
    textFPS.setFont(specs.defaultFont);
    textFPS.setCharacterSize(12);
    textFPS.setFillColor(sf::Color::Black);
    textFPS.setString(std::to_string(fps) + " fps");
    window.draw(textFPS);

    window.display();
}

// Calculates the camera offset based on the player's position
void Game::cameraOffset(double interpolationX, double interpolationY) {
    cameraOffsetX = 0;
    cameraOffsetY = 0;
    double left = -((0 * specs.spriteScale * 16 - (interpolationX * specs.spriteScale * 16) + window.getSize().x / 2) - (specs.spriteScale / 2 * 16));
    double right = (map.getWidth() * specs.spriteScale * 16 - (interpolationX * specs.spriteScale * 16) + window.getSize().x / 2) - (specs.spriteScale / 2 * 16) - window.getSize().x;
    double up = -((0 * specs.spriteScale * 16 - (interpolationY * specs.spriteScale * 16) + window.getSize().y / 2) - (specs.spriteScale / 2 * 16));
    double down = (map.getHeight() * specs.spriteScale * 16 - (interpolationY * specs.spriteScale * 16) + window.getSize().y / 2) - (specs.spriteScale / 2 * 16) - window.getSize().y;

    if (left < 0)
        cameraOffsetX += left;
    if (right < 0)
        cameraOffsetX -= right;
    if (right + left < 0)
        cameraOffsetX = cameraOffsetX / 2;

    if (up < 0)
        cameraOffsetY += up;
    if (down < 0)
        cameraOffsetY -= down;
    if (up + down < 0)
        cameraOffsetY = cameraOffsetY / 2;
}

// Renders the map tiles
void Game::renderMap(double interpolationX, double interpolationY) {
    if (map.isDefined()) {
        for (int y = 0; y < map.getHeight(); y++) {
            for (int x = 0; x < map.getWidth(); x++) {
                if (x < interpolationX + 16) {}
                map.tiles[x + y * map.getWidth()].sprite.setPosition((double)x * specs.spriteScale * 16 - (interpolationX * specs.spriteScale * 16) + window.getSize().x / 2 + cameraOffsetX,
                    (double)y * specs.spriteScale * 16 - (interpolationY * specs.spriteScale * 16) + window.getSize().y / 2 + cameraOffsetY);
                if (x < interpolationX + specs.renderDistance - cameraOffsetX / specs.spriteScale / 16 && x > interpolationX - specs.renderDistance - cameraOffsetX / specs.spriteScale / 16
                    && y < interpolationY + specs.renderDistance / 1.7 - cameraOffsetY / specs.spriteScale / 16 && y > interpolationY - specs.renderDistance / 1.7 - cameraOffsetY / specs.spriteScale / 16)
                    window.draw(map.tiles[x + y * map.getWidth()].sprite);
            }
        }
    }
}

// Renders the player
void Game::renderPlayer() {
    player.sprite.setPosition(window.getSize().x / 2 + cameraOffsetX, window.getSize().y / 2 + cameraOffsetY);
    window.draw(player.sprite);
}

// Renders the lightmap
/*void Game::renderLightmap() {
    sf::RectangleShape background;
    background.setFillColor(sf::Color(0, 0, 0, 150));
    background.setSize(sf::Vector2f(20, 20));
    lightMap.clear();
    lightMap.draw(background);
    window.draw(sf::Sprite (lightMap.getTexture()));
}*/

double Game::getDesiredUPS() const {
    return specs.desiredUPS;
}