#pragma once
// Contains all the specs for the game
class Specs {
public:
	sf::Font defaultFont;
	bool fullscreen = false;
	double scale = 6;
	int width = 1000;
	int height = 650;
	int lunit = 16;
	char desiredUPS = 60;
	int renderDistance = 13;
};