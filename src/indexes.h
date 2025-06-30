#pragma once

#include "Skyrmion/tiling/GridMaker.h"
#include "Skyrmion/input/MovementEnums.h"

enum Textures {
	invalidTexture,
	playerTexture,
	enemyTexture,
	bulletTexture,
	diceTexture,
	tilesTexture,
	borderTexture,
	winTexture,
	loseTexture,
	titleTexture,
	startTexture,
	quitTexture,
	transparencyTexture
};
static std::vector<std::string> TEXTURE_FILES = {
	"#INVALID",
	"res/player.png",
	"res/enemy.png",
	"res/bullet.png",
	"res/dice.png",
	"res/dicetiles.png",
	"res/border.png",
	"res/winscreen.png",
	"res/losescreen.png",
	"res/titlescreen.png",
	"res/startbutton.png",
	"res/quitbutton.png",
	"res/debug/heatmapT.png"
};

enum Messages {
	CLEAR_ENTITIES,
	RESET_MAP,
	SHOW_WIN,
	SHOW_LOST,
	CONTROLS_JOYSTICK,
	CONTROLS_MOUSE,
	CONTROLS_KEYBOARD
};

enum CollisionType {
	EMPTY2 = -1,
	WALL = 0,
	FLOOR = 1,
	EDGE = 2,
	EXIT = 3
};

enum CollisionLayer {
	DIETOP,
	COLLECTABLE,
	ENEMY,
	BULLET,
	PLAYER,
	HOLDING,
	BORDER,
	MENU,
	INPUT,
	TITLE
};
static std::vector<std::string> LAYER_NAMES = {
	"DIETOP", "COLLECTABLE", "ENEMY", "BULLET", "PLAYER", "HOLDING", "BORDER", "MENU", "INPUT", "TITLE"
};

static const std::map<int, int> displayIndex = {
	{' ', -1},
	{'/', -1},
	{'#',0},
	{'&',0},
	{'.', 1},
	{'-', 2},
	{'s', 3},
	{'a', 3},
	{'b', 3},
	{'e', 4}
};

static const std::map<int, int> collisionIndex1 = {
	{' ', EMPTY2},
	{'#', WALL},
	{'&', WALL},
	{'.', FLOOR},
	{'-', EDGE},
	{'s', FLOOR},
	{'a', FLOOR},
	{'b', FLOOR},
	{'e', EXIT}
};

static const std::map<int, int> collisionIndex2 = {
	{' ', FULL},
	{'#', FULL},
	{'&', FULL},
	{'.', EMPTY},
	{'-', EMPTY},
	{'s', EMPTY},
	{'a', EMPTY},
	{'b', EMPTY},
	{'e', EMPTY}
};