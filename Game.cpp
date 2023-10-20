#include "Game.h"

// Constructor creates the Window and declares the Input Buffer
Game::Game() {
    createWindow();
    sf::Image icon;
    icon.loadFromFile("resources/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    for (int i = 0; i < inputBufferSize; i++)
        inputBuffer[i] = -1;
    for (int i = 0; i < inputBufferSize; i++)
        inputBufferMouse[i] = -1;
    for (int i = 0; i < inputBufferSize; i++)
        inputBufferWheel[i] = -1;
    map.loadMap("resources/map");
    player.setPosition(map.width / 2, map.height / 2);
}

void Game::createWindow() {
    if (specs.fullscreen) {
        window.create(sf::VideoMode(), "", sf::Style::Fullscreen);
    }
    else {
        window.create(sf::VideoMode(specs.width, specs.height), "", sf::Style::Titlebar | sf::Style::Close);
    }
    window.setActive();
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(false);
}