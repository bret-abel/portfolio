#include "GameObject.h"

/*
	Set up necessary info for a GameObject's sprite and animations.
*/
void GameObject::setupSprite(TileName subtexture, TileSheet tilesheet, sf::Vector2f pos)
{
	this->pos = pos;
	idleFrames = tilesheet.getRects(subtexture);
	currentFrame = idleFrames[0];
	texture = tilesheet.getTilesetTexture();
	sprite.setTexture(texture);
	sprite.setTextureRect(currentFrame);
	sprite.setPosition(pos);
}

/*
	Scale a GameObject's sprite without allowing warpage of the image.
*/
void GameObject::scaleSprite(float scaleFactor)
{
	sprite.scale(scaleFactor, scaleFactor);
}