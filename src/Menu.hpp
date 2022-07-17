class Menu : public DrawNode {
private:
	sf::RectangleShape fade;
	TextureSet *textures;

	Node endText;
	Node titleText;
	Node startButton;
	Node quitButton;

public:
	Menu(TextureSet *_textures, Node *parent) 
	: DrawNode(fade, SHADING, sf::Vector2i(2000, 2000), parent), 
	endText(TITLE, sf::Vector2i(283, 99), true, this), titleText(TITLE, sf::Vector2i(328, 158), false, this), 
	startButton(TITLE, sf::Vector2i(156, 75), false, this), quitButton(TITLE, sf::Vector2i(156, 75), false, this) {
		
		this->textures = _textures;
		pauseGame(true);

		//Main menu
		titleText.setTexture(_textures->titleTexture);
		titleText.setPosition(0, -300);
		startButton.setTexture(_textures->startTexture);
		startButton.setPosition(-400, -100);
		quitButton.setTexture(_textures->quitTexture);
		quitButton.setPosition(-400, 100);

		//Title Shading
		fade.setSize(sf::Vector2f(2000, 2000));
		fade.setFillColor(sf::Color(255, 255, 255, 50));

		UpdateList::addNode(&endText);
		UpdateList::addNode(&titleText);
		UpdateList::addNode(&startButton);
		UpdateList::addNode(&quitButton);
		UpdateList::addNode(this);
		UpdateList::hideLayer(HOLDING, true);
		UpdateList::addListener(this, sf::Event::MouseButtonPressed);
	}

	void recieveEvent(sf::Event event, WindowSize *windowSize) {
		if(event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2f pos = windowSize->worldPos(event.mouseButton.x, event.mouseButton.y);
			if(startButton.getRect().contains(pos))
				startGame();
			else if(quitButton.getRect().contains(pos))
				UpdateList::stopEngine();
		}
	}

	void startGame() {
		titleText.setHidden();
		startButton.setHidden();
		quitButton.setHidden();
		UpdateList::hideLayer(HOLDING, false);
		pauseGame(false);
	}

	void pauseGame(bool pause) {
		setHidden(!pause);
		UpdateList::pauseLayer(PLAYER, pause);
		UpdateList::pauseLayer(BULLET, pause);
		UpdateList::pauseLayer(ENEMY, pause);
		UpdateList::pauseLayer(BORDER, pause);
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