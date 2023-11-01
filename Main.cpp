#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Entity.h"
#include "Directions.h"
#include <chrono>

void update();
void render();
void zoom(bool in);

Game game;
int fps = 0;
int ups = 0;
double gamma = 0;

int main() {

    sf::Clock clock;
    int second = 1000000000;
    char desiredUPS = 60;
    int delta = second / desiredUPS;
    int updateCounter = 0;
    int secondsCounter = 0;
    int upsCounter = 0;
    int fpsCounter = 0;
    

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
        gamma = (double)elapsedTime / delta;
        updateCounter += elapsedTime;
        secondsCounter += elapsedTime;
    }

    return 0;
}

void update() {
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

    // Keyboard
    for (int i = 0; i < sizeof(game.inputBuffer) / sizeof(game.inputBuffer[0]); i++) {
        switch (game.inputBuffer[i]) {
        case sf::Keyboard::Left:
        case sf::Keyboard::A:
            game.player.move(Left);
            break;
        case sf::Keyboard::Right:
        case sf::Keyboard::D:
            game.player.move(Right);
            break;
        case sf::Keyboard::Down:
        case sf::Keyboard::S:
            game.player.move(Down);
            break;
        case sf::Keyboard::Up:
        case sf::Keyboard::W:
            game.player.move(Up);
            break;
        case sf::Keyboard::F11:
            game.specs.fullscreen = !game.specs.fullscreen;
            game.createWindow();
            for (int i = 0; i < sizeof(game.inputBuffer) / sizeof(game.inputBuffer[0]); i++) {
                game.inputBuffer[i] = -1;
            }
            break;
        case sf::Keyboard::Escape:
            game.window.close();
            game.running = false;
        }
    }

    // Mouse
    for (int i = 0; i < sizeof(game.inputBufferMouse) / sizeof(game.inputBufferMouse[0]); i++) {
        switch (game.inputBufferMouse[i]) {
        }
    }

    // Mouse Wheel
    while (game.inputBufferWheel > 0) {
        zoom(true);
        game.inputBufferWheel--;
    }
    while (game.inputBufferWheel < 0) {
        zoom(false);
        game.inputBufferWheel++;
    }
}

void render() {

    game.player.updateVelocity(gamma);

    game.window.clear();

    double cameraOffsetX = 0;
    double cameraOffsetY = 0;
    double left = -((0 * game.specs.lunit * game.specs.scale - (game.player.x * game.specs.lunit * game.specs.scale) + game.window.getSize().x / 2) - (game.specs.lunit / 2 * game.specs.scale));
    double right = (game.map.width * game.specs.lunit * game.specs.scale - (game.player.x * game.specs.lunit * game.specs.scale) + game.window.getSize().x / 2) - (game.specs.lunit / 2 * game.specs.scale) - game.window.getSize().x;
    double up = -((0 * game.specs.lunit * game.specs.scale - (game.player.y * game.specs.lunit * game.specs.scale) + game.window.getSize().y / 2) - (game.specs.lunit / 2 * game.specs.scale));
    double down = (game.map.height * game.specs.lunit * game.specs.scale - (game.player.y * game.specs.lunit * game.specs.scale) + game.window.getSize().y / 2) - (game.specs.lunit / 2 * game.specs.scale) - game.window.getSize().y;

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

    int renderDistance = 13;

    if (game.map.defined) {
        for (int y = 0; y < game.map.height; y++) {
            for (int x = 0; x < game.map.width; x++) {
                if(x < game.player.x + 16){}
                game.map.tiles[x + y * game.map.width].sprite.setPosition((double)x * game.specs.lunit * game.specs.scale - (game.player.x * game.specs.lunit * game.specs.scale) + game.window.getSize().x / 2 + cameraOffsetX,
                                                                         (double)y * game.specs.lunit * game.specs.scale  - (game.player.y * game.specs.lunit * game.specs.scale) + game.window.getSize().y / 2 + cameraOffsetY);
                if (x < game.player.x + renderDistance - cameraOffsetX / game.specs.lunit / game.specs.scale && x > game.player.x - renderDistance - cameraOffsetX / game.specs.lunit / game.specs.scale
                 && y < game.player.y + renderDistance / 1.7 - cameraOffsetY / game.specs.lunit / game.specs.scale && y > game.player.y - renderDistance / 1.7 - cameraOffsetY / game.specs.lunit / game.specs.scale)
                    game.window.draw(game.map.tiles[x + y * game.map.width].sprite);
            }
        }
    }

    game.player.sprite.setPosition(game.window.getSize().x / 2 + cameraOffsetX, game.window.getSize().y / 2 + cameraOffsetY);
    game.window.draw(game.player.sprite);

    sf::Text textFPS;
    textFPS.setFont(game.specs.defaultFont);
    textFPS.setCharacterSize(12);
    textFPS.setFillColor(sf::Color::Black);
    textFPS.setString(std::to_string(fps) + " fps");
    game.window.draw(textFPS);

    game.window.display();

}

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