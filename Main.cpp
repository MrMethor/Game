#include <SFML/Graphics.hpp>
#include "Game.h"
#include <chrono>

void update();
void render();

Game game;
int fps = 0;
int ups = 0;
double interpolation = 0;
double interpolationModifier = 0;

int main() {
    
    sf::Clock clock;
    const int SECOND = 1000000000;
    int delta = SECOND / game.getDesiredUPS();
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
        if (secondsCounter >= SECOND) {
            ups = upsCounter;
            fps = fpsCounter;
            std::string title = "Game | " + std::to_string(fps) + "FPS " + std::to_string(ups) + "UPS";
            game.window.setTitle(title);
            upsCounter = 0;
            fpsCounter = 0;
            secondsCounter = 0;
        }

        auto finish = std::chrono::high_resolution_clock::now();

        long elapsedTime = (long)std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
        updateCounter += elapsedTime;
        secondsCounter += elapsedTime;

        interpolation = (double)elapsedTime / delta;
        interpolationModifier += interpolation;
        if (interpolationModifier >= 1)
            interpolationModifier -= 1;
    }
}

// Update function that reads input and updates the game
void update() {
    game.update();
}

void render() {
    game.render(interpolationModifier, fps);
}

