#include "GameTile.h"

GameTile::GameTile(TileName subtexture, TileSheet tilesheet, sf::Vector2f pos)
{
	if (subtexture >= TileName::floor_1 && subtexture <= TileName::floor_8)
	{
		this->isPassable = true;
	}
	else
	{
		this->isPassable = false;
	}
	setupSprite(subtexture, tilesheet, pos);
}