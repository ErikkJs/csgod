#include "HitBox.h"

#ifndef ENEMY1_H
#define ENEMY1_H

class Enemy1{
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
		HitBox *boundingBox;
        struct timespec walkTime;
		float animationSpeedFactor;
      
        Enemy1();
        void draw();
		void update();
		~Enemy1(){
			delete hitBox;
			delete boundingBox;
		}
};

#endif
