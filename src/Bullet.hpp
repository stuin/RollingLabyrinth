class Bullet : public Node {
private:
	sf::Vector2f velocity;
	Indexer *collisionMap;

public:
	Bullet(sf::Vector2f _velocity, Indexer *_collisionMap) 
	: Node(BULLET, sf::Vector2i(8,8)), 
	velocity(_velocity), collisionMap(_collisionMap) {
		collideWith(ENEMY);
	}

	void update(double time) {
		sf::Vector2f target = move(velocity, time * 600);

		if(collisionMap->getTile(target) <= 0)
			setDelete();
	}

	void collide(Node *other) {
		other->setDelete();
		setDelete();
	}

	void recieveMessage(int id) {
		if(id == CLEAR_ENTITIES)
			setDelete();
	}
};