#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Player.h"
#include "Directions.h"

void update(Game &game);
void render(Game &game);

int main() {
    
    Game game;

    sf::Clock clock;
    sf::Clock clockSeconds;
    long delta = 1000000 / 60;
    int fps = 0;
    int ups = 0;

    while (game.running) {
        if (clock.getElapsedTime().asMicroseconds() >= delta) {
            update(game);
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
        render(game);
        fps++;
    }

    return 0;
}

void update(Game &game) {
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
        }
    }
}

void render(Game &game) {
    game.window.clear();
    game.player.sprite.setPosition(game.player.x, game.player.y);
    game.window.draw(game.player.sprite);
    game.window.display();
}

