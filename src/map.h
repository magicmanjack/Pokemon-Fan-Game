#include "player.h"
#include <SDL2/SDL.h>
#include <vector>

#ifndef _MAP_H_INCLUDED_
#define _MAP_H_INCLUDED_

class Map {
	public:
        static Player* player;
		std::vector<std::vector<int>> tileValues; // The map in a 2D vector. The outer vector stores the rows (the inner vectors).
		std::vector<SDL_Texture*> textures; // A vector full of textures. Each one maps to a specific tile index.
		SDL_Texture* gridTexture;
		bool enableGrid;
        int scale; // The scale that the map gets rendered at.
        int layer;
		Map(const char* fileName, int mapLayer); // Takes in the filename of the map to be loaded.
		void loadTextures(SDL_Renderer* rr, const char* fileName);
		int getTileAt(int x, int y);
		void setTileAt(int x, int y, int i);
		int getMapTileWidth();
		int getMapTileHeight();
		void render(SDL_Renderer* rr);
	
};

#endif
