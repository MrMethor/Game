#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Entity.h"
#include "Directions.h"
#include <chrono>
#include "Actions.h"
#include <corecrt_math_defines.h>

void update();
void render();
void zoom(bool in);

Game game;
int fps = 0;
int ups = 0;
double interpolation = 0;
double interpolationSum = 0;

int main() {

    sf::Clock clock;
    int second = 1000000000;
    // The amount the game should update per second
    char desiredUPS = 60;
    int delta = second / desiredUPS;
    int updateCounter = 0;
    int secondsCounter = 0;
    int upsCounter = 0;
    int fpsCounter = 0;

    // Main game loop that runs until the window is closed, it calls the update and render functions and calculates FPS and UPS
    while (game.running) {

        auto start = std::chrono::high_resolution_clock::now();

        if (updateCounter >= delta) {
            update();
            updateCounter -= delta;
            upsCounter++;
        }
        render();
        fpsCounter++;
        if (secondsCounter >= second) {
            ups = upsCounter;
            fps = fpsCounter;
            std::string title = "Game | " + std::to_string(fps) + "FPS " + std::to_string(ups) + "UPS";
            game.window.setTitle(title);
            upsCounter = 0;
            fpsCounter = 0;
            secondsCounter = 0;
        }

        auto finish = std::chrono::high_resolution_clock::now();

        long elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
        interpolation = (double)elapsedTime / delta;
        updateCounter += elapsedTime;
        secondsCounter += elapsedTime;
    }
}

// Update function that reads input and updates the game
void update() {

    // Adds keys to the input buffer
    sf::Event event;
    while (game.window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::KeyPressed:
            for (int i = 0; i < sizeof(game.inputBuffer) / sizeof(game.inputBuffer[0]); i++) {
                if (game.inputBuffer[i] == -1) {
                    game.inputBuffer[i] = event.key.code;
                    break;
                }
            }
            break;
        case sf::Event::KeyReleased:
            for (int i = 0; i < sizeof(game.inputBuffer) / sizeof(game.inputBuffer[0]); i++) {
                if (game.inputBuffer[i] == event.key.code)
                    game.inputBuffer[i] = -1;
            }
            break;
        case sf::Event::MouseButtonPressed:
            for (int i = 0; i < sizeof(game.inputBufferMouse) / sizeof(game.inputBufferMouse[0]); i++) {
                if (game.inputBufferMouse[i] == -1) {
                    game.inputBufferMouse[i] = event.key.code;
                    break;
                }
            }
            break;
        case sf::Event::MouseButtonReleased:
            for (int i = 0; i < sizeof(game.inputBufferMouse) / sizeof(game.inputBufferMouse[0]); i++) {
                if (game.inputBufferMouse[i] == event.key.code)
                    game.inputBufferMouse[i] = -1;
            }
            break;
        case sf::Event::MouseWheelMoved:
            game.inputBufferWheel += event.mouseWheel.delta;
            break;
        case sf::Event::Closed:
            game.window.close();
            game.running = false;
        }
    }

    // Compares the input buffer to the keymap and applies the actions based on the keymap
    char movingX = 0;
    char movingY = 0;
    for (int i = 0; i < sizeof(game.inputBuffer) / sizeof(game.inputBuffer[0]); i++) {
        if (game.inputBuffer[i] != -1) {
            switch (game.controls.keymap[game.inputBuffer[i]]) {
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
                game.specs.fullscreen = !game.specs.fullscreen;
                game.createWindow();
                for (int i = 0; i < sizeof(game.inputBuffer) / sizeof(game.inputBuffer[0]); i++)
                    game.inputBuffer[i] = -1;
                break;
            case openMenu:
                game.window.close();
                game.running = false;
            }
        }
    }
    if (movingX == 0 && movingY == 0)
        game.player.degree = -1;
    else {
        game.player.degree = atan2(movingX, movingY) * (180.0 / M_PI);
        game.player.degree = game.player.degree < 0 ? game.player.degree + 360 : game.player.degree;
    }

    // MOUSE WORK IN PROGRESS
    for (int i = 0; i < sizeof(game.inputBufferMouse) / sizeof(game.inputBufferMouse[0]); i++) {
        switch (game.inputBufferMouse[i]) {
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
    while (game.inputBufferWheel > 0) {
        zoom(true);
        game.inputBufferWheel--;
    }
    while (game.inputBufferWheel < 0) {
        zoom(false);
        game.inputBufferWheel++;
    }

    game.player.updateVelocity();
}

void render() {

    game.window.clear();
    // Calculates the interpolation between frames
    if (interpolationSum >= 1)
        interpolationSum -= 1;
    interpolationSum += interpolation;
        
    double interpolationX = game.player.previousX + (game.player.x - game.player.previousX) * interpolationSum;
    double interpolationY = game.player.previousY + (game.player.y - game.player.previousY) * interpolationSum;

    // Calculates the camera offset based on the player's position
    double cameraOffsetX = 0;
    double cameraOffsetY = 0;
    double left = -((0 * game.specs.lunit * game.specs.scale - (interpolationX * game.specs.lunit * game.specs.scale) + game.window.getSize().x / 2) - (game.specs.lunit / 2 * game.specs.scale));
    double right = (game.map.width * game.specs.lunit * game.specs.scale - (interpolationX * game.specs.lunit * game.specs.scale) + game.window.getSize().x / 2) - (game.specs.lunit / 2 * game.specs.scale) - game.window.getSize().x;
    double up = -((0 * game.specs.lunit * game.specs.scale - (interpolationY * game.specs.lunit * game.specs.scale) + game.window.getSize().y / 2) - (game.specs.lunit / 2 * game.specs.scale));
    double down = (game.map.height * game.specs.lunit * game.specs.scale - (interpolationY * game.specs.lunit * game.specs.scale) + game.window.getSize().y / 2) - (game.specs.lunit / 2 * game.specs.scale) - game.window.getSize().y;

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

    // Indicates how many tiles should be rendered around the player
    int renderDistance = 13;

    // Renders the map tiles
    if (game.map.defined) {
        for (int y = 0; y < game.map.height; y++) {
            for (int x = 0; x < game.map.width; x++) {
                if(x < interpolationX + 16){}
                game.map.tiles[x + y * game.map.width].sprite.setPosition((double)x * game.specs.lunit * game.specs.scale - (interpolationX * game.specs.lunit * game.specs.scale) + game.window.getSize().x / 2 + cameraOffsetX,
                                                                         (double)y * game.specs.lunit * game.specs.scale  - (interpolationY * game.specs.lunit * game.specs.scale) + game.window.getSize().y / 2 + cameraOffsetY);
                if (x < interpolationX + renderDistance - cameraOffsetX / game.specs.lunit / game.specs.scale && x > interpolationX - renderDistance - cameraOffsetX / game.specs.lunit / game.specs.scale
                 && y < interpolationY + renderDistance / 1.7 - cameraOffsetY / game.specs.lunit / game.specs.scale && y > interpolationY - renderDistance / 1.7 - cameraOffsetY / game.specs.lunit / game.specs.scale)
                    game.window.draw(game.map.tiles[x + y * game.map.width].sprite);
            }
        }
    }

    // Renders the player
    game.player.sprite.setPosition(game.window.getSize().x / 2 + cameraOffsetX, game.window.getSize().y / 2 + cameraOffsetY);
    game.window.draw(game.player.sprite);

    // Miscellaneous
    sf::Text textFPS;
    textFPS.setFont(game.specs.defaultFont);
    textFPS.setCharacterSize(12);
    textFPS.setFillColor(sf::Color::Black);
    textFPS.setString(std::to_string(fps) + " fps");
    game.window.draw(textFPS);

    // Displays the window
    game.window.display();
}

// Zooms the camera in or out
void zoom(bool in) {
    double zoomSpeed = 0.2;
    int maxScale = 10;
    int minScale = 5;
    if (in && game.specs.scale < maxScale)
        game.specs.scale += zoomSpeed;
    else if (!in && game.specs.scale > minScale)
        game.specs.scale -= zoomSpeed;
    else
        return;

    game.player.sprite.setScale(game.specs.scale, game.specs.scale);
    if (game.map.defined) {
        for (int y = 0; y < game.map.height; y++) {
            for (int x = 0; x < game.map.width; x++) {
                game.map.tiles[x + y * game.map.width].sprite.setScale(game.specs.scale, game.specs.scale);
            }
        }
    }
}