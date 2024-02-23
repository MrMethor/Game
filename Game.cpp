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
    map.loadMap("res/overworld.map");
    player.setPosition(map.getWidth() / 2, map.getHeight() / 2);
}

// Creates and sets up the window
void Game::createWindow() {
    specs.defaultFont.loadFromFile("res/arial.ttf");
    if (specs.fullscreen) {
        window.create(sf::VideoMode(specs.fullscreenWidth, specs.fullscreenHeight, 32), "", sf::Style::Fullscreen);
        view.setSize(sf::Vector2f(specs.fullscreenWidth, specs.fullscreenHeight));
        view.zoom(0.02);
    }
    else {
        window.create(sf::VideoMode(specs.width, specs.height, 32), "Game", sf::Style::Titlebar | sf::Style::Close);
        view.setSize(sf::Vector2f(specs.width, specs.height));
        view.zoom(0.02);
    }
    window.setActive();
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(false);
    sf::Image icon;
    icon.loadFromFile("res/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    view.zoom(6);
}

void Game::update() {
    // Adds keys to the input buffer
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
        zoom(-specs.zoomSpeed);
        inputBufferWheel--;
    }
    while (inputBufferWheel < 0) {
        zoom(specs.zoomSpeed);
        inputBufferWheel++;
    }

    player.updateVelocity(movingX, movingY);
}

// Zooms the camera in
void Game::zoom(double speed) {
    view.zoom(1 + speed);
}

// Rander Method
void Game::render(double interpolationModifier, int fps) {

    window.clear();

    // Calculates the interpolation between frames        
    double interpolationX = player.getPreviousX() + (player.getX() - player.getPreviousX()) * interpolationModifier;
    double interpolationY = player.getPreviousY() + (player.getY() - player.getPreviousY()) * interpolationModifier;

    // Renders the map
    if (map.isDefined()) {
        for (int y = 0; y < map.getHeight(); y++) {
            for (int x = 0; x < map.getWidth(); x++) {
                map.tiles[x + y * map.getWidth()].sprite.setPosition((double)x * spritesheet.getTileSize(), (double)y * spritesheet.getTileSize());
                window.draw(map.tiles[x + y * map.getWidth()].sprite);
            }
        }
    }

    // Renders the player
    player.sprite.setPosition(interpolationX * spritesheet.getTileSize(), interpolationY * spritesheet.getTileSize());
    window.draw(player.sprite);

    // Renders the view
    view.setCenter(interpolationX * spritesheet.getTileSize(), interpolationY * spritesheet.getTileSize());
    window.setView(view);

    window.setView(window.getDefaultView());
    sf::Text textFPS;
    textFPS.setFont(specs.defaultFont);
    textFPS.setCharacterSize(12);
    textFPS.setFillColor(sf::Color::Black);
    textFPS.setString(std::to_string(fps) + " fps");
    window.draw(textFPS);

    window.setView(view);

    window.display();
}

double Game::getDesiredUPS() const {
    return specs.desiredUPS;
}