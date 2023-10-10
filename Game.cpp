#include "Game.h"

// Constructor creates the Window and declares the Input Buffer
Game::Game() {
    window.create(sf::VideoMode(800, 600), "");
    window.setActive();
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(false);
    sf::Image icon;
    icon.loadFromFile("resources/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    for (int i = 0; i < inputBufferSize; i++)
        inputBuffer[i] = -1;
}