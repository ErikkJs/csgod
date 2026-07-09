#ifndef BULLET_H
#define BULLET_H

class Bullet {

    public: 

	float pos[3];
	float vel[3]; 
	float color[3];
	float width; 
	float height;
	float velocityValue;
	struct timespec time;
	
	void draw();
	~Bullet();
	Bullet(); 
};

#endif
	
	
