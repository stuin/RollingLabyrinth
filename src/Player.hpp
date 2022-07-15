#include "Skyrmion/InputHandler.hpp"
#include "Skyrmion/TileMap.hpp"

sf::Keyboard::Key controlLayouts[3][4] = {
	{sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D},
	{sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right},
	{sf::Keyboard::W, sf::Keyboard::R, sf::Keyboard::A, sf::Keyboard::S}
};

class Player : public Node {
	InputHandler input;
	Indexer collisionMap;

	//End screen stuff
	sf::Texture endTexture;
	Node endNode;

public:
	int treasure = 0;
	bool endShown = false;

	Player(Indexer _collisionMap) : 
		Node(PLAYER), input(controlLayouts[2], INPUT, this), 
		collisionMap(_collisionMap), endNode(TITLE, sf::Vector2i(64, 32), false, this) {

		collideWith(TREASURE);
	}

	void update(double time) {
		sf::Vector2f target = input.getMovement(this, time * 320);
		int targetType = collisionMap.getTile(target);

		//Move player
		if(!endShown) {
			//if(targetType == FLOOR)
			setPosition(target);

			//Check for win condition
			if(targetType == EXIT) {
				//Load the end texture
				std::string endFile = "res/endscreen.png";
				if(!endTexture.loadFromFile(endFile))
					throw std::invalid_argument("End texture " + endFile + " not found");
				endNode.setTexture(endTexture);
				endNode.setPosition(0, -48);
				UpdateList::addNode(&endNode);

				std::cout << "YOU WIN!!\n";
				std::cout << treasure << " Treasure chests collected!\n";
				
				endShown = true;
			}
		}
	}

	void collide(Node *object) {
		treasure++;
		object->setDelete();
	}
};