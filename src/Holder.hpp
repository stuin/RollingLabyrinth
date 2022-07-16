#include "Skyrmion/UpdateList.h"
#include "DiceCollection.hpp"

#define DICEMAX 7
#define DICESTART 3

class Holder : public Node {
private:
	int values[DICEMAX];
	std::vector<TileMap> dice;

	DiceCollection collection;
	TextureSet *textures;
	int count = DICESTART;

public:
	Holder(TextureSet *_textures, Node *parent) 
	: Node(BORDER, sf::Vector2i(1024, 128), false, parent), collection(_textures), textures(_textures) {
		setPosition(sf::Vector2f(0, 400));
		setTexture(textures->borderTexture);

		//Set up holding dice
		dice.reserve(DICEMAX);
		for(int i = 0; i < DICEMAX; i++) {
			//dice.emplace_back(HOLDING, sf::Vector2i(DIEWIDTH, DIEWIDTH), true, this);
			dice.emplace_back(&textures->tilesTexture, 16, 16, collection.getDie(0), HOLDING);
			dice[i].setHidden(true);
			dice[i].setParent(this);
			dice[i].setPosition((i-4)*(DIEWIDTH+16)+16, -56);
			UpdateList::addNode(&(dice[i]));

			if(i < DICESTART) {
				values[i] = collection.getNext(MAPCOUNT);
				//dice[i].setTexture(collection.getTexture(values[i]));
				dice[i].setIndex(collection.getDie(values[i]));
				dice[i].setHidden(false);
			}
		}

		UpdateList::addNode(this);
		UpdateList::addListener(this, sf::Event::MouseButtonPressed);

		/*for(int j = 0; j < 10; j++) {
			for(int i = 0; i < 6; i++)
				std::cout << collection.getNext(j,j,i) << " ";
			std::cout << "\n";
		}*/
	}

	Indexer *getCollision() {
		return collection.getCollision();
	}

	void addDie() {
		if(count < MAPCOUNT) {
			values[count] = collection.getNext(MAPCOUNT);
			dice[count].setIndex(collection.getDie(values[count]));
			dice[count].setHidden(false);
			count++;
		}
	}

	void placeDie(int i) {
		sf::Vector2f pos = getParent()->getPosition();
		Indexer *collisionMap = collection.getCollision();
		sf::Texture *treasureTexture = &textures->diceTexture;

		if(collisionMap->getTile(pos) == EDGE) {
			int x = pos.x / collisionMap->getScale().x;
			int y = pos.y / collisionMap->getScale().y;
			if(i < count) {
				if(x % 7 == 0)
					x -= 3;
				else if(x % 7 == 6)
					x += 3;
				else if(y % 7 == 0)
					y -= 3;
				else if(y % 7 == 6)
					y += 3;

				x = (x / 7) * 7;
				y = (y / 7) * 7;

				//Place tiles
				if(collisionMap->getTile(sf::Vector2f(x*96, y*96)) == EMPTY) {
					collection.overlayGrid(collection.getDie(values[i]), x, y);

					//Place treasure
					collection.getDie(values[i])->mapGrid([x, y, treasureTexture](char c, sf::Vector2f pos) {
						if(c == 'a') {
							int _x = (x + pos.x) * 96 + 48;
							int _y = (y + pos.y) * 96 + 48;

							Node *t = new Node(COLLECTABLE, sf::Vector2i(14, 14));
							t->setTexture(*treasureTexture);
							t->setTextureRect(sf::IntRect(0, 0, 14, 14));
							t->setScale(3, 3);
							t->setPosition(_x, _y);
							UpdateList::addNode(t);
						}
					});

					//Shift held dice
					for(int j = i; j < count - 1; j++) {
						values[j] = values[j + 1];
						dice[j].setIndex(collection.getDie(values[j]));
					}
					count--;
					dice[count].setHidden(true);
				}
			}
		}
	}
};