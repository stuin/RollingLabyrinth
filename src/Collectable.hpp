class Collectable : public Node {
private:
	DiceCollection *collection;
	int maxTime = 30;
	double nextTime;
	int perm;
	int index = 0;

public:
	Collectable(DiceCollection *_collection) : Node(COLLECTABLE, sf::Vector2i(14, 14)),
	collection(_collection) {
		this->nextTime = maxTime;
		this->perm = _collection->getNext(PERMUTATIONS);

		int i = _collection->getPerm(perm, 0, index++) / 4;
		setTextureRect(sf::IntRect(0, 14*i, 14, 14));
		setScale(2, 2);
	}

	void update(double time) {
		if(nextTime-- <= 0) {
			int i = collection->getPerm(perm, 0, index++) / 4;
			setTextureRect(sf::IntRect(0, 14*i, 14, 14));
			nextTime = maxTime;
		}
	}
};