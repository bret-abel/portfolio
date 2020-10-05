#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

#ifndef GAMETILE_H
#define GAMETILE_H

#define TILESIZE 16

class GameTile : public GameObject
{
public:
	bool isPassable;
	bool isSpawn;
	GameTile(TileName subtexture, TileSheet tilesheet, sf::Vector2f pos);
	GameTile() = default;
};

#endif // !GAMETILE_H
