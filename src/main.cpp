//SFML headers
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "indexes.h"
#include "Player.hpp"

int main() {
	Settings::loadSettings("res/settings.json");
	srand(time(NULL));

	//Load node textures
	TextureSet textures;
	UpdateList::loadTexture(&textures.playerTexture, "res/player.png");
	UpdateList::loadTexture(&textures.enemyTexture, "res/enemy.png");
	UpdateList::loadTexture(&textures.bulletTexture, "res/bullet.png");
	UpdateList::loadTexture(&textures.diceTexture, "res/dice.png");
	UpdateList::loadTexture(&textures.tilesTexture, "res/dicetiles.png");
	UpdateList::loadTexture(&textures.borderTexture, "res/border.png");
	UpdateList::loadTexture(&textures.winTexture, "res/winscreen.png");
	UpdateList::loadTexture(&textures.loseTexture, "res/losescreen.png");
	UpdateList::loadTexture(&textures.titleTexture, "res/titlescreen.png");
	UpdateList::loadTexture(&textures.startTexture, "res/startbutton.png");
	UpdateList::loadTexture(&textures.quitTexture, "res/quitbutton.png");

	//Setup player
	Player player(&textures);
	player.setTexture(textures.playerTexture);
	player.setScale(GRIDSCALE, GRIDSCALE);
	UpdateList::addNode(&player);

	//Lock UI Layers
	UpdateList::staticLayer(DIETOP);
	UpdateList::staticLayer(BORDER);
	UpdateList::staticLayer(HOLDING);
	UpdateList::staticLayer(PLAYER);
	UpdateList::staticLayer(INPUT);

	//Finish engine setup
	UpdateList::setCamera(&player, sf::Vector2f(1920, 1080));
	UpdateList::startEngine("Rolling Labyrinth", TITLE);
	return 0;
}