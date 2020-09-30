#pragma once
#include <unordered_map>
using std::string;
using sf::IntRect;

class TileSheet
{
public:
	bool load(const string& img, const string& tileList);
	std::vector<IntRect> getRects(const string&);
	sf::Texture getTilesetTexture();
private:
	sf::Texture tileset;
	std::unordered_map<string, std::vector<IntRect>> tileRects;
};