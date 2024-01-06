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