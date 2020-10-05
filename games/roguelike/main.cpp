#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "TileSheet.h"
#include "GameTile.h"
using namespace sf;
int main()
{
	unsigned int windowHeight = 512;
	unsigned int windowWidth = 512;

	// prepare game window
	RenderWindow window(VideoMode(windowWidth, windowHeight), "Roguelike");
	View zoomedIn = window.getView();

	// blow everything up to 32x32 for pixelated aesthetic
	zoomedIn.reset(sf::FloatRect(0, 0, windowWidth/2, windowHeight/2));
	window.setView(zoomedIn);
	
	// load tilesheet from image file and list of coordinates
	TileSheet tilesheet;
	if (!tilesheet.load("assets/dungeon-tileset/tileset.png", "assets/dungeon-tileset/tiles_list"))
	{
		return EXIT_FAILURE;
	}

	// game map
	const int level[] =
	{
		35,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 36,
		37,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		37, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 38,
		39,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4, 40
	};

	TileMap map = TileMap();
	map.load(tilesheet, level, 16, 16);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		// draw here
		window.draw(map);
		window.display();
	}
}