#include "Skyrmion/core/UpdateList.h"
#include "Skyrmion/input/Settings.h"
#include "spawners.h"

void initialize() {
	Settings::loadSettings("res/settings.json");
	srand(time(NULL));

	//Run player setup
	spawnPlayer();

	//Lock UI Layers
	UpdateList::globalLayer(DIETOP);
	UpdateList::globalLayer(BORDER);
	UpdateList::globalLayer(HOLDING);
	UpdateList::globalLayer(PLAYER);
	UpdateList::globalLayer(INPUT);

	//Finish engine setup
	UpdateList::startEngine();
}

std::string TITLE_STR = "Rolling Labyrinth";
std::string *windowTitle() {
	return &TITLE_STR;
}

skColor backgroundColor() {
	return skColor(0,0,0);
}

std::vector<std::string> &textureFiles() {
	return TEXTURE_FILES;
}
std::vector<std::string> &layerNames() {
	return LAYER_NAMES;
}

void recieveNetworkString(std::string data, int code) {
	std::cout << "NETWORK: Received string " << data << "\n";
}