#include "Skyrmion/InputHandler.hpp"
#include "Skyrmion/TileMap.hpp"
#include "Holder.hpp"
#include "Bullet.hpp"
#include "Menu.hpp"

#define MINFIRERATE 0.3f;

int diceLayout[DICEMAX] = {
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

	sf::Vector2f fireDir = sf::Vector2f(1, 0);
	double fireTime = 0;
	bool fired = false;

	void spawnBullet() {
		//Create bullet
		Bullet *bullet = new Bullet(fireDir, collisionMap);
		bullet->setTexture(textures->bulletTexture);
		bullet->setScale(1.5, 1.5);
		bullet->setPosition(getGPosition());
		UpdateList::addNode(bullet);
	}

public:
	Player(TextureSet *_textures) : Node(PLAYER, sf::Vector2i(10, 11)), 
	movementInput("/movement", INPUT, this), fireInput("/weapon", INPUT, this), 
	placeInput(diceLayout, DICEMAX, INPUT, this), holder(_textures, this), 
	menu(_textures, this), textures(_textures) {

		collideWith(COLLECTABLE);
		collideWith(ENEMY);
		collisionMap = holder.getCollision();

		//Position player
		int cord = (STARTROOM+3)*GRIDSIZE+GRIDSIZE/2;
		setPosition(sf::Vector2f(cord, cord));

		//Place tile listener
		Holder *_holder = &holder;
		placeInput.pressedFunc = [_holder](int i) {
			_holder->placeDie(i);
		};

		//Fire listener
		bool *fired = &this->fired;
		double *fireTime = &this->fireTime;
		fireInput.heldFunc = [fired, fireTime](int i) {
			if(*fireTime <= 0)
				*fired = true;
		};

		UpdateList::addListener(this, sf::Event::MouseButtonPressed);
	}

	void recieveEvent(sf::Event event, WindowSize *windowSize) {
		if(event.mouseButton.button == sf::Mouse::Left) {
			fireDir = windowSize->worldPos(event.mouseButton.x, event.mouseButton.y) - getGPosition();
			if(fireTime <= 0) {
				spawnBullet();
				fireTime = MINFIRERATE;
			}
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
			fireDir = target2;

		//Fire bullet
		if((fireTime -= time) <= 0 && fired) {
			spawnBullet();

			//Reset timer
			fireTime = MINFIRERATE;
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

	void recieveMessage(int id) {
		if(id == RESET_MAP) {
			int cord = (STARTROOM+3)*GRIDSIZE+GRIDSIZE/2;
			setPosition(sf::Vector2f(cord, cord));
		}
	}
};