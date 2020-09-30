#include <SFML/Graphics.hpp>
#include "TileSheet.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>


using std::string;
using std::vector;

/**
 * Loads tileset image and text file containing tile coordinates and dimensions.
 *
 * @param img Path to the image file containing tileset.
 * @param tileList Text file containing list of tile coordinates of form "tile_name x y width height {frame count}"
 *
 * @return true upon success or false if either file fails to load
 */
bool TileSheet::load(const string& img, const string& tileList)
{
	// load tilesheet texture
	if (!tileset.loadFromFile(img))
	{
		std::cout << "failed to load: " << img << std::endl;
		return false;
	}

	// load tile names, coordinates, and number of animation frames (if applicable)
	string line;
	vector<string> lineTokens;
	string token;
	int x, y, w, h;
	int numFrames;
	string key;
	vector<sf::IntRect> values;
	
	// read tile name, coordinates, number of
	// animation frames (if applicable) from text file
	std::ifstream tileFile(tileList);
	if (!tileFile.is_open())
	{
		std::cout << "failed to load: " << tileList << std::endl;
		return false;
	}
	// tokenize each line
	while (getline(tileFile, line))
	{
		std::stringstream linestream(line);
		while (getline(linestream, token, ' '))
		{
			lineTokens.push_back(token);
		}

		// sprite isn't animated
		if (lineTokens.size() == 5)
		{
			key = lineTokens[0];
			x = stoi(lineTokens[1]);
			y = stoi(lineTokens[2]);
			w = stoi(lineTokens[3]);
			h = stoi(lineTokens[4]);
			values.push_back(sf::IntRect(x, y, w, h));
			tileRects.emplace(key, values);
		}
		// sprite has animation frames
		else if (lineTokens.size() == 6)
		{
			key = lineTokens[0];
			x = stoi(lineTokens[1]);
			y = stoi(lineTokens[2]);
			w = stoi(lineTokens[3]);
			h = stoi(lineTokens[4]);
			numFrames = stoi(lineTokens[5]);
			for (int frameCount = 0; frameCount < numFrames; frameCount++)
			{
				values.push_back(sf::IntRect(x + w * frameCount, y, w, h));
			}
			tileRects.emplace(key, values);
			
		}
		// clear vectors for next iteration
		lineTokens.clear();
		values.clear();
	}
	return true;
}

/* Returns the coordinates and dimensions on the tilesheet of a given art asset.

   @param name Name of the art asset to get.
   @return vector containing all the rects for a given sprite from the tilesheet
*/
vector<sf::IntRect> TileSheet::getRects(const string& name)
{
	return tileRects[name];
}

sf::Texture TileSheet::getTilesetTexture()
{
	return tileset;
}