class Bullet : public Node {
private:
	Vector2f velocity;
	Indexer *collisionMap;

public:
	Bullet(Vector2f _velocity, Indexer *_collisionMap)
	: Node(BULLET, Vector2i(16,16)),
	velocity(_velocity), collisionMap(_collisionMap) {
		collideWith(ENEMY);
		//setScale(1.5, 1.5);
	}

	void update(double time) {
		Vector2f target = getPosition() + velocity * time;

		if(collisionMap->getTile(target) == FULL)
			setDelete();
		else
			setPosition(target);
	}

	void collide(Node *other) {
		other->setDelete();
		setDelete();
	}

	void recieveSignal(int id, Node *sender) {
		if(id == CLEAR_ENTITIES)
			setDelete();
	}
};