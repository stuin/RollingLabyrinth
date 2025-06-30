#include "Skyrmion/input/InputHandler.h"
#include "Skyrmion/input/MovementSystems.h"
#include "Skyrmion/tiling/TileMap.hpp"
#include "Holder.hpp"
#include "Bullet.hpp"

#define MINFIRERATE 0.3f;

std::vector<std::string> diceLayout = {
	"/dice/1", "/dice/2", "/dice/3", "/dice/4", "/dice/5", "/dice/6", "/dice/7", "/dice/8"
};

class Player : public Node {
	DirectionHandler movementInput;
	DirectionHandler fireInput;
	InputHandler placeInput;
	Indexer *collisionMap1;
	Indexer *collisionMap2;

	Holder holder;

	Vector2f fireDir = Vector2f(1, 0);
	double fireTime = 0;
	bool fired = false;
	bool joystick = false;

	void spawnBullet() {
		//Create bullet
		Bullet *bullet = new Bullet(fireDir, collisionMap2);
		bullet->setTexture(bulletTexture);
		bullet->setPosition(getGPosition());
		UpdateList::addNode(bullet);
	}

public:
	Player() : Node(PLAYER, Vector2i(10, 11)),
	movementInput("/movement", INPUT, this), fireInput("/weapon", INPUT, this),
	placeInput(diceLayout, INPUT, this), holder(this) {

		collideWith(COLLECTABLE);
		collideWith(ENEMY);
		collisionMap1 = holder.getCollision1();
		collisionMap2 = holder.getCollision2();

		//Position player
		int cord = (STARTROOM+3)*GRIDSIZE+GRIDSIZE/2;
		setPosition(Vector2f(cord, cord));
		setTexture(playerTexture);

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
		UpdateList::addListener(this, EVENT_MOUSE);
		UpdateList::setCamera(this, Vector2f(1920.0/getScale().x, 1080.0/getScale().y));
	}

	void recieveEvent(Event event) {
		if(event.code == 0 && event.down) {
			fireDir = vectorLength(screenToGlobal(event.x, event.y) - getGPosition(), 600);
			if(fireTime <= 0) {
				spawnBullet();
				fireTime = MINFIRERATE;
			}
		}
	}

	void update(double time) {
		setPosition(topDownMovement(this, movementInput.getDirection(), collisionMap2, time * 300));

		//Check control scheme
		if(movementInput.joystickMovement != joystick) {
			joystick = movementInput.joystickMovement;
			//std::cout << "Joystick: " << joystick << "\n";
			UpdateList::sendSignal(BORDER, joystick ? CONTROLS_JOYSTICK : CONTROLS_KEYBOARD, this);
		}

		//Win game
		if(collisionMap1->getTile(getPosition()) == EXIT)
			UpdateList::sendSignal(MENU, SHOW_WIN, this);

		//Aim at arrow keys
		Vector2f target2 = fireInput.getDirection();
		if(target2 != Vector2f(0, 0))
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

	void recieveSignal(int id, Node *sender) {
		if(id == RESET_MAP) {
			int cord = (STARTROOM+3)*GRIDSIZE+GRIDSIZE/2;
			setPosition(Vector2f(cord, cord));
		}
	}
};

void spawnPlayer() {
	Player *player = new Player();
	spawnMenu(player);
}