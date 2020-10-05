#include "map.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <SDL.h>

#define TILE_PIXEL_W 32
#define TILE_PIXEL_H 32
#define TILE_DRAWING_WIDTH 64
#define PI 3.14159265
#define MAGIC_ANGLE (90.0 - 35.264)

Map::Map(const char* fileName) {
	enableGrid = false; // Grid is disabled by default.
	offsetX = 400;
	offsetY = 100;
	
	std::ifstream mapFile;
	mapFile.open(fileName);
	if(mapFile.is_open()) {
		std::string lineString;
		for(int iy = 0; std::getline(mapFile, lineString); iy++) {
			std::vector<int> line;
			for(int ix = 0; ix < lineString.length(); ix++) {
				line.push_back(((int)lineString[ix]) - 32);
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

void Map::transform(int in[3]) {
	double rotateZ[3][3] = {{sin(45.0*PI/180.0), -sin(45.0*PI/180.0), 0.0},
							{cos(45.0*PI/180.0), cos(45.0*PI/180.0), 0.0},
							{0.0, 0.0, 0.0}};
	//^^Rotates vectors 45 degrees around the Z-axis.
	double rotateX[3][3] = {{1.0, 0.0, 0.0},
							{0.0, cos(MAGIC_ANGLE * PI/180.0), -sin(MAGIC_ANGLE * PI/180.0)},
							{0.0, sin(MAGIC_ANGLE * PI/180.0), cos(MAGIC_ANGLE * PI/180.0)}};
	//^^Rotates vectors (90.0 - 35.264) degrees around the X-axis.
	int operation1[3] = {0, 0, 0};
	for(int row = 0; row < 3; row++) {
		for(int column = 0; column < 3; column++) {
			operation1[row] += rotateZ[row][column] * in[column];
		}
	} 
	int operation2[3] = {0, 0, 0};
	for(int row = 0; row < 3; row++) {
		for(int column = 0; column < 3; column++) {
			operation2[row] += rotateX[row][column] * operation1[column];
		}
	}
	std::copy(operation2, operation2 + 3, in);
}

void Map::offset(int in[3]) {
	in[0] += offsetX;
	in[1] += offsetY;
}


void Map::render(SDL_Renderer* rr) {
	for(int ix = 0; ix < getMapTileWidth(); ix++) {
		for(int iy = 0; iy < getMapTileHeight(); iy++) {
			int point[3] = {ix * TILE_DRAWING_WIDTH, iy * TILE_DRAWING_WIDTH, 0};
			transform(point);
			offset(point);
			SDL_Rect tileRect;
			tileRect.x = point[0] - round(tan(60.0 * PI/180.0) * sin(35.264 * PI/180.0) * sin(45.0 * PI/180.0) * TILE_DRAWING_WIDTH);
			tileRect.y = point[1] - round(sin(35.264 * PI/180.0) * sin(45.0 * PI/180.0) * TILE_DRAWING_WIDTH * 2);
			tileRect.w = round(tan(60.0 * PI/180.0) * sin(35.264 * PI/180.0) * sin(45.0 * PI/180.0) * TILE_DRAWING_WIDTH * 2);
			tileRect.h = round(sin(35.264 * PI/180.0) * sin(45.0 * PI/180.0) * TILE_DRAWING_WIDTH * 4);
			SDL_RenderCopy(rr, textures.at(getTileAt(ix, iy)), NULL, &tileRect);
			if(enableGrid) {
				SDL_RenderCopy(rr, gridTexture, NULL, &tileRect); // Draws grid.
			}
		}
	}
}

