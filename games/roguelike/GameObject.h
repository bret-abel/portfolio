#pragma once
#include <SFML/Graphics.hpp>
#include "TileSheet.h"

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject
{
public:
	sf::Vector2f pos;
	sf::Texture texture;
	sf::Sprite sprite;
	std::vector<sf::IntRect> idleFrames;
	sf::IntRect	currentFrame;
	void setupSprite(TileName subtexture, TileSheet tilesheet, sf::Vector2f pos);
	void scaleSprite(float);
};



#endif // !GAMEOBJECT_H
