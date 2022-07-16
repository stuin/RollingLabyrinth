//SFML headers
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Skyrmion/LightMap.h"
#include "indexes.h"
#include "Player.hpp"

int main() {
	//Load node textures
	sf::Texture playerTexture;
	sf::Texture treasureTexture;
	sf::Texture tilesTexture;
	sf::Texture borderTexture;
	UpdateList::loadTexture(&playerTexture, "res/upperplayer.png");
	UpdateList::loadTexture(&treasureTexture, "res/treasure.png");
	UpdateList::loadTexture(&tilesTexture, "res/dicetiles.png");
	UpdateList::loadTexture(&borderTexture, "res/border.png");

	//Setup Light maps
	/*Indexer lightMap(&grid, lightIndex, 0, 2, 2);
	LightMap staticLights(8, 8, 0, 0.1, lightMap, LIGHT, true);
	lighting.addLightMap(&staticLights);
	UpdateList::addNode(&staticLights);*/

	//Upper area player
	Player player(&tilesTexture, &borderTexture);
	player.setPosition(sf::Vector2f(2960, 2960));
	player.setTexture(playerTexture);
	player.setScale(6, 6);
	UpdateList::addNode(&player);

	//Place Treasure chests
	/*collisionMap.mapGrid([&treasureTexture](char c, sf::Vector2f pos) {
		if(c == 'H' || c == 'h') {
			Node *t = new Node(TREASURE, sf::Vector2i(10, 9));
			t->setTexture(treasureTexture);
			t->setPosition(pos + sf::Vector2f(8, 8));
			UpdateList::addNode(t);
		}
	});*/

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