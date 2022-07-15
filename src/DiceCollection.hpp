#include "Skyrmion/TileMap.hpp"

#define MAPCOUNT 12
#define DIEWIDTH 112

class DiceCollection {
private:
	std::vector<TileMap> maps;

public:
	DiceCollection(sf::Texture *tileset) {
		maps.reserve(MAPCOUNT);
		for(int i = 1; i <= MAPCOUNT; i++) {
			std::string name = "res/dice/map_a";
			if(i < 10)
				name += '0';
			name += std::to_string(i) + ".txt";
			//std::cout << name << "\n";
			GridMaker grid(name);
			maps.emplace_back(tileset, 16, 16, Indexer(&grid, displayIndex, 0), HOLDING);

			maps[i-1].setPosition(i * DIEWIDTH, 0);
			UpdateList::addNode(&(maps[i-1]));
		}
	}

	int getNext() {
		return rand() % maps.size();
	}

	int getNext(int prev) {
		int next = getNext();
		while(next / 4 == prev / 4)
			next = getNext();
		return next;
	}

	const sf::Texture &getTexture(int index) {
		return *(maps[index].getTexture());
	}
};