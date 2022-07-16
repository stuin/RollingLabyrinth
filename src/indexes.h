#pragma once

#include "Skyrmion/GridMaker.h"

struct TextureSet {
	sf::Texture playerTexture;
	sf::Texture enemyTexture;
	sf::Texture bulletTexture;
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
	ENEMY,
	BULLET,
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
	{'&',0},
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
	{'&', WALL},
	{'.', FLOOR},
	{'-', EDGE},
	{'s', FLOOR},
	{'a', FLOOR},
	{'b', FLOOR},
	{'e', FLOOR},
};