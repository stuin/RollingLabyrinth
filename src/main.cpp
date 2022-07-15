//SFML headers
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Skyrmion/LightMap.h"
#include "indexes.h"
#include "Player.hpp"
#include "Holder.hpp"

int main() {
	//Load node textures
	sf::Texture playerTexture;
	sf::Texture treasureTexture;
	sf::Texture tilesTexture;
	sf::Texture borderTexture;
	UpdateList::loadTexture(&playerTexture, "res/upperplayer.png");
	UpdateList::loadTexture(&treasureTexture, "res/treasure.png");
	UpdateList::loadTexture(&tilesTexture, "res/foresttiles.png");
	UpdateList::loadTexture(&borderTexture, "res/border.png");

	//Load base tile maps
	GridMaker grid(112*4, 112*4);
	TileMap dungeon(&tilesTexture, 16, 16, Indexer(&grid, displayIndex, 0), DIETOP);
	Indexer collisionMap(&grid, collisionIndex, WALL, 16, 16);
	UpdateList::addNode(&dungeon);

	//Setup Light maps
	/*Indexer lightMap(&grid, lightIndex, 0, 2, 2);
	LightMap staticLights(8, 8, 0, 0.1, lightMap, LIGHT, true);
	lighting.addLightMap(&staticLights);
	UpdateList::addNode(&staticLights);*/

	//Upper area player
	Player player(collisionMap);
	player.setPosition(sf::Vector2f(100, 100));
	player.setTexture(playerTexture);
	player.setScale(7, 7);
	UpdateList::addNode(&player);

	//Dice Holder
	Holder holder(&tilesTexture);
	holder.setParent(&player);
	holder.setPosition(sf::Vector2f(0, 300));
	holder.setTexture(borderTexture);
	UpdateList::addNode(&holder);

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

	//Finish engine setup
	UpdateList::setCamera(&player, sf::Vector2f(1920, 1080));
	UpdateList::startEngine("The Path Below", TITLE);

	return 0;
}