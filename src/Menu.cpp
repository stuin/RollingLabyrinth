#include "Skyrmion/UpdateList.h"
#include "spawners.h"

class Menu : public DrawNode {
private:
	sf::RectangleShape fade;
	TextureSet *textures;

	Node endText;
	Node titleText;
	Node startButton;
	Node restartButton;
	Node quitButton;

public:
	Menu(TextureSet *_textures, Node *parent) 
	: DrawNode(fade, MENU, sf::Vector2i(2000, 2000), parent), 
	endText(TITLE, sf::Vector2i(283, 99), true, this), titleText(TITLE, sf::Vector2i(328, 158), false, this), 
	startButton(TITLE, sf::Vector2i(156, 75), false, this), restartButton(TITLE, sf::Vector2i(156, 75), false, this), 
	quitButton(TITLE, sf::Vector2i(156, 75), false, this) {
		
		this->textures = _textures;
		pauseGame(true);

		//Main menu
		titleText.setTexture(_textures->titleTexture);
		titleText.setPosition(0, -300);
		startButton.setTexture(_textures->startTexture);
		startButton.setPosition(-400, -100);
		quitButton.setTexture(_textures->quitTexture);
		quitButton.setPosition(-400, 100);

		//Pause menu
		restartButton.setTexture(_textures->startTexture);
		restartButton.setPosition(200, 200);
		restartButton.setHidden();

		//Title Shading
		fade.setSize(sf::Vector2f(2000, 2000));
		fade.setFillColor(sf::Color(255, 255, 255, 50));

		UpdateList::addNode(&endText);
		UpdateList::addNode(&titleText);
		UpdateList::addNode(&startButton);
		UpdateList::addNode(&restartButton);
		UpdateList::addNode(&quitButton);
		UpdateList::addNode(this);
		UpdateList::hideLayer(HOLDING, true);
		UpdateList::addListener(this, sf::Event::MouseButtonPressed);
		UpdateList::addListener(this, sf::Event::KeyPressed);
	}

	void recieveEvent(sf::Event event, WindowSize *windowSize) {
		if(event.type == sf::Event::KeyPressed) {
			if(!startButton.isHidden() && event.key.code == sf::Keyboard::Space)
				startGame();
			else if(event.key.code == sf::Keyboard::Escape)
				pauseGame(isHidden());
		} else if(event.mouseButton.button == sf::Mouse::Left && !isHidden()) {
			sf::Vector2f pos = windowSize->worldPos(event.mouseButton.x, event.mouseButton.y);
			//std::cout << pos.x << ":" << pos.y << "\n";
			if(startButton.getRect().contains(pos))
				startGame();
			else if(quitButton.getRect().contains(pos))
				UpdateList::stopEngine();
			else if(restartButton.getRect().contains(pos))
				restartGame();
		}
	}

	void recieveMessage(int id) {
		if(id == SHOW_WIN)
			showEnd(true);
		else if(id == SHOW_LOST)
			showEnd(false);
	}

	void startGame() {
		titleText.setHidden();
		startButton.setHidden();
		restartButton.setHidden(false);
		quitButton.setPosition(-200, 200);
		UpdateList::hideLayer(HOLDING, false);
		pauseGame(false);
	}

	void restartGame() {
		UpdateList::sendSignal(CLEAR_ENTITIES);
		UpdateList::sendSignal(RESET_MAP);
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
			endText.setTexture(textures->winTexture);
		else
			endText.setTexture(textures->loseTexture);
		endText.setScale(3, 3);
		endText.setPosition(0, -48);
		endText.setHidden(false);
		pauseGame(true);
	}
};

void spawnMenu(TextureSet *_textures, Node *parent) {
	new Menu(_textures, parent);
}