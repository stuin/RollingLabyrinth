#include "Skyrmion/InputHandler.hpp"
#include "Skyrmion/TileMap.hpp"
#include "Holder.hpp"

sf::Keyboard::Key controlLayouts[3][4] = {
	{sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D},
	{sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right},
	{sf::Keyboard::W, sf::Keyboard::R, sf::Keyboard::A, sf::Keyboard::S}
};

sf::Keyboard::Key diceLayout[DICEMAX] = {
	sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3, sf::Keyboard::Num4,
	sf::Keyboard::Num5, sf::Keyboard::Num6, sf::Keyboard::Num7, sf::Keyboard::Num8
};

class Player : public Node {
	DirectionHandler movement;
	InputHandler input;
	Indexer *collisionMap;

	Holder holder;

	//End screen stuff
	sf::Texture endTexture;
	Node endNode;

public:
	int treasure = 0;
	bool endShown = false;

	Player(sf::Texture *tilesTexture, sf::Texture *borderTexture) : 
		Node(PLAYER), movement(controlLayouts[2], INPUT, this), 
		input(diceLayout, 4, INPUT, this),
		holder(tilesTexture, borderTexture, this),
		endNode(TITLE, sf::Vector2i(64, 32), false, this) {

		collideWith(TREASURE);
		collisionMap = holder.getCollision();

		//Place new tile
		Indexer *_collisionMap = this->collisionMap;
		Holder *_holder = &(this->holder);
		Player *_player = this;
		input.pressedFunc = [_holder, _player, _collisionMap](int i) {
			if(_collisionMap->getTile(_player->getPosition()) == EDGE) {
				int x = _player->getPosition().x / _collisionMap->getScale().x;
				int y = _player->getPosition().y / _collisionMap->getScale().y;
				if(i < _holder->getCount()) {
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
					std::cout << x << " " << y << "\n";

					_holder->overlayGrid(i, x, y);
				}
			}
		};
	}

	void update(double time) {
		sf::Vector2f target = movement.getMovement(this, time * 320);
		int targetType = collisionMap->getTile(target);

		//Move player
		if(!endShown) {
			if(targetType != WALL)
				setPosition(target);

			//Check for win condition
			if(targetType == EXIT) {
				//Load the end texture
				std::string endFile = "res/endscreen.png";
				if(!endTexture.loadFromFile(endFile))
					throw std::invalid_argument("End texture " + endFile + " not found");
				endNode.setTexture(endTexture);
				endNode.setPosition(0, -48);
				UpdateList::addNode(&endNode);

				std::cout << "YOU WIN!!\n";
				std::cout << treasure << " Treasure chests collected!\n";
				
				endShown = true;
			}
		}
	}

	void collide(Node *object) {
		treasure++;
		object->setDelete();
	}
};