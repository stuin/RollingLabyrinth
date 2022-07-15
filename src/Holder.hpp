#include "Skyrmion/UpdateList.h"
#include "DiceCollection.hpp"

#define DICEMAX 8
#define DICESTART 4

class Holder : public Node {
private:
	int count = DICESTART;
	int values[DICEMAX];
	std::vector<Node> dice;
	DiceCollection collection;

public:
	Holder(sf::Texture *_tileset): Node(BORDER, sf::Vector2i(1024, 128)), collection(_tileset) {
		dice.reserve(DICEMAX);
		for(int i = 0; i < DICEMAX; i++) {
			dice.emplace_back(HOLDING, sf::Vector2i(DIEWIDTH, DIEWIDTH), true, this);
			dice[i].setPosition((i)*(DIEWIDTH+16), 0);
			UpdateList::addNode(&(dice[i]));

			if(i < DICESTART) {
				values[i] = collection.getNext();
				std::cout << values[i] << "\n";
				dice[i].setTexture(collection.getTexture(values[i]));
				dice[i].setHidden(false);
			}
		}

		UpdateList::addListener(this, sf::Event::MouseButtonPressed);
	}

	void recieveEvent(sf::Event event, int shiftX, int shiftY) {
		sf::Vector2i pos(event.mouseButton.x * shiftX, event.mouseButton.y * shiftY);
		if(event.mouseButton.button == sf::Mouse::Left && getRect().contains(pos)) {
			
		}
	}

	void addDie() {

	}
};