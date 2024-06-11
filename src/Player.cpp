#include "Skyrmion/InputHandler.h"
#include "Skyrmion/TileMap.hpp"
#include "Holder.hpp"
#include "Bullet.hpp"

#define MINFIRERATE 0.3f;

std::vector<int> diceLayout = {
	sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3, sf::Keyboard::Num4,
	sf::Keyboard::Num5, sf::Keyboard::Num6, sf::Keyboard::Num7, sf::Keyboard::Num8
};

class Player : public Node {
	DirectionHandler movementInput;
	DirectionHandler fireInput;
	InputHandler placeInput;
	Indexer *collisionMap;

	Holder holder;
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
	placeInput(diceLayout, INPUT, this), 
	holder(_textures, this), textures(_textures) {

		collideWith(COLLECTABLE);
		collideWith(ENEMY);
		collisionMap = holder.getCollision();

		//Position player
		int cord = (STARTROOM+3)*GRIDSIZE+GRIDSIZE/2;
		setPosition(sf::Vector2f(cord, cord));
		setTexture(_textures->playerTexture);
		setScale(GRIDSCALE, GRIDSCALE);

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

		UpdateList::addNode(this);
		UpdateList::addListener(this, sf::Event::MouseButtonPressed);
		UpdateList::setCamera(this, sf::Vector2f(1920, 1080));
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
		move(movementInput.getDirection(), collisionMap, time * 300);

		//Win game
		if(collisionMap->getTile(getPosition()) == EXIT)
			UpdateList::sendSignal(MENU, SHOW_WIN, this);

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
				UpdateList::sendSignal(MENU, SHOW_LOST, this);
			object->setDelete();
		}
	}

	void recieveMessage(int id, Node *sender) {
		if(id == RESET_MAP) {
			int cord = (STARTROOM+3)*GRIDSIZE+GRIDSIZE/2;
			setPosition(sf::Vector2f(cord, cord));
		}
	}
};

void spawnPlayer(TextureSet *_textures) {
	Player *player = new Player(_textures);
	spawnMenu(_textures, player);
}