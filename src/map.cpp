#include "map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <SDL.h>
#include "transform.h"

#define PI 3.14159265
#define TILE_PIXEL_W 32
#define TILE_PIXEL_H 32
#define TILE_WIDTH_NO_TRANSFORM 22.627417

Player* Map::player = new Player(TILE_WIDTH_NO_TRANSFORM / 2, TILE_WIDTH_NO_TRANSFORM / 2, 1); // Static player is shared by all instances of map. This is for layering.

Map::Map(const char* fileName, int mapLayer) {
	enableGrid = false; // Grid is disabled by default.
    Transform::offsetX = 400;
    Transform::offsetY = 0;
    Transform::offsetZ = 0;
    //^^Starts at the top right corner of the map.

    scale = 2; // The scale is 2x by default.
	player->scale = scale;
    layer = mapLayer;    

	std::ifstream mapFile;
	mapFile.open(fileName);
	if(mapFile.is_open()) {
		std::string lineString;
		for(int iy = 0; std::getline(mapFile, lineString); iy++) {
			std::vector<int> line;
            std::stringstream sStream(lineString);
            std::string tileString;
			while(std::getline(sStream, tileString, ',')) {
                line.push_back(std::stoi(tileString));
            }
			tileValues.push_back(line);
		}
	} else {
		std::cout << "couldn't open map file: " << fileName << std::endl;
	}
	mapFile.close();
}

void Map::loadTextures(SDL_Renderer* rr, const char* fileName) {

	SDL_Surface* surface;
	surface = SDL_LoadBMP("res/tile.bmp");
	SDL_SetColorKey(surface, true, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
	gridTexture = SDL_CreateTextureFromSurface(rr, surface);
	SDL_FreeSurface(surface);
	//^^ Grid texture gets loaded.
		
	SDL_Surface* masterSurface = SDL_LoadBMP(fileName);
	
	int tilesWide = (masterSurface -> w) / TILE_PIXEL_W;
	int tilesHigh = (masterSurface -> h) / TILE_PIXEL_H;
	
	for(int iy = 0; iy < tilesHigh; iy++) {
		for(int ix = 0; ix < tilesWide; ix++) {
			SDL_Rect tileRect;
			tileRect.x = ix * TILE_PIXEL_W;
			tileRect.y = iy * TILE_PIXEL_H;
			tileRect.w = TILE_PIXEL_W;
			tileRect.h = TILE_PIXEL_H;
			SDL_Surface* tileSurface;
			tileSurface = SDL_CreateRGBSurface(0, TILE_PIXEL_W, TILE_PIXEL_H, 24, 0, 0, 0, 0);
			SDL_BlitSurface(masterSurface, &tileRect, tileSurface, NULL);
			SDL_SetColorKey(tileSurface, true, SDL_MapRGB(tileSurface->format, 0x00, 0x00, 0x00));
			textures.push_back(SDL_CreateTextureFromSurface(rr, tileSurface));
		}
	}
}

int Map::getMapTileWidth() {
	return tileValues.at(0).size();
}

int Map::getMapTileHeight() {
	return tileValues.size();
}

int Map::getTileAt(int x, int y) {
	return tileValues.at(y).at(x);
}

void Map::setTileAt(int x, int y, int i) {
	tileValues.at(y).at(x) = i;
}

void Map::render(SDL_Renderer* rr) {
	for(int ix = 0; ix < getMapTileWidth(); ix++) {
		for(int iy = 0; iy < getMapTileHeight(); iy++) {
			double point[3] = {ix * TILE_WIDTH_NO_TRANSFORM * scale, iy * TILE_WIDTH_NO_TRANSFORM * scale, 0};
			Transform::transform(point);
			Transform::offset(point);
			SDL_Rect tileRect;
			tileRect.x = round(point[0] - (cos(45.0 * PI/180.0) * TILE_WIDTH_NO_TRANSFORM * scale));
			tileRect.y = round(point[1] - (cos(45.0 * PI/180.0) * sin(30.0 * PI/180.0) * TILE_WIDTH_NO_TRANSFORM * 2.0 * scale));
			tileRect.w = round(cos(45.0 * PI/180.0) * TILE_WIDTH_NO_TRANSFORM * 2.0 * scale);
			tileRect.h = round(cos(45.0 * PI/180.0) * sin(30.0 * PI/180.0) * TILE_WIDTH_NO_TRANSFORM * 4.0 * scale);
            if(getTileAt(ix, iy) >= 0) { // Prevents trying to load any negative indexes.
			    SDL_RenderCopy(rr, textures.at(getTileAt(ix, iy)), NULL, &tileRect);
            }
            if(player->layer == layer && floor(player->x / (TILE_WIDTH_NO_TRANSFORM)) == ix && floor(player->y / (TILE_WIDTH_NO_TRANSFORM)) == iy) {
                SDL_RenderCopy(rr, gridTexture, NULL, &tileRect); // Draws grid.               
                player->render(rr); // Draws player at whatever tile its on.
            }
			if(enableGrid) {
				SDL_RenderCopy(rr, gridTexture, NULL, &tileRect); // Draws grid.
			}
		}
	}
}

