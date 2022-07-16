#include "Skyrmion/TileMap.hpp"

#define MAPCOUNT 12
#define DIEWIDTH 112

class DiceCollection {
private:
	std::vector<GridMaker> maps;
	GridMaker grid;
	TileMap *dungeon;

public:
	DiceCollection(sf::Texture *tileset) : grid(16*7, 8*7) {
		maps.reserve(MAPCOUNT);
		for(int i = 1; i <= MAPCOUNT; i++) {
			std::string name = "res/dice/map_a";
			if(i < 10)
				name += '0';
			name += std::to_string(i) + ".txt";
			//std::cout << name << "\n";
			maps.emplace_back(name);
			//maps.emplace_back(tileset, 16, 16, Indexer(&grid, displayIndex, 0), HOLDING);
		}

		//Load base tile map
		dungeon = new TileMap(tileset, 16, 16, new Indexer(&grid, displayIndex, 0), DIETOP);
		dungeon->setScale(6, 6);
		UpdateList::addNode(dungeon);

		GridMaker startGrid("res/dice/map_start.txt");
		Indexer startIndex(&startGrid, displayIndex, 0);
		overlayGrid(&startIndex, 28, 28);
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

	/*const sf::Texture &getTexture(int index) {
		return *(maps[index].getTexture());
	}*/
	Indexer *getDie(int index) {
		return new Indexer(&maps[index], displayIndex, 0);
	}

	Indexer *getCollision() {
		return new Indexer(&grid, collisionIndex, FLOOR, 96, 96);
	}

	void overlayGrid(Indexer *index, unsigned int x, unsigned int y) {
		GridMaker *grid = &(this->grid);
		index->mapGrid([x, y, grid](char c, sf::Vector2f pos) {
			grid->setTile(x + pos.x, y + pos.y, c);
		});
		dungeon->reload();
	}
};