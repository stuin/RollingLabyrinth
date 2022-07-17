#pragma once

#include "Skyrmion/GridMaker.h"

struct TextureSet {
	sf::Texture playerTexture;
	sf::Texture enemyTexture;
	sf::Texture bulletTexture;
	sf::Texture diceTexture;
	sf::Texture tilesTexture;
	sf::Texture borderTexture;
	sf::Texture winTexture;
	sf::Texture loseTexture;
	sf::Texture titleTexture;
	sf::Texture startTexture;
	sf::Texture quitTexture;
};

enum CollisionType {
	WALL,
	EMPTY,
	FLOOR,
	EDGE,
	EXIT
};

enum CollisionLayer {
	DIETOP,
	COLLECTABLE,
	ENEMY,
	BULLET,
	PLAYER,
	BORDER,
	HOLDING,
	SHADING,
	INPUT,
	TITLE
};

static const std::map<char, int> displayIndex = {
	{' ', -1},
	{'#',0},
	{'&',0},
	{'.', 1},
	{'-', 2},
	{'s', 3},
	{'a', 3},
	{'b', 3},
	{'e', 4}
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
	{'e', EXIT}
};