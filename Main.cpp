#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Entity.h"
#include "Directions.h"

void update();
void render();
void zoom(bool in);

Game game;

int main() {

    sf::Clock clock;
    sf::Clock clockSeconds;
    int desiredUPS = 60;
    long delta = 1000000 / desiredUPS;
    long remainder = 0;
    int fps = 0;
    int ups = 0;

    while (game.running) {
        if (clock.getElapsedTime().asMicroseconds() + remainder >= delta) {
            update();
            remainder = clock.getElapsedTime().asMicroseconds() - delta;
            ups++;
            clock.restart();
        }
        if (clockSeconds.getElapsedTime().asMicroseconds() >= 1000000) {
            std::string title = "Game | " + std::to_string(fps) + "FPS " + std::to_string(ups) + "UPS";
            game.window.setTitle(title);
            fps = 0;
            ups = 0;
            clockSeconds.restart();
        }
        render();
        fps++;
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
            for (int i = 0; i < sizeof(game.inputBufferWheel) / sizeof(game.inputBufferWheel[0]); i++) {
                if (game.inputBufferWheel[i] == -1) {
                    game.inputBufferWheel[i] = event.key.code;
                    return;
                }
            }
            break;
        case sf::Event::Closed:
            game.window.close();
            game.running = false;
        }
    }
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
        case sf::Keyboard::P:
            zoom(true);
            break;
        case sf::Keyboard::O:
            zoom(false);
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
    for (int i = 0; i < sizeof(game.inputBufferMouse) / sizeof(game.inputBufferMouse[0]); i++) {
        switch (game.inputBufferMouse[i]) {
        }
    }
    for (int i = 0; i < sizeof(game.inputBufferWheel) / sizeof(game.inputBufferWheel[0]); i++) {
        std::cout << game.inputBufferWheel[i] << "\n";
        switch (game.inputBufferWheel[i]) {
        case sf::Mouse::HorizontalWheel:
            zoom(true);
            game.inputBufferWheel[i] = -1;
            break;
        case sf::Mouse::VerticalWheel:
            zoom(false);
            game.inputBufferWheel[i] = -1;
            break;
        }
    }
    


    game.player.update();
}

void render() {
    game.window.clear();

    if (game.map.defined) {
        for (int y = 0; y < game.map.height; y++) {
            for (int x = 0; x < game.map.width; x++) {
                game.map.tiles[x + y * game.map.width].sprite.setPosition((float)x * game.specs.lunit * game.specs.scale - (game.player.x * game.specs.lunit * game.specs.scale) + game.window.getSize().x / 2,
                                                                         (float)y * game.specs.lunit * game.specs.scale  - (game.player.y * game.specs.lunit * game.specs.scale) + game.window.getSize().y / 2);
                game.window.draw(game.map.tiles[x + y * game.map.width].sprite);
            }
        }
    }

    game.player.sprite.setPosition(game.window.getSize().x / 2 /*+ game.cameraOffsetX*/, game.window.getSize().y / 2 /*+ game.cameraOffsetY*/);
    game.window.draw(game.player.sprite);

    game.window.display();
}

void zoom(bool in) {
    if (in) {
        game.specs.scale += 0.1;
    }
    else {
        game.specs.scale -= 0.1;
    }
    game.player.sprite.setScale(game.specs.scale, game.specs.scale);
    if (game.map.defined) {
        for (int y = 0; y < game.map.height; y++) {
            for (int x = 0; x < game.map.width; x++) {
                game.map.tiles[x + y * game.map.width].sprite.setScale(game.specs.scale, game.specs.scale);
            }
        }
    }
}
