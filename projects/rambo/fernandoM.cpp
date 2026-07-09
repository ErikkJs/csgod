/*
Author: Fernando Montes De Oca
First Modified Date: February 23, 2018
Last Modified Date: May 6, 2018
*/

//includes
#include <X11/keysym.h>
#include <sstream>
#include <GL/glx.h>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include "Global.h"
#include "Timers.h"
#include "SpriteSheet.h"
#include "Character.h"
#include "fernandoM.h"
#include "Bullet.h" 
#include "fonts.h"
#include "Bat.h"
#include "PowerUp.h"
#include "HitBox.h"
#include <stdio.h>  
#include <stdlib.h>     
#include <time.h>  
#include "joshuaC.h"
#include "Enemy1.h"

//extern variables
extern int flipped;
extern float cx;
extern Timers timers;
extern Global g;
extern int keys[];
extern Character rambo;
extern SpriteSheet img[];
extern int nbullets;
extern int MAX_ENEMIES;
extern int nPirates;
extern int MAX_BATS;
extern int nBats;
extern Bat* bats;
extern std::vector<PowerUp> powerUps;
extern bool display_hitbox;
extern float gravity;
extern void sound(int select);
extern Enemy1* pirates;
extern Enemy1* juggernauts;
extern int nJuggernauts;

using namespace std; 

//constants 
const int MAX_BULLETS = 30;
float fireRate = 0.35;	
struct timespec newHeadshotTimer;
double seconds = 0;
double headshotSeconds = 10.0;

//Bullet Constructor
Bullet::Bullet() 
{
    velocityValue = 10;
    //define starter values
    pos[0] = rambo.getCenterX();
    pos[1] = rambo.getCenterY();
    vel[0] = 0;
    vel[1] = 0;
    height = 5.0;
    width = height;
}

//Bullet Destructor
Bullet::~Bullet() 
{
}

void Bullet::draw() 
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2i(pos[0] - (width / 2), pos[1] + (height/2));
    glVertex2i(pos[0] + (width / 2), pos[1] + (height/2));
    glVertex2i(pos[0] + (width / 2), pos[1] - (height/2));
    glVertex2i(pos[0] - (width / 2), pos[1] - (height/2));
    glEnd();
}

void shootFaster() {

    if (fireRate > 0.09) {
		fireRate = fireRate - 0.07;
    }
}
//make headshot a state in character

void headShot() {
	clock_gettime(CLOCK_REALTIME, &newHeadshotTimer);	
	rambo.headshot = true;
}
void spaceButton() {

    if (keys[g.shootingKey]) {
		struct timespec newBT;
		clock_gettime(CLOCK_REALTIME, &newBT);
		double seconds = timers.timeDiff(&g.bulletTimer, &newBT);

    	if (seconds > fireRate) {
	    	timers.timeCopy(&g.bulletTimer, &newBT);
	    	if (nbullets < MAX_BULLETS) {   
			//shoot a bullet...
			Bullet *b = &g.ramboBullets[nbullets];
			b->pos[1] = rambo.boundingBox->getBottom() + rambo.height;

				if (rambo.angleUp) { 
					b->pos[1] = rambo.boundingBox->getTop();
					b->vel[0] = b->velocityValue;
					b->vel[1] = b->velocityValue;
				} else if (rambo.angleDown) { 
					if (rambo.flipped) {
						b->pos[0] = rambo.boundingBox->getLeft();
						b->pos[1] = rambo.boundingBox->getBottom() + 30;
					} else { 
						b->pos[0] = rambo.boundingBox->getRight();
						b->pos[1] = rambo.boundingBox->getBottom() + 30;
					}
					b->vel[0] = b->velocityValue;
					b->vel[1] = -b->velocityValue;
				} else if (rambo.shootingStraight) {
					b->vel[0] = b->velocityValue;
					b->vel[1] = 0;
				} else if (rambo.aimUp) {
					b->pos[1] = rambo.boundingBox->getTop();
					b->vel[0] = 0;
					b->vel[1] = b->velocityValue;
				} else if (rambo.jumping) {
					b->pos[0] = rambo.getCenterX();
					
					if (keys[XK_Left]) {
						if (keys[XK_Up]) {
							b->vel[0] = b->velocityValue;
							b->vel[1] = b->velocityValue;
						} else if (keys[XK_Down]) { 
							b->vel[0] = b->velocityValue;
							b->vel[1] = -b->velocityValue;
						} else {//shoot straight
							b->vel[0] = b->velocityValue;
							b->vel[1] = 0;
						}
					} else if (keys[XK_Right]) {
						if (keys[XK_Up]) { 
							b->vel[0] = b->velocityValue;
							b->vel[1] = b->velocityValue;
						} else if (keys[XK_Down]) {
							b->vel[0] = b->velocityValue;
							b->vel[1] = -b->velocityValue;
						} else { 
							//shoot straight
							b->vel[0] = b->velocityValue;
							b->vel[1] = 0;
						}
					} else if (keys[XK_Up]) {
						b->vel[0] = 0;
						b->vel[1] = b->velocityValue;
					} else if (keys[XK_Down]) { 
						b->vel[0] = 0;
						b->vel[1] = -b->velocityValue;
					} else {
						b->vel[0] = b->velocityValue;
						b->vel[1] = 0;
					}
				} else { 
					b->vel[0] = b->velocityValue;
					b->vel[1] = 0;
				}
				if (rambo.flipped) {
					b->pos[0] = rambo.boundingBox->getLeft();
					if (!(b->vel[0] < 0.0)) {        
						b->vel[0] *= -1;
					}            
					if (rambo.aimUp) {
						b->pos[0] = rambo.boundingBox->getLeft() + 11;
					} else if (rambo.prone) { 
						b->pos[1] = rambo.boundingBox->getBottom() + 22;
					} else if (rambo.angleUp) {
						b->pos[0] = rambo.boundingBox->getLeft() + 12;
					}
				} else {
					b->pos[0] = rambo.boundingBox->getRight();
					if ((b->vel[0] < 0.0)) {
						b->vel[0] *= -1;
					}
					if (rambo.aimUp) {
						b->pos[0] = rambo.boundingBox->getRight() - 9;
					} else if (rambo.prone) { 
						b->pos[1] = rambo.boundingBox->getBottom() + 22;
					} else if (rambo.angleUp) {
						b->pos[0] = rambo.boundingBox->getRight() - 12;
					}
				}        
			b->color[0] = 1.0f;
			b->color[1] = 1.0f;
			b->color[2] = 1.0f; 
			nbullets++;
			sound(2);
	    	}
		}
    }	
}

void fernandoPhysics() 
{
    int i = 0;
    //Update bullet positions
    while (i < nbullets) {
		Bullet *b = &g.ramboBullets[i];
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];

		//Check for collision with window edges and deleting if so
		if (b->pos[0] < 0.0) {
			memcpy(&g.ramboBullets[i], &g.ramboBullets[nbullets-1], sizeof(Bullet));
			nbullets--;
		} else if (b->pos[0] > (float)g.xres) {
			memcpy(&g.ramboBullets[i], &g.ramboBullets[nbullets-1], sizeof(Bullet));
			nbullets--;
		} else if (b->pos[1] < 0.0) {
			memcpy(&g.ramboBullets[i], &g.ramboBullets[nbullets-1], sizeof(Bullet));
			nbullets--;
		} else if (b->pos[1] > (float)g.yres) {
			memcpy(&g.ramboBullets[i], &g.ramboBullets[nbullets-1], sizeof(Bullet));
			nbullets--;
		}
		i++;
    }  

    for (unsigned int j = 0; j < powerUps.size(); j++) {
		if (powerUps[j].hitBox->getLeft() <= rambo.hitBox->getRight() &&
			powerUps[j].hitBox->getRight() >= rambo.hitBox->getLeft() &&
			powerUps[j].hitBox->getTop() >= rambo.hitBox->getBottom() &&
			powerUps[j].hitBox->getBottom() <= rambo.hitBox->getTop()) {


	    	if ( powerUps[j].frame == 1) {
				if (rambo.velocityXStrength < 8) {
		    		rambo.velocityXStrength++;
		 			sound(5);
				}
	    	} else if ( powerUps[j].frame == 2) {
				for(int i = 0; i < nBats; i++) {
		    		createExplosion(bats[i].centerX,bats[i].centerY);
				}
				for(int i = 0; i < nPirates; i++) {
		    		createExplosion(pirates[i].centerX,pirates[i].centerY);
				}
				for(int i = 0; i < nPirates; i++) {
		    		createExplosion(juggernauts[i].centerX,juggernauts[i].centerY);
				}
				g.score = g.score + (nBats * 25) + (nPirates * 50) + (nJuggernauts * 100) + 100;
				nBats = 0;
				nPirates = 0;
				nJuggernauts = 0;
				sound(4);
	    	} else if ( powerUps[j].frame == 3 ) { 
				shootFaster();
				sound(5);
	   	 	} else if( powerUps[j].frame == 4 ) { 

				if (rambo.health < 4) {
		    		rambo.health++;
		    		sound(6);
				}
	    	} else if ( powerUps[j].frame == 0 ) { 
				headShot();
	    	}
	   		 else {
				shootFaster();
	    	}
	    	powerUps[j].done = true;
		}  
    }
    //update powerUp position
    for (unsigned int i = 0; i < powerUps.size(); i++) {

		if (powerUps[i].centerY > 60) {
	    	powerUps[i].centerY += gravity;
		}
    }

	if (rambo.headshot) {
		seconds = timers.timeDiff(&newHeadshotTimer, &timers.timeCurrent);
		if (seconds > headshotSeconds) {
			rambo.headshot = false;
		}
	}
}

void deleteBullet(int n) 
{
    memcpy(&g.ramboBullets[n], &g.ramboBullets[nbullets-1], sizeof(Bullet));
    nbullets--;          
}

void renderTheBackground() {
    //render background//
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.RamboTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255, 255, 255, 255);

    float ssWidth = (float)1.0 / img[2].columns;
    float ssHeight = (float)1.0 / img[2].rows;

    float textureX = 0;
    float textureY = 0;

    float centerX = g.xres / 2;
    float centerY = (g.yres / 2) + g.yres*0.20833;

    float width = img[2].width/1.4;
    float height = img[2].height/1.5;

    glBegin(GL_QUADS);
    glTexCoord2f(textureX, textureY + ssHeight);
    glVertex2i(centerX - width, centerY - height);

    glTexCoord2f(textureX, textureY);
    glVertex2i(centerX - width, centerY + height);

    glTexCoord2f(textureX + ssWidth, textureY);
    glVertex2i(centerX + width, centerY + height);

    glTexCoord2f(textureX + ssWidth, textureY + ssHeight);
    glVertex2i(centerX + width, centerY - height);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
    //render background end
}

PowerUp::PowerUp(float x, float y, float velX, float velY, int index)
{
    centerX = x;
    centerY = y;
    velocityX = velX;
    velocityY = velY;
    height = .04* (float)g.yres;
    width = height;
    spriteSheetIndex = 11;
    frame = index;
    done = false;
    hitBox = new HitBox(centerY+(height/2),centerY-(height/2),centerX-(width/2),centerX+(height/2));
}

void PowerUp::draw()
{
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.powerUpsTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    float ssWidth = (float)1.0/img[spriteSheetIndex].columns;
    float ssHeight = (float)1.0/img[spriteSheetIndex].rows;

    int ix = this->frame % img[spriteSheetIndex].columns;
    int iy = 0;

    if (frame >= img[spriteSheetIndex].columns) {
		iy = 1;
    }

    if (frame >= (img[spriteSheetIndex].columns*2)) {
		iy = 2;
    }

    if (frame >= (img[spriteSheetIndex].columns*3)) {
		iy = 3;
    }

    if (frame >= img[spriteSheetIndex].columns*4) {
		iy = 4;
    }

    if (frame >= (img[spriteSheetIndex].columns*5)) {
		iy = 5;
    }

    float textureX = (float)ix / img[spriteSheetIndex].columns;
    float textureY = (float)iy / img[spriteSheetIndex].rows;

    glBegin(GL_QUADS);
    glTexCoord2f(textureX, textureY+ssHeight);
    glVertex2i(centerX-width, centerY-height);

    glTexCoord2f(textureX, textureY);
    glVertex2i(centerX-width, centerY+height);

    glTexCoord2f(textureX+ssWidth, textureY);
    glVertex2i(centerX+width, centerY+height);

    glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
    glVertex2i(centerX+width, centerY-height);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);

    if (display_hitbox) {
		hitBox->draw();
    }
    update();
}

void PowerUp::update() {

    hitBox->updateHitBox(centerY + (height / 2),
	    centerY - (height / 2) - (height * .486111), //28
	    centerX,  
	    centerX);     

}
void createPowerUp(float x, float y, float velX, float velY, int index)
{
    srand (time(NULL));

    index = rand() % 5;

	while ((rambo.health == 4 && index == 4) || (rambo.headshot && index == 5) || 
			(fireRate <= 0.09 && index == 3) || (rambo.velocityXStrength >= 8 && index == 1)) {
		index = rand() % 5;
	}

    //create temporary powerUp
    PowerUp temp(x, y, velX, velY, index);

    //push temp powerUp to actual
    powerUps.push_back(temp);


}

void cleanPowerUps()
{
    //must loop backwards since size is changing
    for (int x = powerUps.size()-1; x >= 0; x--) {
		if (powerUps[x].done) {
	   		powerUps.erase(powerUps.begin() + x);
		}
    }
}

double printGroupNumber() 
{
    static double td = 0.0; 
#ifdef PROFILING
    //////////////////////////////////////////////
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    Rect r;
    r.center = 0;
    r.bot = g.yres - 200;
    r.left = g.xres/4;

    for (int i = 0; i < 100000; i++) {
		r.center = 0;
		for (int j = 0; j < 10000000; j++) {
			cout << "ok" << endl;
			for (int k = 0; k < 10000000; k++) {
				cout << "ok" << endl;
			}
		}
    }

    ggprint8b(&r, 16, 0xffff47, "Group 2");

    clock_gettime(CLOCK_REALTIME, &end);
    td += timers.timeDiff(&start, &end);
    //////////////////////////////////////////////////
#endif
    return td;


}
double printGroupNumberOpt(){

    static double td = 0.0;
#ifdef PROFILING
    //////////////////////////////////////////////
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    Rect r;
    r.bot = g.yres << 2;
    r.bot = r.bot >> 2;
    r.left = g.xres >> 2;
    r.center = 0;
    ggprint8b(&r, 16, 0xffff47, "Group 2");

    clock_gettime(CLOCK_REALTIME, &end);
    td += timers.timeDiff(&start, &end);
    //////////////////////////////////////////////////
#endif
    return td;

}

void printFernandoM(int size, int color) 
{
    Rect r;
    r.bot = g.yres - 300;
    r.left = 30;
    r.center = 0;
    ggprint8b(&r, size, color, "Fernando Montes De Oca");
    ggprint8b(&r, size, color, "GROUP NAME: %lf",printGroupNumber());
    ggprint8b(&r, size, color, "GROUP NAME OPT: %lf",printGroupNumberOpt());
}
