class Enemy : public Node {
private:
	Node *player;
	Indexer *collisionMap;
	double startTimer = 1.2;

public:
	Enemy(Node *_player, Indexer *_collisionMap) : Node(ENEMY), player(_player), collisionMap(_collisionMap) {

	}

	void update(double time) {
		if((startTimer -= time) <= 0) {
			sf::Vector2f target = getShiftedPosition(
				player->getGPosition() - getGPosition(), time * 200);
			int targetType = collisionMap->getTile(target);

			if(targetType != WALL && targetType != EMPTY)
				setPosition(target);
		}
	}
};