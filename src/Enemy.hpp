class Enemy : public Node {
private:
	Node *player;
	Indexer *collisionMap;
	double startTimer = 1.2;

public:
	Enemy(Node *_player, Indexer *_collisionMap) : Node(ENEMY), player(_player), collisionMap(_collisionMap) {

	}

	void update(double time) {
		if((startTimer -= time) <= 0)
			setPosition(topDownMovement(this, player->getGPosition() - getGPosition(), collisionMap, time * 200));
	}

	void recieveSignal(int id, Node *sender) {
		if(id == CLEAR_ENTITIES)
			setDelete();
	}
};