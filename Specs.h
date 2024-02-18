#pragma once
// Contains all the specs for the game
class Specs {
public:
	sf::Font defaultFont;
	bool fullscreen = false;
	double scale = 6;
	double spriteScale = 0;
	int width = 1000;
	int height = 650;
	int fullscreenWidth = 1920;
	int fullscreenHeight = 1080;
	char desiredUPS = 60;
	int renderDistance = 13;
};