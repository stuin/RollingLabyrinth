#include "Skyrmion/UpdateList.h"
#include "DiceCollection.hpp"
#include "Enemy.hpp"

#define DICEMAX 8
#define DICESTART 3

class Holder : public Node {
private:
	int values[DICEMAX];
	std::vector<TileMap> dice;
	int count = DICESTART;

	AnimatedTileMap *rolling;
	int endFrame = 0;

	DiceCollection collection;
	TextureSet *textures;

public:
	Holder(TextureSet *_textures, Node *parent) : Node(BORDER, sf::Vector2i(1024, 128), false, parent), 
	collection(_textures), textures(_textures) {
		setPosition(sf::Vector2f(0, 450));
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

	int deleteDie(int i) {
		if(count == 0)
			return -1;

		//Choose random die
		if(i == -1)
			i = collection.getNext(count);

		//Shift held dice
		for(int j = i; j < count - 1; j++) {
			values[j] = values[j + 1];
			dice[j].setIndex(collection.getDie(values[j]));
		}
		count--;
		dice[count].setHidden(true);
		return count;
	}

	void placeNode(int x, int y, char c, sf::Vector2f pos) {
		x = (x + pos.x) * GRIDSIZE + GRIDSIZE/2;
		y = (y + pos.y) * GRIDSIZE + GRIDSIZE/2;

		if(c == 'a') {
			Node *t = new Node(COLLECTABLE, sf::Vector2i(14, 14));
			t->setTexture(textures->diceTexture);
			t->setTextureRect(sf::IntRect(0, 0, 14, 14));
			t->setScale(2, 2);
			t->setPosition(x, y);
			UpdateList::addNode(t);
		} else if(c == 'b') {
			Enemy *t = new Enemy(getParent(), collection.getCollision());
			t->setTexture(textures->enemyTexture);
			t->setScale(3, 3);
			t->setPosition(x, y);
			UpdateList::addNode(t);
		}
	}

	void placeDie(int i) {
		sf::Vector2f pos = getParent()->getPosition();
		Indexer *collisionMap = collection.getCollision();
		Holder *_holder = this;

		if(collisionMap->getTile(pos) == EDGE) {
			int x = pos.x / collisionMap->getScale().x - 1;
			int y = pos.y / collisionMap->getScale().y - 1;
			//std::cout << x << " " << y << std::endl;
			if(i < count) {
				if(x % 7 == 0)
					x -= 3;
				else if(x % 7 == 6)
					x += 3;
				else if(y % 7 == 0)
					y -= 3;
				else if(y % 7 == 6)
					y += 3;

				x = (x / 7) * 7 + 1;
				y = (y / 7) * 7 + 1;

				//Place tiles
				if(collisionMap->getTile(sf::Vector2f(x*GRIDSIZE, y*GRIDSIZE)) == EMPTY) {
					collection.overlayGrid(collection.getDie(values[i]), x, y);

					//Place dice/enemy
					collection.getDie(values[i])->mapGrid([x, y, _holder](char c, sf::Vector2f pos) {
						if(c == 'a' || c == 'b')
							_holder->placeNode(x, y, c, pos);
					});

					deleteDie(i);
				}
			}
		}
	}
};