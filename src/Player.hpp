#include "Skyrmion/InputHandler.hpp"
#include "Skyrmion/TileMap.hpp"
#include "Holder.hpp"
#include "Bullet.hpp"

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
	DirectionHandler movementInput;
	InputHandler placeInput;
	Indexer *collisionMap;
	TextureSet *textures;

	Holder holder;
	Node endNode;

	sf::Vector2f fireAt;
	bool fired = false;

public:
	bool endShown = false;

	Player(TextureSet *textures) : Node(PLAYER, sf::Vector2i(10, 10)), 
	movementInput(controlLayouts[2], INPUT, this), placeInput(diceLayout, DICEMAX, INPUT, this),
	holder(textures, this), endNode(TITLE, sf::Vector2i(64, 32), true, this) {

		collideWith(COLLECTABLE);
		collideWith(ENEMY);
		collisionMap = holder.getCollision();

		this->textures = textures;
		endNode.setTexture(textures->endTexture);
		endNode.setPosition(0, -48);

		//Place new tile
		Holder *_holder = &holder;
		placeInput.pressedFunc = [_holder](int i) {
			_holder->placeDie(i);
		};

		UpdateList::addListener(this, sf::Event::MouseButtonPressed);
	}

	void recieveEvent(sf::Event event, WindowSize *windowSize) {
		if(event.mouseButton.button == sf::Mouse::Left) {
			fireAt = windowSize->worldPos(event.mouseButton.x, event.mouseButton.y) - getGPosition();
			fired = true;
		}
	}

	void update(double time) {
		sf::Vector2f target = movementInput.getMovement(this, time * 300);
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

			if(fired) {
				Bullet *bullet = new Bullet(fireAt, collisionMap);
				bullet->setTexture(textures->bulletTexture);
				bullet->setScale(1.5, 1.5);
				bullet->setPosition(getGPosition());
				UpdateList::addNode(bullet);
				fired = false;
			}
		}
	}

	void collide(Node *object) {
		if(object->getLayer() == COLLECTABLE) {
			holder.addDie();
			object->setDelete();
		} else if(object->getLayer() == ENEMY) {
			holder.deleteDie(-1);
			object->setDelete();
		}
	}
};