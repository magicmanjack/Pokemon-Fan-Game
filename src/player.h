#include <SDL2/SDL.h>

#ifndef _PLAYER_H_DEFINED_
#define _PLAYER_H_DEFINED_

class Player {
    public:
        double x;
        double y;
        double w;
        double h;
        bool mUp;
        bool mDown;
        bool mLeft;
        bool mRight;
        int walkSpeed;
        int layer; // The layer that the player starts on.
        //^^The dimensions and location of the player.
        Player(double spawnX, double spawnY, int startLayer);
        void update();
        void render(SDL_Renderer* rr);
};

#endif
