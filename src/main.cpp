#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <ratio>
#include <chrono>
#include "map.h"

const int w_width = 800, w_height = 600, desired_fps = 60;
SDL_Window * window;
SDL_Renderer * rr;
bool closed = false; // Set to true once the window is closed.

Map* map;

void update() {
	SDL_Event event;
	if(SDL_PollEvent(&event)) {
		if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
			closed = true;
		}
		if(event.type == SDL_KEYDOWN) {
			if(event.key.keysym.sym == SDLK_BACKQUOTE) {
				map->enableGrid = !map->enableGrid;
			}
		}
	}	
}

void render() {
	map->render(rr);
}

int main(int argc, char** argv) {
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		std::cout << "COULD NOT INITIALIZE SDL!" << std::endl;
	}
	
	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_width, w_height, 0);
	rr = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//^^Window and renderer created.
	
	map = new Map("src/test.txt");
	map -> loadTextures(rr, "res/test_tile_set.bmp");
	
	using namespace std::chrono;
	
	high_resolution_clock::time_point now = high_resolution_clock::now(), then = now, timer = now;
	double millisPerUpdate = 1000.0 / desired_fps;
	double updatesQueued = 0.0;
	bool canRender = false;
	
	int updates = 0, frames = 0;
	
	while(!closed) {
		now = high_resolution_clock::now();
		duration<double, std::milli> timePassed = now - then;
		updatesQueued += timePassed.count() / millisPerUpdate;
		then = now;
		
		while(updatesQueued > 0) {
			update();
			updatesQueued--;
			updates++;
			canRender = true;
		}
		
		if(canRender) {
			SDL_SetRenderDrawColor(rr, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(rr);
			//Clears the screen.
			render();
			SDL_RenderPresent(rr);
			frames++;
			canRender = false;
		}
		
		duration<double, std::milli> timerDuration = now - timer;
		if(timerDuration.count() >= 5000.0) {
			timer = now;
			std::cout << "Updates: " << updates / 5 << ", Frames: " << frames / 5 << std::endl;
			updates = 0;
			frames = 0;
		}
	}
	
	SDL_Quit();
	
	return 0;
}
