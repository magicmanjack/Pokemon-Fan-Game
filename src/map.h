#include <SDL.h>
#include <vector>

#ifndef _MAP_H_INCLUDED_
#define _MAP_H_INCLUDED_

class Map {
	public:
		std::vector<std::vector<int>> tileValues; // The map in a 2D vector. The outer vector stores the rows (the inner vectors).
		std::vector<SDL_Texture> textures; // A vector full of textures. Each one maps to a specific tile index.
		Map(const char* fileName); // Takes in the filename of the map to be loaded.
		void loadTextures(const char* fileName);
		int getTileAt(int x, int y);
		void setTileAt(int x, int y, int i);
		void render(SDL_Renderer* rr);
	
};

#endif