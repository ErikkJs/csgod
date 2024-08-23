//modified by:Erik Soto
//
//program: lab2.cpp
//author:  Gordon Griesel
//purpose: Framework for graphics
//         Using X11 (Xlib) for drawing
//
//drawing straight lines
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <iostream>
using namespace std;
const int MAX_POINTS =10000000;

struct Point
{
    double x,y;
};

class Global {
    public:
        int xres, yres,npoints,grabbed_point;
        bool rigid;
        Point point[MAX_POINTS];
        bool anchors;
        int distances[MAX_POINTS];
        Global() {
            srand((unsigned)time(NULL));
            xres = 800;
            yres = 600;
            npoints=0;
            grabbed_point=-1;
            rigid=false;
            anchors=true;
        }
} g;

class X11 {
    private:
        Display *dpy;
        Window win;
        GC gc;
    public:
        X11() {
            //constructor
            if (!(dpy=XOpenDisplay(NULL))) {
                fprintf(stderr, "ERROR: could not open display\n"); fflush(stderr);
                exit(EXIT_FAILURE);
            }
            int scr = DefaultScreen(dpy);
            win = XCreateSimpleWindow(dpy, RootWindow(dpy, scr), 1, 1,
                    g.xres, g.yres, 0, 0x00ffffff, 0x00000000);
            XStoreName(dpy, win, "X11 sample program");
            gc = XCreateGC(dpy, win, 0, NULL);
            XMapWindow(dpy, win);
            XSelectInput(dpy, win, ExposureMask | StructureNotifyMask |
                    PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                    KeyPressMask);
        }
        ~X11() {
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);
        }
        void check_resize(XEvent *e) {
            //ConfigureNotify is sent when window size changes.
            if (e->type != ConfigureNotify)
                return;
            XConfigureEvent xce = e->xconfigure;
            g.xres = xce.width;
            g.yres = xce.height;
        }
        void clear_screen() {
            XClearWindow(dpy, win);
        }
        void setColor3i(int r, int g, int b) {
            unsigned long cref = 0L;
            cref += r;
            cref <<= 8;
            cref += g;
            cref <<= 8;
            cref += b;
            XSetForeground(dpy, gc, cref);
        }
        bool getXPending() {
            return (XPending(dpy));
        }
        void getXNextEvent(XEvent *e) {
            XNextEvent(dpy, e);
        }
        void drawPixel(int x, int y) {
            XDrawPoint(dpy, win, gc, x, y);
        }
        void drawLine(int x0, int y0, int x1, int y1) {
            XDrawLine(dpy, win, gc, x0, y0, x1, y1);
        }
        void drawText(int x, int y, const char *text) {
            XDrawString(dpy, win, gc, x, y, text, strlen(text));
        }

        void fillRectangle(int x,int y,int w,int h)
        {
            XFillRectangle(dpy, win, gc, x, y, w, h);
        }
        void drawredrect(int x, int y, int w, int h)
        {

            XDrawRectangle(dpy, win, gc, x, y, w+5, h+5);
        }

        void setDistances(){
            for(int i=0; i<MAX_POINTS;i++)
            {
                double v[2];

                v[0]=g.point[i+1].x-g.point[i].x;
                v[1]=g.point[i+1].y-g.point[i].y;
                g.distances[i]=sqrt(v[0]*v[0] + v[1]*v[1]); 


            }
        }
        void subDivide()
        {
            for(int i =0; i<g.npoints;i++)
            {
                double v[2];
                v[0]=(g.point[i].x +g.point[i+1].x )/2;
                v[1]=(g.point[i].y +g.point[i+1].y )/2;

                g.point[g.npoints].x=v[0];
                g.point[g.npoints].y=v[1];
                g.npoints++;

            }

        }




} x11;

//function prototypes
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();


int main(void)
{
    int done = 0;
    while (!done) {
        //Check the event queue
        while (x11.getXPending()){
            //Handle them one-by-one
            XEvent e;
            x11.getXNextEvent(&e);
            x11.check_resize(&e);
            check_mouse(&e);
            done = check_keys(&e);
            //Render when any event happens
            render();
        }
    }
    return 0;
}

void check_mouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;
    int mx = e->xbutton.x;
    int my = e->xbutton.y;
    //
    if (e->type == ButtonRelease){
        g.grabbed_point=-1;
        return;
    }
    if (e->type == ButtonPress){
        //A mouse button was pressed.
        if (e->xbutton.button==1){
            //Left button pressed
            //grab a point
            double close_dist=9e9;
            int close_idx;
            for(int i=0;i<g.npoints;i++){
                double d0=mx-g.point[i].x;
                double d1=my-g.point[i].y;
                double dist = sqrt(d0*d0 + d1*d1);
                if(dist<close_dist){
                    close_dist=dist;
                    close_idx=i;
                }
            }
            if(close_dist < 20.0){
                g.grabbed_point=close_idx;
            }
        }
        if (e->xbutton.button==3){
            //Right button pressed
            //save point
            if(g.npoints<MAX_POINTS){
                g.point[g.npoints].x=mx;
                g.point[g.npoints].y=my;
                ++g.npoints;
            }
        }
    }
    if (savex != mx || savey != my) {
        //mouse moved
        savex = mx;
        savey = my;
        if(g.grabbed_point>=0){
            if(g.rigid){

                for(int i=0;i<g.npoints;i++){
                    double v[2];

                    v[0]=g.point[1].x-g.point[0].x;
                    v[1]=g.point[1].y-g.point[0].y;
                    double vlen=sqrt(v[0]*v[0] + v[1]*v[1]); 
                    g.point[g.grabbed_point].x = mx;
                    g.point[g.grabbed_point].y = my;

                    //create new vector
                    double v1[2];
                    v1[0]=g.point[1].x-mx;
                    v1[1]=g.point[1].y-my;

                    //nromalizes the vector
                    double len = sqrt(v1[0]*v1[0] + v1[1]*v1[1]);
                    v1[0] /=len;
                    v1[1] /=len;
                    //scale the vector
                    v1[0] *= vlen;
                    v1[1] *= vlen;

                    g.point[g.grabbed_point+1].x = mx+v1[0];
                    g.point[g.grabbed_point+1].y = my+v1[1];


                    // second point 
                    v[0]=g.point[1].x-g.point[0].x;
                    v[1]=g.point[1].y-g.point[0].y;
                    vlen=sqrt(v[0]*v[0] + v[1]*v[1]); 

                }
            }
            else
            {
                g.point[g.grabbed_point].x = mx;
                g.point[g.grabbed_point].y = my;
            }
        }
        for(int i =0; i<g.npoints;i++)
        {
         //   if(g.point[i].x - mx <= 20 && g.point[i].y -my <= 20)
           // {

            x11.drawredrect(g.point[i].x-10, g.point[i].y-10, 10*2+1, 10*2+1);
            
           // }
        
        }

    }
}

int check_keys(XEvent *e)
{
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    switch (key) {
        case XK_r:
            // sets the rigidness
            x11.setDistances();
            if(g.rigid == true){g.rigid=false;}
            else{g.rigid=true;}
            break;
        case XK_a:
            //makes anchors dissapear
            if(g.anchors==true){g.anchors=false;}else g.anchors =true;
            break;
        case XK_d:
            // deltes last point
            g.npoints--;
            break;
        case XK_s:// split the points 
            // this is a for loop
            for(int i=0; i<=g.npoints-2;i++) {
                //point array  
                double v[2];

                v[0]=(g.point[i].x +g.point[i+1].x)/2;
                v[1]=(g.point[i].y +g.point[i+1].y)/2;
                g.npoints++;

                //dynamicaly rearranging maticitical points
                for(int j=g.npoints; j>i; j--)
                { 
                    g.point[j].x= g.point[j-1].x;
                    g.point[j].y= g.point[j-1].y;
                }
                //through scientific methods inputting indices 
                g.point[i+1].x =v[0];
                g.point[i+1].y =v[1];
                i++;  
            } 

            break;
        case XK_Escape:
            //program is ending
            return 1;
    }
    return 0;
}

void physics(void)
{
    //no physics yet.
}

void drawAllPoints(int size)
{
    x11.setColor3i(255, 255, 255);
    for(int i=0;i<g.npoints;i++){
        //x11.drawPixel(g.point[i].x,g.point[i].y);
        if(g.anchors==1){
            x11.fillRectangle(g.point[i].x-size, g.point[i].y-size, size*2+1, size*2+1);
        }
    }
}

void drawAllLines()
{
    drawAllPoints(10);
    for (int i=0;i<g.npoints-1;i++){
        x11.drawLine(g.point[i].x, g.point[i].y, g.point[i+1].x, g.point[i+1].y);
    }
}
void render(void)
{
    x11.clear_screen();
    //render function is always at the bottom of program.
    x11.setColor3i(255, 255, 255);
    x11.drawText(4, 20, "Right-click to place a point.");
    x11.drawText(4, 32, "Press L to toggle lines.");
    x11.drawText(4, 10, "Press S to sub divide and add point.");
    x11.drawText(4, 82, "Press D to delete last point.");
    x11.drawText(4, 44,"Press B to toggle Bresenham lines.");
    x11.drawText(4, 56,"Press r to toggle rigid lines.");
    x11.drawText(4, 68,(g.rigid)?"rigid is on":"rigid is off");
    x11.drawText(4, 122, "Press A to delete anchors.");
    x11.setColor3i(255, 0, 0);
    ///drawAllPoints(10);
    drawAllLines();

}














