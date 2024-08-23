// Platform boxes class
// Father: CS God
// Author: Erik Soto


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "ppm.h"
#include "fonts.h"
#include "erikS.h"
#include "Global.h"
extern Global g;
extern Level lev;
using namespace std;

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
			     (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]



void renderBackground(int lev)
{
//background image
	if (lev == 1)
		g.tempBackgroundTexture = g.RamboTexture;
	//if (lev == 2)
		//g.tempBackgroundTexture = g.LATexture;

	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	glBindTexture(GL_TEXTURE_2D, g.tempBackgroundTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(g.x[0], g.y[1]); glVertex2i(0,0);
		glTexCoord2f(g.x[0], g.y[0]); glVertex2i(0, g.yres);
		glTexCoord2f(g.x[1], g.y[0]); glVertex2i(g.xres, g.yres);
		glTexCoord2f(g.x[1], g.y[1]); glVertex2i(g.xres, 0);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void pauseScreen()
{
	//Renders Pause Screen
	//Ability to go back to Menu
	Rect r; 
	float h = 100.0;
	float w = 200.0;
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	ggprint8b(&r, 16, 0, "Press P - Play");
	ggprint8b(&r, 16, 0, "Press E - Exit to Menu");
	ggprint8b(&r, 16, 0, "Press Esc - Exit Game");
}


class Platform{

    class point{
	public: 
	    float x,y;


    };

    class color{
	public:	
	    float r,g,b;
    };



    private:
    point one,two,three,four;
    color set;

    public:
    int top;
    int left;
    int right;


    Platform(float xpos ,float ypos,float xpos2 ,float ypos2,float xpos3 ,float ypos3,float xpos4 ,float ypos4)
    {
	// set the ranges for physics
	top = greatest(ypos,ypos2,ypos3,ypos4);

	one.x = xpos;
	one.y = ypos;

	two.x = xpos2;
	two.y = ypos2;

	three.x = xpos3;
	three.y = ypos3;

	four.x = xpos4;
	four.y = ypos4;
    }

    void drawPlatform()
    {
	glBegin(GL_QUADS);
	glColor3f(set.r,set.g,set.b);
	glVertex2i(one.x,	one.y);
	glVertex2i(two.x,	two.y);
	glColor3f(set.r,set.g,set.b);
	glVertex2i(three.x,	three.y);
	glVertex2i(four.x,	four.y);

	glEnd();
    }
    void setColor(float r, float g, float b)
    {
	set.r =r;
	set.g =g;
	set.b = b;


    }
    float greatest(float one,float two,float three,float four)
    {
	float g= greatest( one,two);
	float b= greatest(three,four);
	return greatest(g,b);

    }
    float greatest(float one,float two)
    {
	if(one < two)
	{
	    return two;
	}
	else return one;

    }
};

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
    //Partial tile offset must be determined here.
    //The leftmost tile might be partially off-screen.
    //cdd: camera position in terms of tiles.
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
