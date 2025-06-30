#include "Skyrmion/tiling/TileMap.hpp"
#include "spawners.h"

#define MAPCOUNT 24
#define DIEWIDTH 112
#define PERMUTATIONS 720
#define ROTATIONS 4096
#define GRIDSCALE 4
#define GRIDSIZE 16*4
#define GRIDWIDTH 11
#define STARTROOM 5*7+1

class DiceCollection {
private:
	std::vector<GridMaker> maps;
	GridMaker grid;
	TileMap *dungeon;

public:
	DiceCollection() : grid(GRIDWIDTH*7+2, GRIDWIDTH*7+2) {
		maps.reserve(MAPCOUNT);
		for(int i = 1; i <= MAPCOUNT; i++) {
			std::string name = "res/dice/map_a";
			if(i < 10)
				name += '0';
			name += std::to_string(i) + ".txt";
			maps.emplace_back(name);
			//maps.emplace_back(tileset, 16, 16, MapIndexer(&grid, displayIndex, 0), HOLDING);
		}

		//Load base tile map
		dungeon = new TileMap(tilesTexture, 0, 16, 16, new MapIndexer(&grid, displayIndex, 0), DIETOP);
		dungeon->setScale(GRIDSCALE, GRIDSCALE);
		UpdateList::addNode(dungeon);

		rebuildMap();
	}

	void rebuildMap() {
		grid.clearTiles();

		//Create grid border
		int width = grid.getSize().x;
		int height = grid.getSize().y;
		for(int x = 0; x < width; x++) {
			grid.setTileI(x, 0, '&');
			grid.setTileI(x, height-1, '&');
		}
		for(int y = 0; y < height; y++) {
			grid.setTileI(0, y, '&');
			grid.setTileI(width-1, y, '&');
		}

		//Add starter room
		GridMaker startGrid("res/dice/map_start.txt");
		overlayGrid(&startGrid, STARTROOM, STARTROOM);

		//Find location on edge
		int y = getNext(GRIDWIDTH);
		int x = getNext(GRIDWIDTH);
		switch(getNext(4)) {
			case 0:
				y = 0;
				break;
			case 1:
				y = GRIDWIDTH - 1;
				break;
			case 2:
				x = 0;
				break;
			case 3:
				x = GRIDWIDTH - 1;
				break;
		}

		//Add exit room
		GridMaker endGrid("res/dice/map_end.txt");
		overlayGrid(&endGrid, (x*7)+1, (y*7)+1);
	}

	int getNext(int size) {
		return rand() % size;
	}

	int getPerm(int order, int rotation, int index) {
		int perm[6] = {5, 4, 3, 2, 1, 0};
		rotation *= 4;
		index %= 6;

		for(int i = 0; i < index + 1; i++) {
			int j = i + order % (6-i);
			int s = perm[i];
			perm[i] = perm[j];
			perm[j] = s;

			order /= (6-i);
			rotation /= 4;
		}
		return perm[index] * 4 + rotation % 4;
	}

	/*const sf::Texture &getTexture(int index) {
		return *(maps[index].getTexture());
	}*/
	Indexer *getDie(int index) {
		return &maps[index];
	}

	MapIndexer *getCollision1() {
		return new MapIndexer(&grid, collisionIndex1, FLOOR, GRIDSIZE, GRIDSIZE);
	}

	MapIndexer *getCollision2() {
		return new MapIndexer(&grid, collisionIndex2, FLOOR, GRIDSIZE, GRIDSIZE);
	}

	void overlayGrid(Indexer *index, unsigned int x, unsigned int y) {
		GridMaker *grid = &(this->grid);
		index->mapGrid([x, y, grid](int c, Vector2f pos) {
			if(c != '/')
				grid->setTileI(x + pos.x, y + pos.y, c);
		});
		dungeon->reload();
	}
};