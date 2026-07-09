#include "HitBox.h"

#ifndef HULK_H
#define HULK_H

class Hulk{
    public:
	    float centerX;
	    float centerY;
	    float height;
	    float width;
	    int frame;
	    bool flipped;
        bool jumping;
	    int health;
        int spriteSheetIndex;
        float velocityX;
        float velocityY;
		HitBox *hitBox;
        struct timespec walkTime;
		float animationSpeedFactor;
      
        Hulk();
        void draw();
		void update();
		void checkJumping();
		~Hulk(){
			delete hitBox;
		}
};

#endif
