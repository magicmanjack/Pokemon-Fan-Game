#include "player.h"
#include "transform.h"
#include <cmath>

#define PI 3.14159265


Player::Player(double spawnX, double spawnY, int startLayer) {
    x = spawnX;
    y = spawnY;
	z = 0.0;
	scale = 1; // Default value.
	targetX = spawnX;
	targetY = spawnY;
    w = 20;
    h = 30;
    walkSpeed = 2;
    layer = startLayer;
}

void Player::setWalkLocation(double targetX, double targetY) {
	Player::targetX = targetX;
	Player::targetY = targetY;
}

void Player::update() {

	if(targetX > x) {
		if(x + walkSpeed > targetX) {
			x += targetX - x;
		} else {
			x += walkSpeed;
		}
	}
	if(targetX < x) {
		if(x - walkSpeed < targetX) {
			x -= x - targetX;
		} else {
			x -= walkSpeed;
		}
	}
	if(targetY > y) {
		if(y + walkSpeed > targetY) {
			y += targetY - y;
		} else {
			y += walkSpeed;
		}
	}
	if(targetY < y) {
		if(y - walkSpeed < targetY) {
			y -= y - targetY;
		} else {
			y -= walkSpeed;
		}
	}
    
}

void Player::render(SDL_Renderer* rr) {
    SDL_SetRenderDrawColor(rr, 0xFF, 0x00, 0x00, 0xFF);
    double location[] = {x * scale, y * scale, z};
    Transform::transform(location);
    Transform::offset(location);
    SDL_Rect rect;
    rect.x = round(location[0] - (w * scale/2));
    rect.y = round(location[1] - (h * scale));
    rect.w = round(w * scale);
    rect.h = round(h * scale);
    SDL_RenderFillRect(rr, &rect);
    SDL_RenderDrawPoint(rr, location[0], location[1]);
}