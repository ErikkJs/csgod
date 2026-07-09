#ifndef EXPLOSION_H
#define EXPLOSION_H

class Explosion{
    public:
	    float centerX;
	    float centerY;
	    float height;
	    float width;
        int spriteSheetIndex;
        int frame;
        bool done;
        // struct timespec animationTime;
		// float animationSpeedFactor;
      
        Explosion(float, float);
        void draw();
};

#endif