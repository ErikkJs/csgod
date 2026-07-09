#include "HitBox.h"

#ifndef BAT_H
#define BAT_H

class Bat{
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
        bool shiny;
        
        Bat();
        void draw();
		void update();
		~Bat(){
			delete hitBox;
			delete boundingBox;
		}
};

#endif
