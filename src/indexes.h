#pragma once

#include "Skyrmion/GridMaker.h"

struct TextureSet {
	sf::Texture playerTexture;
	sf::Texture diceTexture;
	sf::Texture tilesTexture;
	sf::Texture borderTexture;
	sf::Texture endTexture;
};

enum CollisionType {
	WALL,
	EMPTY,
	FLOOR,
	EDGE,
	EXIT
};

enum CollisionLayer {
	TABLE,
	DIESIDES,
	DIETOP,
	COLLECTABLE,
	PLAYER,
	LIGHT,
	BORDER,
	HOLDING,
	INPUT,
	TITLE
};

static const std::map<char, int> displayIndex = {
	{' ', -1},
	{'#',0},
	{'.', 1},
	{'-', 2},
	{'s', 2},
	{'a', 3},
	{'b', 3},
	{'e', 3}
};

static const std::map<char, int> collisionIndex = {
	{' ', EMPTY},
	{'#', WALL},
	{'.', FLOOR},
	{'-', EDGE},
	{'s', FLOOR},
	{'a', FLOOR},
	{'b', FLOOR},
	{'e', FLOOR},
};

static const std::map<char, int> lightIndex = {
	{'#', -100},
	{' ', -100},
	{'@', -100},
	{'T', -80},
	{'t', 0},
	{'H', 0},
	{'h', 0},
	{',', 0},
	{'.', 0},
	{']', 0},
	{'[', 0},
	{'=', 0},
	{'_', 0},
	{'-', 0},
	{'\\',0},
	{'+', 0},
	{'|', 0},
	{'!', 100}
};