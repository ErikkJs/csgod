// Platform boxes class
// Father: CS God
// Author: Erik Soto


#include <math.h>
#include <stdio.h>
#include "SpriteSheet.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "ppm.h"
#include "fonts.h"
#include "PowerUp.h"
#include "Explosion.h"
#include "erikS.h"
#include "kuljitS.h"
#include "Global.h"
#include "Timers.h"
#include "Character.h"
#include "joshuaC.h"
extern Global g;
extern Level lev;
extern Character rambo;
extern int selectedOption;
extern int level;
extern SpriteSheet img[];
extern int keys[];
extern int nPirates;
extern bool hit;
extern int nBats;
extern float fireRate;
extern int nJuggernauts;
extern Timers timers;
extern std::vector<PowerUp> powerUps;
extern std::vector<Explosion> explosions;
extern Platform platforms[];
extern void leader_board();
using namespace std;
bool flag = true;
extern double menuSelectionDelay;
//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
			     (c)[1]=(a)[1]-(b)[1]; \
    (c)[2]=(a)[2]-(b)[2]
void newGame()
{
    fireRate=.35;
    powerUps.clear();
    explosions.clear();
    level=1;
    nPirates =0;
    nBats =0;
    nJuggernauts =0;
    //Constructors
    rambo.centerX = g.xres/2;
    rambo.centerY = 800;
    rambo.height = .08 * (float)g.yres;
    float height =rambo.height;
    rambo.width = height * 0.7;
    //printf("%f\n", height);
    rambo.frame = 0;
    rambo.flipped = false;
    rambo.jumping = false;
    rambo.shooting = false;
    rambo.prone = false;
    rambo.aimUp = false;
    rambo.angleUp = false;
    rambo.angleDown = false;
    rambo.shootingStraight = false;
    rambo.health = 4;
    rambo.headshot = false;
    rambo.velocityX = 0;
    rambo.velocityY = 0;
    rambo.velocityXStrength = 4;
    g.walk=0;
    g.walkFrame =0;
    g.delay = 0.1;
    g.drawWeaponDelay = 0.5;
    g.score =0;
    timers.recordTime(&timers.timeStart);    
    timers.recordTime(&timers.timeEnd);    
    timers.recordTime(&timers.timeCurrent);    
    timers.recordTime(&timers.walkTime);    
    timers.recordTime(&timers.ramboWeaponOutTime);    
    timers.recordTime(&timers.menuSelectionTime);    
    timers.recordTime(&timers.pirateSpawnTime);    
    timers.recordTime(&timers.batSpawnTime);    
    timers.recordTime(&timers.ramboCollisionTime);
    timers.recordTime(&timers.juggernautSpawnTime);
    flag = true;
}

void teleportCheck()
{
    if(rambo.centerX < 0)
    {   
	rambo.centerX = g.xres;
    }
    if(rambo.centerX > g.xres)
    {
	rambo.centerX = 0;
    } 
}

void renderBackground(float s , float sh, float w, float h)
{
    //background image
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, g.RamboTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    float ssWidth = s;
    float ssHeight =sh;

    float textureX = 0;
    float textureY = 0;

    float centerX = g.xres / 2;
    float centerY = (g.yres / 2) + g.yres*0.20833;

    float width = w;
    float height = h;

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

void pauseScreen()
{
    //Renders Pause Screen
    Rect r; 
    float h = 100.0;
    float w = 200.0;
    glPushMatrix();
    glEnable(GL_BLEND);
    glColor4f(0.58,0.58,0.72,0.8);
    glTranslated(g.xres/2, g.yres/2, 0);
    glBegin(GL_QUADS);
    glVertex2i(-w, -h);
    glVertex2i(-w,  h);
    glVertex2i(w,   h);
    glVertex2i(w,  -h);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
    r.bot = g.yres/2 + 80;
    r.left = g.xres/2;
    r.center = 1; 
    ggprint8b(&r, 16, 0, "PAUSE SCREEN");
    r.center = 0;
    r.left = g.xres/2 - 100;
    // start game timer and clear screen with double buffer for pretty transition
    ggprint8b(&r, 16, 0, "Press P - Play");
    // implent with the keys function 
    ggprint8b(&r, 16, 0, "Press E - Exit to Menu");
    ggprint8b(&r, 16, 0, "Press Esc - Exit Game");
}

Platform::Platform(
	float xpos ,float ypos,
	float xpos2 ,float ypos2,
	float xpos3 ,float ypos3,
	float xpos4 ,float ypos4)
{
    // set the ranges for physics
    top = greatest(ypos,ypos2,ypos3,ypos4);
    bottom = least(ypos,ypos2,ypos3,ypos4);

    one.x = xpos;
    one.y = ypos;

    two.x = xpos2;
    two.y = ypos2;

    three.x = xpos3;
    three.y = ypos3;

    four.x = xpos4;
    four.y = ypos4;

    centerx = (xpos + xpos3)/2;
    centery = (ypos + ypos2)/2;
    left = least(xpos,xpos2,xpos3,xpos4);

    right = greatest(xpos,xpos2,xpos3,xpos4);
}

void Platform::drawPlatform()
{
    float width = img[14].width*.6;
    float height = img[14].height*.6;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, g.platform);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(centerx-width, centery-height);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(centerx-width, centery+height);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(centerx+width, centery+height);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(centerx+width, centery-height);
    glEnd();

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Platform::setColor(float r, float g, float b)
{
    set.r =r;
    set.g =g;
    set.b =b;


}

float Platform::greatest(float one,float two,float three,float four)
{
    float g= greatest( one,two);
    float b= greatest(three,four);
    return greatest(g,b);

}

float Platform::greatest(float one,float two)
{
    if(one < two)
    {
	return two;
    }
    else return one;

}
float Platform::least(float one,float two,float three,float four)
{
    float g= least(one,two);
    float b= least(three,four);
    return least(g,b);

}

float Platform::least(float one,float two)
{
    if(one > two)
    {
	return two;
    }
    else return one;

}



class levelGlo {
    public:
	Vec box[20];
	//camera is centered at (0,0) lower-left of screen. 
	Flt camera[2];
	~levelGlo() {

	}
	levelGlo() {

	    camera[0] = camera[1] = 40.0;
	    for (int i=0; i<20; i++) {
		box[i][0] = rnd() * g.xres;
		box[i][1] = rnd() * (g.yres-220) + 220.0;
		box[i][2] = 0.0;
	    }
	    //memset(keys, 0, 65536);
	}
};

Level::Level() {
    for (int i=0; i < 180; i++) {

	dynamicHeight[i] = -1;

    }

    tilesize[0] = 32;

    tilesize[1] = 32;

    ftsz[0] = (Flt)tilesize[0];

    ftsz[1] = (Flt)tilesize[1];

    tile_base = (g.yres/g.yres);

    //read level
    FILE *fpi = fopen("level.txt","r");

    if (fpi) {

	nrows=0;

	char line[700];

	while (fgets(line, 700, fpi) != NULL) {

	    removeCrLf(line);

	    int slen = strlen(line);

	    ncols = slen;
	    for (int j=0; j<slen; j++) {

		arr[nrows][j] = line[j];

	    }

	    ++nrows;

	}

	fclose(fpi);

    }



}

void Level::removeCrLf(char *str) {
    char *p = str;

    while (*p) {

	if (*p == 10 || *p == 13) {

	    *p = '\0';

	    break;

	}

	++p;

    }

}

// need to design the level with text file
void renderlevel(){
    //========================
    //Render the tile system
    //========================
    int tx = lev.tilesize[0];
    int ty = lev.tilesize[1];
    Flt dd = lev.ftsz[0];
    Flt offy = lev.tile_base;
    int ncols_to_render = g.xres / lev.tilesize[0] + 2;
    int col = (int)(g.camera[0] / dd);
    col = col % lev.ncols;

    Flt cdd = g.camera[0] / dd;
    //flo: just the integer portion
    Flt flo = floor(cdd);
    //dec: just the decimal portion
    Flt dec = (cdd - flo);
    //offx: the offset to the left of the screen to start drawing tiles
    Flt offx = -dec * dd;

    for (int j=0; j<ncols_to_render; j++) {
	int row = lev.nrows-1;
	for (int i=0; i<lev.nrows; i++) {
	    if (lev.arr[row][col] == 'w') {
		glColor3f(0.8, 0.8, 0.6);
		glPushMatrix();
		//put tile in its place
		glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
		glBegin(GL_QUADS);
		glVertex2i( 0,  0);
		glVertex2i( 0, ty);
		glVertex2i(tx, ty);
		glVertex2i(tx,  0);
		glEnd();
		glPopMatrix();
	    }
	    if (lev.arr[row][col] == 'b') {
		glColor3f(0.9, 0.2, 0.2);
		glPushMatrix();
		glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
		glBegin(GL_QUADS);
		glVertex2i( 0,  0);
		glVertex2i( 0, ty);
		glVertex2i(tx, ty);
		glVertex2i(tx,  0);
		glEnd();
		glPopMatrix();
	    }
	    --row;
	}
	col = (col+1) % lev.ncols;
    }
}

void Live(int xres, int yres)
{
    //generate a healthbar on top left of the screen
    //dynamic based on the amount of lives left
    Rect r;
    unsigned int c = 0x002d88d8;
    r.bot = yres-30;
    r.left = (xres/xres)+70 ;
    r.center = 0;
    ggprint8b(&r, 16, c, "Lives");

    for (int i = 0; i < 3; i++) {
	glPushMatrix();
	glColor3ub(255, 0, 255);
	glTranslatef(g.xres/2, g.yres/2, 0);
	float w = g.xres/100;
	float h = g.yres/100;
	glBegin(GL_QUADS);
	// have to texture map rambos head to each quad;
	glVertex2i(-w, -h);
	glVertex2i(-w, h);
	glVertex2i(w, h);
	glVertex2i(w, -h);
	glEnd();
	glPopMatrix();
    }
}
void erikRender()
{
    for(int i=0; i < 3; i++)
    {
	platforms[i].drawPlatform();
    }

}
void renderDeath()
{
    Rect d;
    //glClearColor(0.0,0.5,1.0,1.0);
    //glClear(GL_COLOR_BUFFER_BIT);

    float ws = (float)1/img[2].columns;
    float hs = (float)1/img[2].rows;
    float w  = img[2].width/1.4;
    float h  = img[2].height/1.5;


    // render background after dead screen
    renderBackground(ws,hs,w,h);

    d.bot = g.yres - 20;
    d.left = g.xres/2;
    d.center = 1;
    ggprint13(&d, 16, 0xffffff, "Score: %i", g.score);
    ggprint13(&d, 16, 0xffffff, "Level: %i", level);


    //draw Wasted logo //////////////////////////////////////
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.wasted);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    float ssWidth = (float)1.0/img[1].columns;
    float ssHeight = (float)1.0/img[1].rows;

    float textureX = 0;
    float textureY = 0;

    float centerX = g.xres/2;
    float centerY = g.yres*2/3; 

    float width = floor(((float)g.xres/1280)*img[1].width);
    float height = floor(((float)g.yres/720)*img[1].height);

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

    ////////////////////////////////////////////

    //display meny options
    Rect r;
    r.bot = g.yres/3;
    r.left = g.xres/2;
    r.center = 1;

    switch (selectedOption) {
	case 0:
	    ggprint8b(&r, 16, 0x123fff, "NEW GAME");
	    ggprint8b(&r, 16, 0xffffff, "LEADERBOARD");
	    ggprint8b(&r, 16, 0xffffff, "EXIT");
	    break;
	case 1:

	    ggprint8b(&r, 16, 0xffffff, "NEW GAME");
	    ggprint8b(&r, 16, 0x123fff, "LEADERBOARD");
	    ggprint8b(&r, 16, 0xffffff, "EXIT");
	    break;
	case 2:

	    ggprint8b(&r, 16, 0xffffff, "NEW GAME");
	    ggprint8b(&r, 16, 0xffffff, "LEADERBOARD");
	    ggprint8b(&r, 16, 0x123fff, "EXIT");
	    break;
	    // case 3:
	    // 	ggprint8b(&r, 16, 0x123fff, "NEW GAME");
	    // 	ggprint8b(&r, 16, 0xffffff, "LEADERBOARD");
	    // 	ggprint8b(&r, 16, 0xffffff, "EXIT");
	    // 	break;
	default:
	    break;
    }

    if(flag) {
	leader_board();
	flag = false;
    }
}
void checkKeysDeath()
{
    if (keys[XK_Up]) {
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
		&timers.timeCurrent);
	if (timeSpan > menuSelectionDelay) {
	    selectedOption = ((selectedOption-1)+3)%3;
	    timers.recordTime(&timers.menuSelectionTime);
	}
    }

    if (keys[XK_Down]) {
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
		&timers.timeCurrent);
	if (timeSpan > menuSelectionDelay) {
	    selectedOption = ((selectedOption+1)+3)%3;
	    timers.recordTime(&timers.menuSelectionTime);
	}
    }

    if (keys[XK_Return]) {
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
		&timers.timeCurrent);
	if (timeSpan > menuSelectionDelay) {
	    acceptGameState(selectedOption);
	    timers.recordTime(&timers.menuSelectionTime);
	}
    }
}
void platformPhysics()
{
    for(int j =0; j < 3; j++)
    {
	if (platforms[j].left <= rambo.centerX &&
		platforms[j].right >= rambo.centerX &&
		platforms[j].top >= rambo.centerY-rambo.height &&
		platforms[j].bottom <= rambo.centerY+rambo.height)
	{
	    if(rambo.velocityY < 0){
	    rambo.jumping = false;
	    rambo.centerY = platforms[j].top + rambo.height;
	    break;
	    }
	} else if (rambo.centerY > 100 && !(platforms[j].left <= rambo.centerX) 
		&& !(platforms[j].right >= rambo.centerX) 
		&&!(platforms[j].top >= rambo.centerY-rambo.height) 
		&&!(platforms[j].bottom <= rambo.centerY+rambo.height)) {
	    rambo.jumping = false;
	    break;
	}
	else if(rambo.centerY==100) {
	    rambo.jumping = false;
	}
	else{
	    rambo.jumping = true;
	}

    }

}

