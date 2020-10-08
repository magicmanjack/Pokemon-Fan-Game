#include "player.h"
#include "transform.h"
#include <cmath>

#define PI 3.14159265


Player::Player(double spawnX, double spawnY, int startLayer) {
    x = spawnX;
    y = spawnY;
    w = 32;
    h = 32;
    mUp = false;
    mDown = false;
    mLeft = false;
    mRight = false;
    walkSpeed = 2;
    layer = startLayer;
}

void Player::update() {
    if(mUp) {
        y -= walkSpeed;
        x -= walkSpeed;
    }
    if(mDown) {
        y += walkSpeed;
        x += walkSpeed;
    }
    if(mLeft) {
        y += walkSpeed;
        x -= walkSpeed;
    }

    if(mRight) {
        y -= walkSpeed;
        x += walkSpeed;
    }
}

void Player::render(SDL_Renderer* rr) {
    SDL_SetRenderDrawColor(rr, 0xFF, 0x00, 0x00, 0xFF);
    double location[] = {x, y, 0};
    Transform::transform(location);
    Transform::offset(location);
    SDL_Rect rect;
    rect.x = round(location[0] - (w/2));
    rect.y = round(location[1] - h);
    rect.w = round(w);
    rect.h = round(h);
    SDL_RenderFillRect(rr, &rect);
    SDL_RenderDrawPoint(rr, location[0], location[1]);
}
