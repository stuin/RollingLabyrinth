#include "Skyrmion/input/InputHandler.h"
#include "Skyrmion/core/UpdateList.h"
#include "spawners.h"

std::vector<std::string> menuLayout = {
	"/menu/start", "/menu/pause"
};

class Menu : public Node {
private:
	InputHandler menuInput;

	Node endText;
	Node titleText;
	Node startButton;
	Node restartButton;
	Node quitButton;

public:
	Menu(Node *parent)
	: Node(MENU, Vector2i(2000, 2000), false, parent),
	menuInput(menuLayout, INPUT, this),
	endText(TITLE, Vector2i(283, 99), true, this), titleText(TITLE, Vector2i(328, 158), false, this),
	startButton(TITLE, Vector2i(156, 75), false, this), restartButton(TITLE, Vector2i(156, 75), false, this),
	quitButton(TITLE, Vector2i(156, 75), false, this) {

		pauseGame(true);
		setScale(parent->getInverseScale());

		//Main menu
		titleText.setTexture(titleTexture);
		titleText.setUPosition(0, -300);
		startButton.setTexture(startTexture);
		startButton.setUPosition(-400, -100);
		quitButton.setTexture(quitTexture);
		quitButton.setUPosition(-400, 100);

		//Pause menu
		restartButton.setTexture(startTexture);
		restartButton.setUPosition(200, 200);
		restartButton.setHidden();

		//Text position
		endText.setScale(3, 3);
		endText.setUPosition(0, -48);

		//Title Shading
		setTexture(transparencyTexture);
		setTextureIntRect(IntRect(50, 0, 1, 1));
		//fade.setSize(Vector2f(2000, 2000));
		//fade.setFillColor(skColor(255, 255, 255, 50));

		//Link buttons
		Menu *_menu = this;
		menuInput.pressedFunc = [_menu](int i) {
			if(i == 0) {
				if(!_menu->startButton.isHidden())
					_menu->startGame();
				else if(!_menu->restartButton.isHidden())
					_menu->restartGame();
			} else if(i == 1) {
				if(_menu->endText.isHidden())
					_menu->pauseGame(_menu->isHidden());
				else
					UpdateList::stopEngine();
			}
		};

		UpdateList::addNode(&endText);
		UpdateList::addNode(&titleText);
		UpdateList::addNode(&startButton);
		UpdateList::addNode(&restartButton);
		UpdateList::addNode(&quitButton);
		UpdateList::addNode(this);
		UpdateList::hideLayer(HOLDING, true);
		UpdateList::addListener(this, EVENT_MOUSE);
	}

	void recieveEvent(Event event) {
		if(event.code == 0 && event.down && !isHidden()) {
			Vector2f pos = screenToGlobal(event.x, event.y);
			//std::cout << pos.x << ":" << pos.y << "\n";
			if(startButton.getRect().contains(pos))
				startGame();
			else if(quitButton.getRect().contains(pos))
				UpdateList::stopEngine();
			else if(restartButton.getRect().contains(pos))
				restartGame();
		}
	}

	void recieveSignal(int id, Node *sender) {
		if(id == SHOW_WIN)
			showEnd(true);
		else if(id == SHOW_LOST)
			showEnd(false);
	}

	void startGame() {
		titleText.setHidden();
		startButton.setHidden();
		restartButton.setHidden(false);
		quitButton.setUPosition(-200, 200);
		UpdateList::hideLayer(HOLDING, false);
		pauseGame(false);
	}

	void restartGame() {
		UpdateList::sendSignal(CLEAR_ENTITIES, this);
		UpdateList::sendSignal(RESET_MAP, this);
		pauseGame(false);
	}

	void pauseGame(bool pause) {
		setHidden(!pause);
		UpdateList::pauseLayer(PLAYER, pause);
		UpdateList::pauseLayer(BULLET, pause);
		UpdateList::pauseLayer(ENEMY, pause);
		UpdateList::pauseLayer(BORDER, pause);
		UpdateList::pauseLayer(HOLDING, pause);
	}

	void showEnd(bool win) {
		if(win)
			endText.setTexture(winTexture);
		else
			endText.setTexture(loseTexture);
		endText.setHidden(false);
		pauseGame(true);
	}
};

void spawnMenu(Node *parent) {
	new Menu(parent);
}