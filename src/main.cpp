#include <SDL.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <ratio>
#include <chrono>
#include "map.h"

#include "transform.h"

#define TILE_WIDTH_NO_TRANSFORM 22.627417

const int w_width = 800, w_height = 600, desired_fps = 60;
SDL_Window * window;
SDL_Renderer * rr;
bool closed = false; // Set to true once the window is closed.

bool up, down, left, right; // Used to keep track of which keys are pressed. Helps with eight directional movement.

Map* map;
Map* secondLayer;

void update() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
			closed = true;
		}
		if(event.type == SDL_KEYDOWN) {
			if(event.key.keysym.sym == SDLK_BACKQUOTE) {
				map->enableGrid = !map->enableGrid;
			}
            if(event.key.keysym.sym == SDLK_w) {
                up = true;
            }
            if(event.key.keysym.sym == SDLK_s) {
                down = true;
            }
            if(event.key.keysym.sym == SDLK_a) {
                left = true;
            }
            if(event.key.keysym.sym == SDLK_d) {
                right = true;
            }
		}
        if(event.type == SDL_KEYUP) {
            if(event.key.keysym.sym == SDLK_w) {
                up = false;
            }
            if(event.key.keysym.sym == SDLK_s) {
                down = false;
            }
            if(event.key.keysym.sym == SDLK_a) {
                left = false;
            }
            if(event.key.keysym.sym == SDLK_d) {
                right = false;
            }
		}
	}
	if(Map::player->targetX == Map::player->x && Map::player->targetY == Map::player->y) {
		if(left) {
			Map::player->targetX -= TILE_WIDTH_NO_TRANSFORM;
			Map::player->targetY += TILE_WIDTH_NO_TRANSFORM;
		}
		if(right) {
			Map::player->targetX += TILE_WIDTH_NO_TRANSFORM;
			Map::player->targetY -= TILE_WIDTH_NO_TRANSFORM;
		}
		if(up) {
			if(!left) {
				Map::player->targetX -= TILE_WIDTH_NO_TRANSFORM;
			}
			if(!right) {
				Map::player->targetY -= TILE_WIDTH_NO_TRANSFORM;
			}
		}
		if(down) {
			if(!right) {
				Map::player->targetX += TILE_WIDTH_NO_TRANSFORM;
			}
			if(!left) {
				Map::player->targetY += TILE_WIDTH_NO_TRANSFORM;
			}
		}
	}
		
    Map::player->update();
	
    double playerPos[] = {Map::player->x, Map::player->y, 0.0};
    Transform::transform(playerPos);
    Transform::offsetX = (w_width / 2) - playerPos[0];
    Transform::offsetY = (w_height / 2) - playerPos[1];
    //^^Calculating the offset of objects on the screen so that the player remains in the center.	
}

void render() {
	map->render(rr);
    secondLayer->render(rr);
}

int main(int argc, char** argv) {
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		std::cout << "COULD NOT INITIALIZE SDL!" << std::endl;
	}
	
	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_width, w_height, 0);
	rr = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//^^Window and renderer created.
	
	map = new Map("res/test_map.txt", 0);
	map -> loadTextures(rr, "res/test_tile_set.bmp");
    secondLayer = new Map("res/test_map_layer_2.txt", 1);
    secondLayer -> loadTextures(rr, "res/test_tile_set.bmp");
	up = false;
	down = false;
	left = false;
	right = false;
	
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
