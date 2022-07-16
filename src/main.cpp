//SFML headers
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Skyrmion/LightMap.h"
#include "indexes.h"
#include "Player.hpp"

int main() {
	srand(time(NULL));

	//Load node textures
	TextureSet textures;
	UpdateList::loadTexture(&textures.playerTexture, "res/player.png");
	UpdateList::loadTexture(&textures.enemyTexture, "res/enemy.png");
	UpdateList::loadTexture(&textures.bulletTexture, "res/bullet.png");
	UpdateList::loadTexture(&textures.diceTexture, "res/dice.png");
	UpdateList::loadTexture(&textures.tilesTexture, "res/dicetiles.png");
	UpdateList::loadTexture(&textures.borderTexture, "res/border.png");
	UpdateList::loadTexture(&textures.endTexture, "res/endscreen.png");

	//Setup Light maps
	/*Indexer lightMap(&grid, lightIndex, 0, 2, 2);
	LightMap staticLights(8, 8, 0, 0.1, lightMap, LIGHT, true);
	lighting.addLightMap(&staticLights);
	UpdateList::addNode(&staticLights);*/

	//Setup player
	Player player(&textures);
	player.setPosition(sf::Vector2f(2960, 2960));
	player.setTexture(textures.playerTexture);
	player.setScale(6, 6);
	UpdateList::addNode(&player);

	UpdateList::alwaysLoadLayer(TABLE);
	UpdateList::alwaysLoadLayer(DIETOP);
	UpdateList::alwaysLoadLayer(BORDER);
	UpdateList::alwaysLoadLayer(HOLDING);
	UpdateList::alwaysLoadLayer(PLAYER);
	UpdateList::alwaysLoadLayer(INPUT);

	//Finish engine setup
	UpdateList::setCamera(&player, sf::Vector2f(1920, 1080));
	UpdateList::startEngine("The Path Below", TITLE);

	return 0;
}