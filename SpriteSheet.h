#include <SFML/Graphics.hpp>

#pragma once

class SpriteSheet {
public:
	sf::Texture texture;
	SpriteSheet(std::string path);
};