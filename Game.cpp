#include "Game.h"
#include <fstream>

// Constructor creates the Window and declares the Input Buffer
Game::Game() {
    specs.defaultFont.loadFromFile("resources/arial.ttf");
    createWindow();
    sf::Image icon;
    icon.loadFromFile("resources/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    for (int i = 0; i < inputBufferSize; i++)
        inputBuffer[i] = -1;
    for (int i = 0; i < inputBufferSize; i++)
        inputBufferMouse[i] = -1;
    map.loadMap("resources/overworld.map");
    player.setPosition(map.width / 2, map.height / 2);
}

// Creates and sets up the window
void Game::createWindow() {
    if (specs.fullscreen)
        window.create(sf::VideoMode(), "", sf::Style::Fullscreen);
    else
        window.create(sf::VideoMode(specs.width, specs.height), "", sf::Style::Titlebar | sf::Style::Close);
    window.setActive();
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(false);
}

// Loads the controls from the file
void Game::loadControls() {
	std::ifstream file("user/controls.con");
	if (!file.is_open())
        file.open("default/controls.con");
}

// Calculates the camera offset based on the player's position
void Game::cameraOffset() {
    cameraOffsetX = 0;
    cameraOffsetY = 0;
    double left = -((0 * specs.lunit * specs.scale - (player.interpolationX * specs.lunit * specs.scale) + window.getSize().x / 2) - (specs.lunit / 2 * specs.scale));
    double right = (map.width * specs.lunit * specs.scale - (player.interpolationX * specs.lunit * specs.scale) + window.getSize().x / 2) - (specs.lunit / 2 * specs.scale) - window.getSize().x;
    double up = -((0 * specs.lunit * specs.scale - (player.interpolationY * specs.lunit * specs.scale) + window.getSize().y / 2) - (specs.lunit / 2 * specs.scale));
    double down = (map.height * specs.lunit * specs.scale - (player.interpolationY * specs.lunit * specs.scale) + window.getSize().y / 2) - (specs.lunit / 2 * specs.scale) - window.getSize().y;

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
void Game::renderMap() {
    if (map.defined) {
        for (int y = 0; y < map.height; y++) {
            for (int x = 0; x < map.width; x++) {
                if (x < player.interpolationX + 16) {}
                map.tiles[x + y * map.width].sprite.setPosition((double)x * specs.lunit * specs.scale - (player.interpolationX * specs.lunit * specs.scale) + window.getSize().x / 2 + cameraOffsetX,
                    (double)y * specs.lunit * specs.scale - (player.interpolationY * specs.lunit * specs.scale) + window.getSize().y / 2 + cameraOffsetY);
                if (x < player.interpolationX + specs.renderDistance - cameraOffsetX / specs.lunit / specs.scale && x > player.interpolationX - specs.renderDistance - cameraOffsetX / specs.lunit / specs.scale
                    && y < player.interpolationY + specs.renderDistance / 1.7 - cameraOffsetY / specs.lunit / specs.scale && y > player.interpolationY - specs.renderDistance / 1.7 - cameraOffsetY / specs.lunit / specs.scale)
                    window.draw(map.tiles[x + y * map.width].sprite);
            }
        }
    }
}

// Renders the player
void Game::renderPlayer() {
    player.sprite.setPosition(window.getSize().x / 2 + cameraOffsetX, window.getSize().y / 2 + cameraOffsetY);
    window.draw(player.sprite);
}