#include "HitBox.h"

#ifndef POWERUP_H
#define POWERUP_H

class PowerUp{
    public:
	    float centerX;
	    float centerY;
	    float height;
	    float width;
        int spriteSheetIndex;
        int frame;
        bool done;
        float velocityX;
        float velocityY;
        HitBox *hitBox;
  
        // struct timespec animationTime;
		// float animationSpeedFactor;        
        PowerUp(float, float, float, float, int);
       /* ~PowerUp() {
            delete hitBox;
        }*/
        void draw();
        void update();
};

#endif