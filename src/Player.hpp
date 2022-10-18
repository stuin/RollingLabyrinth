#include "Skyrmion/InputHandler.hpp"
#include "Skyrmion/TileMap.hpp"
#include "Holder.hpp"
#include "Bullet.hpp"
#include "Menu.hpp"

#define MINFIRERATE 0.3f;

std::vector<std::string> diceLayout = {
	"/dice/1", "/dice/2", "/dice/3", "/dice/4",
	"/dice/5", "/dice/6", "/dice/7", "/dice/8",
}; 

class Player : public Node {
	DirectionHandler movementInput;
	DirectionHandler fireInput;
	InputHandler placeInput;
	Indexer *collisionMap;

	Holder holder;
	Menu menu;
	TextureSet *textures;

	double fireTime = 0;
	sf::Vector2f mouseDir = sf::Vector2f(0, 1);

	void spawnBullet(bool mouse) {
		if(fireTime <= 0) {
			sf::Vector2f dir = fireInput.getDirection();
			if(mouse)
				dir = mouseDir;
			if(dir == sf::Vector2f(0, 0))
				dir = sf::Vector2f(0, 1);

			//Create bullet
			Bullet *bullet = new Bullet(dir, collisionMap);
			bullet->setTexture(textures->bulletTexture);
			bullet->setScale(1.5, 1.5);
			bullet->setPosition(getGPosition());
			UpdateList::addNode(bullet);

			fireTime = MINFIRERATE;
		}
	}

public:

	Player(TextureSet *_textures) : Node(PLAYER, sf::Vector2i(10, 11)), 
	movementInput("/movement", INPUT, this), fireInput("/weapon", INPUT, this), 
	placeInput(diceLayout, INPUT, this), holder(_textures, this), 
	menu(_textures, this), textures(_textures) {

		collideWith(COLLECTABLE);
		collideWith(ENEMY);
		collisionMap = holder.getCollision();

		//Place tile listener
		Holder *_holder = &holder;
		placeInput.pressedFunc = [_holder](int i) {
			_holder->placeDie(i % DICEMAX);
		};

		//Fire listener
		Player *_player = this;
		int fireMouse = fireInput.addKey(MOUSE_OFFSET+0);
		fireInput.heldFunc = [fireMouse, _player](int i) {
			if(i > 3)
				_player->spawnBullet(i == fireMouse);
		}; 

		UpdateList::addListener(this, sf::Event::MouseMoved);
	}

	void recieveEvent(sf::Event event, WindowSize *windowSize) {
		mouseDir = windowSize->worldPos(event.mouseMove.x, event.mouseMove.y) - getGPosition();
	}

	void update(double time) {
		sf::Vector2f target = movementInput.getMovement(this, time * 300);
		int targetType = collisionMap->getTile(target);

		//Move player
		if(targetType != WALL && targetType != EMPTY)
			setPosition(target);

		//Run timer
		if(fireTime > 0)
			fireTime -= time;

		//Win game
		if(targetType == EXIT)
			menu.showEnd(true);
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