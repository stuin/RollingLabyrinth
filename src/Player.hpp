#include "Skyrmion/InputHandler.hpp"
#include "Skyrmion/TileMap.hpp"
#include "Holder.hpp"
#include "Bullet.hpp"
#include "Menu.hpp"

sf::Keyboard::Key controlLayouts[3][4] = {
	{sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D},
	{sf::Keyboard::W, sf::Keyboard::R, sf::Keyboard::A, sf::Keyboard::S},
	{sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right}
};

sf::Keyboard::Key diceLayout[DICEMAX] = {
	sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3, sf::Keyboard::Num4,
	sf::Keyboard::Num5, sf::Keyboard::Num6, sf::Keyboard::Num7, sf::Keyboard::Num8
};

class Player : public Node {
	DirectionHandler movementInput;
	DirectionHandler fireInput;
	InputHandler placeInput;
	Indexer *collisionMap;

	Holder holder;
	Menu menu;
	TextureSet *textures;

	sf::Vector2f fireAt = sf::Vector2f(1, 0);
	bool fired = false;

public:
	Player(TextureSet *_textures) : Node(PLAYER, sf::Vector2i(10, 11)), 
	movementInput(controlLayouts[0], INPUT, this), fireInput(controlLayouts[2], INPUT, this), 
	placeInput(diceLayout, DICEMAX, INPUT, this), holder(_textures, this), 
	menu(_textures, this), textures(_textures) {

		collideWith(COLLECTABLE);
		collideWith(ENEMY);
		collisionMap = holder.getCollision();

		//Place new tile
		Holder *_holder = &holder;
		placeInput.pressedFunc = [_holder](int i) {
			_holder->placeDie(i);
		};

		UpdateList::addListener(this, sf::Event::MouseButtonPressed);
		UpdateList::addListener(this, sf::Event::KeyPressed);
	}

	void recieveEvent(sf::Event event, WindowSize *windowSize) {
		if(event.type == sf::Event::MouseButtonPressed) {
			if(event.mouseButton.button == sf::Mouse::Left) {
				fireAt = windowSize->worldPos(event.mouseButton.x, event.mouseButton.y) - getGPosition();
				fired = true;
			}
		} else {
			if(event.key.code == sf::Keyboard::Space)
				fired = true;
		}
	}

	void update(double time) {
		sf::Vector2f target = movementInput.getMovement(this, time * 300);
		int targetType = collisionMap->getTile(target);

		//Move player
		if(targetType != WALL && targetType != EMPTY)
			setPosition(target);

		//Win game
		if(targetType == EXIT)
			menu.showEnd(true);

		//Aim at arrow keys
		sf::Vector2f target2 = fireInput.getDirection();
		if(target2 != sf::Vector2f(0, 0))
			fireAt = target2;

		//Fire bullet
		if(fired) {
			Bullet *bullet = new Bullet(fireAt, collisionMap);
			bullet->setTexture(textures->bulletTexture);
			bullet->setScale(1.5, 1.5);
			bullet->setPosition(getGPosition());
			UpdateList::addNode(bullet);
			fired = false;
		}
	}

	void collide(Node *object) {
		if(object->getLayer() == COLLECTABLE) {
			holder.addDie();
			object->setDelete();
		} else if(object->getLayer() == ENEMY) {
			if(holder.deleteDie(-1) == -1)
				menu.showEnd(false);
			object->setDelete();
		}
	}
};