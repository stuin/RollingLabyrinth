#define COLLECTABLEMAXTIME 0.5

class Collectable : public Node {
private:
	DiceCollection *collection;
	double nextTime = COLLECTABLEMAXTIME;
	int perm;
	int index = 0;

public:
	Collectable(DiceCollection *_collection) : Node(COLLECTABLE, Vector2i(14, 14)),
	collection(_collection) {
		this->perm = _collection->getNext(PERMUTATIONS);

		int i = _collection->getPerm(perm, 0, index++) / 4;
		setTextureIntRect(IntRect(0, 14*i, 14, 14));
		setScale(2, 2);
	}

	void update(double time) {
		if((nextTime -= time) <= 0) {
			int i = collection->getPerm(perm, 0, index++) / 4;
			setTextureIntRect(IntRect(0, 14*i, 14, 14));
			nextTime = COLLECTABLEMAXTIME;
		}
	}

	void recieveSignal(int id, Node *sender) {
		if(id == CLEAR_ENTITIES)
			setDelete();
	}
};