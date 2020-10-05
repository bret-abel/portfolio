#include "TileMap.h"
#include <stdio.h>
#include <iostream>

bool TileMap::load(TileSheet tilesheet, const int* lvlTiles, unsigned int columns, unsigned int rows)
{

    tileset = tilesheet.getTilesetTexture();

    // initialize array of GameTiles to fit level
    gameTiles = new GameTile[columns * rows];

    // resize the vertex array to fit the level size
    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(columns * rows * 4);

    // fill GameTiles array and VertexArray based on contents of lvlTiles
    for (unsigned int col = 0; col < columns; col++)
    {
        for (unsigned int row = 0; row < rows; row++)
        {
            int currTile = col + row * columns;
            TileName subtexture = static_cast<TileName>(lvlTiles[currTile]);
            IntRect rect = tilesheet.getRects(subtexture)[0];
            sf::Vector2f pos = sf::Vector2f(col * rect.width, row * rect.height);
            gameTiles[currTile] = GameTile(subtexture, tilesheet, pos);

            sf::Vertex* quad = &vertices[currTile * 4];

            // coordinates on the screen
            quad[0].position = pos;
            quad[1].position = sf::Vector2f(pos.x + rect.width, pos.y);
            quad[2].position = sf::Vector2f(pos.x + rect.width, pos.y + rect.height);
            quad[3].position = sf::Vector2f(pos.x, pos.y + rect.height);
         
            // coordinates on the tilesheet
            quad[0].texCoords = sf::Vector2f(rect.left, rect.top);
            quad[1].texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
            quad[2].texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
            quad[3].texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
        }
    }
    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &tileset;

    // draw the vertex array
    target.draw(vertices, states);
}

