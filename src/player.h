#include <SDL.h>

#ifndef _PLAYER_H_DEFINED_
#define _PLAYER_H_DEFINED_

class Player {
    public:
        double x;
        double y;
        double w;
        double h;
		double targetX;
		double targetY;
        int walkSpeed;
        int layer; // The layer that the player starts on.
        //^^The dimensions and location of the player.
        Player(double spawnX, double spawnY, int startLayer);
		void setWalkLocation(double targetX, double targetY);
        void update();
        void render(SDL_Renderer* rr);
};

#endif
