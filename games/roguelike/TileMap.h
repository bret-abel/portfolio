#pragma once
#include <SFML/Graphics.hpp>
#include "TIleSheet.h"
#include "GameTile.h"
class TileMap : public sf::Drawable, public sf::Transformable
{public:
	bool load(TileSheet tilesheet, const int* tiles, unsigned int width, unsigned int height);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray vertices;
	sf::Texture tileset;
	GameTile* gameTiles;
};