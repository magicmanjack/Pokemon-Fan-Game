#include <SDL.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <ratio>
#include <chrono>

#define PI 3.14159265
#define MAGIC_ANGLE (90.0 - 35.264)

const int w_width = 800, w_height = 600, desired_fps = 60;
SDL_Window * window;
SDL_Renderer * rr;
bool closed = false;

void update() {
	SDL_Event event;
	if(SDL_PollEvent(&event)) {
		if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
			closed = true;
		}
	}	
}

void transform(int in[3]) {
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

void render() {
	int spacing = 50;
	for(int ix = 0; ix < 10; ix++) {
		for(int iy = 0; iy < 10; iy++) {
			int point[3] = {ix * spacing, iy * spacing, 0};
			transform(point);
			SDL_SetRenderDrawColor(rr, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(rr, point[0], point[1]);
		}
	}
}

int main(int argc, char** argv) {
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		std::cout << "COULD NOT INITIALIZE SDL!" << std::endl;
	}
	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_width, w_height, 0);
	rr = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
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