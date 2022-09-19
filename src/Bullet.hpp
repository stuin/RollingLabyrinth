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
		sf::Vector2f target = getShiftedPosition(velocity, time * 600);
		int targetType = collisionMap->getTile(target);

		if(targetType == WALL || targetType == EMPTY)
			setDelete();
		else
			setPosition(target);
	}

	void collide(Node *other) {
		other->setDelete();
		setDelete();
	}
};