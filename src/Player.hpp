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
	sf::Keyboard::Num5, sf::Keyboard::Num6, sf::Keyboard::Num7
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
	bool endShown = false;

	Player(TextureSet *textures) : 
		Node(PLAYER), movement(controlLayouts[2], INPUT, this), 
		input(diceLayout, 4, INPUT, this),
		holder(textures, this),
		endNode(TITLE, sf::Vector2i(64, 32), true, this) {

		collideWith(COLLECTABLE);
		collisionMap = holder.getCollision();
		endNode.setTexture(textures->endTexture);
		endNode.setPosition(0, -48);

		//Place new tile
		Holder *_holder = &holder;
		input.pressedFunc = [_holder](int i) {
			_holder->placeDie(i);
		};
	}

	void update(double time) {
		sf::Vector2f target = movement.getMovement(this, time * 320);
		int targetType = collisionMap->getTile(target);

		//Move player
		if(!endShown) {
			if(targetType != WALL && targetType != EMPTY)
				setPosition(target);

			//Check for win condition
			if(targetType == EXIT) {
				//Show end screen
				endNode.setHidden(false);
				UpdateList::addNode(&endNode);
				endShown = true;
			}
		}
	}

	void collide(Node *object) {
		holder.addDie();
		object->setDelete();
	}
};