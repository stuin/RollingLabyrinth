#include "Skyrmion/UpdateList.h"
#include "DiceCollection.hpp"
#include "Collectable.hpp"
#include "Enemy.hpp"

#define DICEMAX 8
#define DICESTART 3
#define DICEMAXTIME 0.2

class DiceRoller {
public:
	AnimatedTileMap *map = NULL;
	int perm = 0;
	int rotation = 0;
	int final = 0;

	DiceRoller(AnimatedTileMap *map) {
		this->map = map;
	}

	int start(DiceCollection *collection) {
		this->final = collection->getNext(6);
		this->perm = collection->getNext(PERMUTATIONS);
		this->rotation = collection->getNext(ROTATIONS);

		for(int j = 0; j < 6; j++)
			map->getFrame(j)->setIndex(collection->getDie(collection->getPerm(perm, rotation, j)));
		map->setCurrentFrame(0);
		map->setHidden(false);
		map->setPauseAfter(6 + final);
		map->setPaused(false);

		return collection->getPerm(perm, rotation, final);
	}

	void copy(DiceRoller *other, DiceCollection *collection) {
		this->final = other->final;
		this->perm = other->perm;
		this->rotation = other->rotation;

		for(int j = 0; j < 6; j++)
			map->getFrame(j)->setIndex(collection->getDie(collection->getPerm(perm, rotation, j)));
		map->setPaused(true);
		map->setCurrentFrame(final);
	}
};

class Holder : public Node {
private:
	int values[DICEMAX];
	std::vector<DiceRoller> dice;
	int count = DICESTART;

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
			AnimatedTileMap *map = new AnimatedTileMap(sf::Vector2i(16*7, 16*7), DICEMAXTIME, HOLDING);
			for(int j = 0; j < 6; j++)
				map->addFrame(new TileMap(&textures->tilesTexture, 
					16, 16, collection.getDie(0), HOLDING));
			map->setHidden(true);
			map->setParent(this);
			map->setPosition((i-4)*(DIEWIDTH+16)+16, -56);
			dice.emplace_back(map);
			UpdateList::addNode(map);

			if(i < DICESTART)
				values[i] = dice[i].start(&collection);
		}

		UpdateList::addNode(this);
		UpdateList::addListener(this, sf::Event::MouseButtonPressed);
	}

	void recieveEvent(sf::Event event, WindowSize *windowSize) {
		if(event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2f pos = windowSize->worldPos(event.mouseButton.x, event.mouseButton.y);
			if(getRect().contains(pos)) {
				for(int i = 0; i < count; i++) {
					if(dice[i].map->getRect().contains(pos))
						placeDie(i);
				}
			}
		}
	}

	void recieveMessage(int id, Node *sender) {
		if(id == RESET_MAP) {
			for(int i = 0; i < DICEMAX; i++) {
				if(i < DICESTART)
					values[i] = dice[i].start(&collection);
				else
					dice[i].map->setHidden(true);
			}
			count = DICESTART;
			collection.rebuildMap();
		}
	}

	Indexer *getCollision() {
		return collection.getCollision();
	}

	void addDie() {
		if(count >= DICEMAX)
			deleteDie(-1);
		values[count] = dice[count].start(&collection);
		count++;
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
			dice[j].copy(&dice[j + 1], &collection);
		}
		count--;
		dice[count].map->setHidden(true);
		return count;
	}

	void placeNode(int x, int y, char c, sf::Vector2f pos) {
		x = (x + pos.x) * GRIDSIZE + GRIDSIZE/2;
		y = (y + pos.y) * GRIDSIZE + GRIDSIZE/2;

		if(c == 'a' || collection.getNext(8) == 0) {
			Collectable *d = new Collectable(&collection);
			d->setTexture(textures->diceTexture);
			d->setPosition(x, y);
			UpdateList::addNode(d);
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
				//Locate top left corner
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
				int goalTile = (values[i] / 4 == 0) ? WALL : EMPTY;
				if(collisionMap->getTile(sf::Vector2f(x*GRIDSIZE, y*GRIDSIZE)) == goalTile) {
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