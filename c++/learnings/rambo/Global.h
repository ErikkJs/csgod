#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "Bullet.h"

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt     Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)     ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]

//GAME STATES
#define MAINMENU 0
#define INGAME 1
#define PAUSEMENU 2
#define DEATH 3

//OPTIONS
#define NEWGAME 0
#define LEADERBOARD 1
#define EXIT 2
#define RESUMEGAME 3

//CONSTANTS
#define JUMP_STRENGTH 14
#define PI 3.1415

class Global{
    public:
	int done;
    unsigned long score;
	int xres, yres;
	int walk;
	int walkFrame;
	Flt x[2];
	Flt y[2];
	Flt camera[2];
	double delay;
	double drawWeaponDelay;
	struct timespec bulletTimer;
	struct timespec headshotTimer;
	Bullet *ramboBullets;
	GLuint walkTexture;
	GLuint ramboLogoTexture;
	GLuint powerUpTexture;
	GLuint tempBackgroundTexture;
	GLuint RamboTexture;
	GLuint spacePirateTexture;
	GLuint batEnemyTexture;
	GLuint batEnemyShinyTexture;
	GLuint explosionTexture;
	GLuint cursorTexture;
	GLuint healthBarTexture;
	GLuint enemyHealthBarTexture;
	GLuint powerUpsTexture;
	GLuint hulkTexture;
	GLuint wasted;
	GLuint platform;
    Vec box[20];
	int shootingKey;
	Global();
};
#endif

