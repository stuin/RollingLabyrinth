#include "Skyrmion/UpdateList.h"
#include "DiceCollection.hpp"

#define DICEMAX 8
#define DICESTART 4

class Holder : public Node {
private:
	int values[DICEMAX];
	std::vector<TileMap> dice;

	DiceCollection collection;
	int count = DICESTART;

public:
	Holder(sf::Texture *_tileset, sf::Texture *borderTexture, Node *parent) 
	: Node(BORDER, sf::Vector2i(1024, 128), false, parent), collection(_tileset) {
		setPosition(sf::Vector2f(0, 400));
		setTexture(*borderTexture);

		//Set up holding dice
		dice.reserve(DICEMAX);
		for(int i = 0; i < DICEMAX; i++) {
			//dice.emplace_back(HOLDING, sf::Vector2i(DIEWIDTH, DIEWIDTH), true, this);
			dice.emplace_back(_tileset, 16, 16, collection.getDie(0), HOLDING);
			dice[i].setHidden(true);
			dice[i].setParent(this);
			dice[i].setPosition((i-4)*(DIEWIDTH+16)+16, -56);
			UpdateList::addNode(&(dice[i]));

			if(i < DICESTART) {
				values[i] = collection.getNext();
				std::cout << values[i] << "\n";
				//dice[i].setTexture(collection.getTexture(values[i]));
				dice[i].setIndex(collection.getDie(values[i]));
				dice[i].setHidden(false);
			}
		}

		UpdateList::addNode(this);
		UpdateList::addListener(this, sf::Event::MouseButtonPressed);
	}

	Indexer *getCollision() {
		return collection.getCollision();
	}

	void overlayGrid(int i, int x, int y) {
		collection.overlayGrid(collection.getDie(values[i]), x, y);
	}

	int getCount() {
		return count;
	}
};