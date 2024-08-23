//modified by:Erik Soto
//date:10/17/17
//
//3480 Computer Graphics
//lab8.cpp
//Author: Gordon Griesel
//Date:   2017
//
//a simple implementation of OpenGL
//circle drawing
//rotation matrix
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "math.h"
#include <iostream>
using namespace std;
#define rnd() (float)rand() / (float)RAND_MAX
const int MAX_POINTS = 10000;

void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
//void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);

struct Point {
    float x, y;
};

class Global {
    public:
	int xres, yres;
	int mode;
	Point point[MAX_POINTS];
	int npoints;
	Point center;
	float radius;
	float matrix[2][2];
	Global() {
	    srand((unsigned)time(NULL));
	    xres = 800;
	    yres = 600;
	    mode = 0;
	    npoints = 5;
	    center.x = xres/2;
	    center.y = yres/2;
	    radius = 100.0;
	}
} g;

class X11_wrapper {
    private:
	Display *dpy;
	Window win;
	GLXContext glc;
    public:
	X11_wrapper() {
	    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	    //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	    XSetWindowAttributes swa;
	    //setup_screen_res(g.xres, g.yres);
	    dpy = XOpenDisplay(NULL);
	    if(dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
	    }
	    Window root = DefaultRootWindow(dpy);
	    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	    if(vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
	    } 
	    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	    swa.colormap = cmap;
	    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
		StructureNotifyMask | SubstructureNotifyMask;
	    win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
		    vi->depth, InputOutput, vi->visual,
		    CWColormap | CWEventMask, &swa);
	    set_title();
	    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	    glXMakeCurrent(dpy, win, glc);
	}
	~X11_wrapper() {
	    XDestroyWindow(dpy, win);
	    XCloseDisplay(dpy);
	}
	void set_title() {
	    //Set the window title bar.
	    XMapWindow(dpy, win);
	    XStoreName(dpy, win, "Lab-8 Circles");
	}
	void check_resize(XEvent *e) {
	    //The ConfigureNotify is sent by the
	    //server if the window is resized.
	    if (e->type != ConfigureNotify)
		return;
	    XConfigureEvent xce = e->xconfigure;
	    if (xce.width != g.xres || xce.height != g.yres) {
		//Window size did change.
		void reshape_window(int width, int height);
		reshape_window(xce.width, xce.height);
	    }
	}
	bool getPending() { return XPending(dpy); }
	void getNextEvent(XEvent *e) { XNextEvent(dpy, e); }
	void swapBuffers() { glXSwapBuffers(dpy, win); }
} x11;

int main()
{
    init_opengl();
    int done = 0;
    while (!done) {
	while (x11.getPending()) {
	    XEvent e;
	    x11.getNextEvent(&e);
	    x11.check_resize(&e);
	    //check_mouse(&e);
	    done = check_keys(&e);
	}
	physics();
	render();
	x11.swapBuffers();
    }
    return 0;
}

void setup_screen_res(const int w, const int h) {
    g.xres = w;
    g.yres = h;
}

void reshape_window(int width, int height) {
    //window has been resized.
    setup_screen_res(width, height);
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    x11.set_title();
}

void init_opengl()
{
    //OpenGL initialization
    glViewport(0, 0, g.xres, g.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //Clear the screen
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
    glClear(GL_COLOR_BUFFER_BIT);
}

int check_keys(XEvent *e)
{
    //Was there input from the keyboard?
    if (e->type == KeyPress) {
	int key = XLookupKeysym(&e->xkey, 0);
	switch (key) {
	    case XK_1:
		//lab-8 circle points on a circle sin cos
		g.mode = 1;
		break;
	    case XK_2:
		//points on a circle rotation matrix
		g.mode = 2;
		break;
	    case XK_3:
		//points on a circle, rotation matrix
		g.mode = 3;
		break;
	    case XK_4:
		//Bresenham's circle algorithm
		g.mode = 4;
		break;
	    case XK_5:
		//triangle strip
		g.mode = 5;
		break;
	    case XK_6:
		//triangle fan
		g.mode = 6;
		break;
	    case XK_7:
		//triangle fan
		g.mode = 7;
		break;
	    case XK_minus:
		if (--g.npoints < 5)
		    g.npoints = 5;
		break;
	    case XK_equal:
		if (++g.npoints >= MAX_POINTS)
		    g.npoints = MAX_POINTS;
		break;
	    case XK_comma:
		g.radius -= 2.0;
		if (g.radius < 2.0)
		    g.radius = 2.0;
		break;
	    case XK_period:
		g.radius += 2.0;
		if (g.radius > 1000.0)
		    g.radius = 1000.0;
		break;
	    case XK_Escape:
		return 1;
		break;
	}
    }
    return 0;
}

void physics()
{

}

void showMenu()
{
    Rect r;
    r.bot = g.yres - 20;
    r.left = 15;
    r.center = 0;
    unsigned int color = 0x00ffff00;
    ggprint8b(&r, 20, color, "3480 Lab-8");
    color = 0x0000ff00;
    ggprint8b(&r, 16, color, "+ - number of points: %i", g.npoints);
    ggprint8b(&r, 20, color, "< > radius: %4.0f", g.radius);
    ggprint8b(&r, 16, color, "1 - points on a circle, (cos sin)");
    ggprint8b(&r, 16, color, "2 - points on a circle, (rotation matrix)");
    ggprint8b(&r, 16, color, "3 - Lab-8 circle");
    ggprint8b(&r, 16, color, "4 - Bresenham's circle");
    ggprint8b(&r, 16, color, "5 - Triangle-strip circle");
    ggprint8b(&r, 16, color, "6 - Triangle-fan circle");
}

const float PI = 3.14159265;

void points_on_a_circle(){

    glColor3ub(30,60,90);
    float angle =0.0;
    float inc = PI *2.0 / (float)g.npoints;
    for(int i =0; i<g.npoints; i++){
	g.point[i].x = cos(angle)* g.radius;
	g.point[i].y = sin(angle)* g.radius;
	angle +=inc;
    }
    // draw the stuff

    glColor3ub(255,255,255);
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<g.npoints;i++)
    {
	glVertex2f(g.center.x + g.point[i].x , g.center.y + g.point[i].y);

    }
    glEnd();


}
void lab8_circle(){

    glColor3ub(255,255,255);
    float x = g.radius;
    float y = 0.0;

    glBegin(GL_POINTS);
    while(x>=y){
	glVertex2f(g.center.x+x , g.center.y+y);
	glVertex2f(g.center.x+y , g.center.y+x);

	glVertex2f(g.center.x+x , g.center.y-y);
	glVertex2f(g.center.x+y , g.center.y-x);

	glVertex2f(g.center.x-x , g.center.y+y);
	glVertex2f(g.center.x-y , g.center.y+x);

	glVertex2f(g.center.x-x , g.center.y-y);
	glVertex2f(g.center.x-y , g.center.y-x);
	y += 1.0;
	// lets decrement
	float distance = sqrt(x*x + y*y);
	if (distance > g.radius)
	    x -=1.0;

    }
    glEnd();
}
void buildMatrix(float m[2][2],float angle){
    m[0][0] = cos(angle);
    m[0][1] =-sin(angle);
    m[1][0] = sin(angle);
    m[1][1] = cos(angle);
}
void rotatePoint(float m[2][2] , Point *p){
    Point t;
    t.x = p->x * m[0][0] + p->y * m[0][1];
    t.y = p->x * m[1][0] + p->y * m[1][1];

    p->x = t.x;
    p->y = t.y;

}
void rotationMatrixCircle(){
    float angle =0.0;
    float inc = PI * 2.0/(float)g.npoints;
    for(int i =0; i<g.npoints; i++)
    {
	buildMatrix(g.matrix ,angle);
	g.point[i].x = g.radius;
	g.point[i].y = 0.0;
	rotatePoint(g.matrix, &g.point[i]);
	angle +=inc;
    }
    glColor3ub(66,255,66);
    glBegin(GL_LINE_LOOP);
    for(int i =0; i<g.npoints;i++)
    {
	glVertex2f(g.center.x+g.point[i].x,g.center.y+g.point[i].y);
    }
    glEnd();




}
void bresenham_circle(float x0, float y0, float radius){

    float x = radius-1;
    float y = 0.0;
    float dx = 1.0;
    float dy = 1.0;
   float err = dx - (radius);

    while (x >= y)
    {
	glVertex2f(x0 + x, y0 + y);
	glVertex2f(x0 + y, y0 + x);
	glVertex2f(x0 - y, y0 + x);
	glVertex2f(x0 - x, y0 + y);
	glVertex2f(x0 - x, y0 - y);
	glVertex2f(x0 - y, y0 - x);
	glVertex2f(x0 + y, y0 - x);
	glVertex2f(x0 + x, y0 - y);

	if (err <= 0)
	{
	    y++;
	    err += dy;
	    dy += 2;
	}
	if (err > 0)
	{
	    x--;
	    dx += 2;
	    err += (-radius) + dx;
	}
    }


}
float angle =0;
int points =4;
void rotatingsquare()
{
    glColor3ub(30,60,90);
    float inc = PI * 2.0 /4;
   for(int i=0; i<points; i++)
   {
    g.point[i].x =cos(angle)*g.radius +140;
    g.point[i].y =sin(angle)*g.radius +140 ;
    angle += inc;   
   }
   glPushMatrix();
   glTranslatef(g.center.x, g.center.y, 0);
   angle +=2;
   glBegin(GL_QUADS);
   for(int i=0; i < points; i++)
   {
       glVertex2f(g.point[i].x , g.point[i].y);
   }

      glEnd();
      glPopMatrix();

}

void trianglestrip()
{
    glColor3ub(30,60,90);
    float angle =0.0;
    float inc = PI *2.0 / (float)g.npoints;
    for(int i =0; i<g.npoints*2; i++){
	g.point[i].x = cos(angle)* g.radius;
	g.point[i].y = sin(angle)* g.radius;

	i++;
	g.point[i].x = cos(angle)* (g.radius-30);
	g.point[i].y = sin(angle)* (g.radius-30);

	angle +=inc;
    }
    // draw the stuff

    int r = 10;
    int g1 = 10;
    int b = 10;
    glColor3ub(r,g1,b);
    glBegin(GL_TRIANGLE_STRIP);

    for(int i=0; i<g.npoints*2;i++)
    {

    	glColor3ub(r,g1,b);
	glVertex2f((int)g.center.x-g.point[i].x , g.center.y-g.point[i].y);
	r +=30;
	g1 +=30;
	b +=30;

    }
    
	glVertex2f((int)g.center.x-g.point[0].x , g.center.y-g.point[1].y);
	glVertex2f((int)g.center.x-g.point[1].x , g.center.y-g.point[0].y);
    glEnd();
}

void trianglefan(){

    glColor3ub(30,60,90);
    float angle =0.0;
    float inc = PI *2.0 / (float)g.npoints;
    for(int i =0; i<g.npoints; i++){
	g.point[i].x = cos(angle)* g.radius;
	g.point[i].y = sin(angle)* g.radius;
	angle +=inc;
    }
    // draw the stuff

    int r = 70;
    int g1 = 20;
    int b = 0;
    glColor3ub(r,g1,b);
    glBegin(GL_TRIANGLE_FAN);

    for(int i=0; i<g.npoints;i++)
    {

    	glColor3ub(r,g1,b);
	glVertex2f((int)g.center.x-g.point[i].x , g.center.y-g.point[i].y);
	r +=30;
	g1 +=30;
	b +=30;

    }
    glEnd();
   /* glBegin(GL_TRIANGLE_FAN);
     r = 10;
    g1 = 10;
    b = 10;
    for(int i=0; i<g.npoints;i++)
    {

    	glColor3ub(r,g1,b);
	glVertex2f((int)g.center.x+g.point[i].x , g.center.y+g.point[i].y);
	r +=30;
	g1 +=30;
	b +=30;

    }
    glEnd();
*/

}
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    showMenu();
    switch (g.mode) {
	case 1:
	    points_on_a_circle(); // finished, circle 

	    break;
	case 2:
	    rotationMatrixCircle();// finished circle

	    break;
	case 3:
	    lab8_circle(); // finished circle 

	    break;
	case 4:
	    bresenham_circle(g.radius,0,g.radius);   // not finished 

	    break;
	case 5:
	     trianglestrip();   // triangle strip finished 
        break;

   case 6:
	    trianglefan();      // triagnle fan finsihed 
       break;

   case 7:
       rotatingsquare(); // rotating square not finished 
       break;
	  
   default: 
       break;
    }
}

