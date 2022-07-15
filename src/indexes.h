#pragma once

#include "Skyrmion/GridMaker.h"

enum CollisionType {
	WALL,
	PROP,
	FLOOR,
	EDGE,
	EXIT
};

enum CollisionLayer {
	TABLE,
	DIESIDES,
	DIETOP,
	TREASURE,
	PLAYER,
	LIGHT,
	BORDER,
	HOLDING,
	PLACINGTOP,
	PLACINGSIDES,
	INPUT,
	TITLE
};

static const std::map<char, int> displayIndex = {
	{' ', -1},
	{'\\',0},
	{'a', 1},
	{'-', 1},
	{'+', 2},
	{'#', 3},
	{',', 4},
	{'H', 4},
	{'.', 5},
	{'h', 5},
	{'=', 5},
	{'[', 5},
	{'|', 6},
	{']', 6},
	{'@', 7},
	{'!', 8},
	{'T', 9},
	{'t', 10}
};

static const std::map<char, int> collisionIndex = {
	{'#', WALL},
	{' ', EDGE}
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