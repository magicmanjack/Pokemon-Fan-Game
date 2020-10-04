#include "map.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <SDL.h>

Map::Map(const char* fileName) {
	std::ifstream mapFile;
	mapFile.open(fileName);
	if(mapFile.is_open()) {
		std::string lineString;
		for(int iy = 0; std::getline(mapFile, lineString); iy++) {
			std::vector<int> line;
			for(int ix = 0; ix < lineString.length(); ix++) {
				line.push_back((int)lineString[ix]);
			}
			tileValues.push_back(line);
		}
	} else {
		std::cout << "couldn't open map file: " << fileName << std::endl;
	}
	mapFile.close();
}

void Map::loadTextures(const char* fileName) {

}

int Map::getTileAt(int x, int y) {
	return tileValues.at(y).at(x);
}

void Map::setTileAt(int x, int y, int i) {
	tileValues.at(y).at(x) = i;
}

void Map::render(SDL_Renderer* rr) {
	
}

