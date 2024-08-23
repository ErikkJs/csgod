//modified by:
//       date:
//    changes:
//
//
//program: lab4.cpp
//author:  Gordon Griesel (do change the author's name)
//purpose: Framework for spring physics
//         Using XWindows (Xlib) for drawing pixels
//         Double buffer used for smooth animation
//
//Rope physics
//  This will lead you to cloth physics.
//  Rope or string is 1-dimensional.
//  Cloth is 2-dimensional, lots of strings connected into a grid.
//
// Key presses:
// R - drop the rope
// S - enlarge the sphere. (shift-S to reduce)
// 1 - pull rope to the left
// 2 - pull rope to the right
// Arrow keys - move sphere
// Mouse drag sphere with left button
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/Xdbe.h>

typedef double Flt;
#define PI 3.14159265358979
#define SWAP(x,y) (x)^=(y);(y)^=(x);(x)^=(y)
#define rnd() (Flt)rand() / (Flt)RAND_MAX
void init();
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void setup_springs();
void physics();
void render();
//
class Global {
    public:
        int xres, yres;
        int gravity;
        int jello;
        int jwidth;
        bool clicked=false;
        int masses;
        Global() {
            xres=800;
            yres=600;
            gravity = 0;
            jello = 0;
            masses=0;
            jwidth=1;
        }
} g;

//-------------------------------------------
//XWindows wrapper class
class X11_wrapper {
    private:
        Display *dpy;
        Window win;
        GC gc;
        XdbeBackBuffer backBuffer;
        XdbeSwapInfo swapInfo;
    public:
        X11_wrapper() {
            printf("constructor\n"); fflush(stdout); 
            //constructor. Do not change.
            int major, minor;
            XSetWindowAttributes attributes;
            XdbeBackBufferAttributes *backAttr;
            dpy = XOpenDisplay(NULL);
            //List of events we want to handle
            attributes.event_mask = ExposureMask | StructureNotifyMask |
                PointerMotionMask | ButtonPressMask |
                ButtonReleaseMask | KeyPressMask | KeyReleaseMask;
            //Various window attributes
            attributes.backing_store = Always;
            attributes.save_under = True;
            attributes.override_redirect = False;
            attributes.background_pixel = 0x00000000;
            //Get default root window
            Window root = DefaultRootWindow(dpy);
            //Create a window
            win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
                    CopyFromParent, InputOutput, CopyFromParent,
                    CWBackingStore | CWOverrideRedirect | CWEventMask |
                    CWSaveUnder | CWBackPixel, &attributes);
            //Create gc
            gc = XCreateGC(dpy, win, 0, NULL);
            //Get DBE version
            if (!XdbeQueryExtension(dpy, &major, &minor)) {
                fprintf(stderr, "Error : unable to fetch Xdbe Version.\n");
                XFreeGC(dpy, gc);
                XDestroyWindow(dpy, win);
                XCloseDisplay(dpy);
                exit(1);
            }
            printf("Xdbe version %d.%d\n",major,minor);
            //Get back buffer and attributes (used for swapping)
            backBuffer = XdbeAllocateBackBufferName(dpy, win, XdbeUndefined);
            backAttr = XdbeGetBackBufferAttributes(dpy, backBuffer);
            swapInfo.swap_window = backAttr->window;
            swapInfo.swap_action = XdbeUndefined;
            XFree(backAttr);
            //Map and raise window
            setWindowTitle();
            XMapWindow(dpy, win);
            XRaiseWindow(dpy, win);
            printf("exit constructor\n"); fflush(stdout); 
        }
        ~X11_wrapper() {
            //Do not change. Deallocate back buffer.
            if(!XdbeDeallocateBackBufferName(dpy, backBuffer)) {
                fprintf(stderr,"Error : unable to deallocate back buffer.\n");
            }
            XFreeGC(dpy, gc);
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);
        }
        void swapBuffers() {
            XdbeSwapBuffers(dpy, &swapInfo, 1);
            usleep(4000);
        }
        bool getPending() {
            return XPending(dpy);
        }
        void getNextEvent(XEvent *e) {
            XNextEvent(dpy, e);
        }
        void set_color_3i(int r, int g, int b) {
            unsigned long cref = 0L;
            cref += r;
            cref <<= 8;
            cref += g;
            cref <<= 8;
            cref += b;
            XSetForeground(dpy, gc, cref);
        }
        void setWindowTitle() {
            char ts[256];
            sprintf(ts, "dbeCloth  R=Replay  %i x %i", g.xres, g.yres);
            XStoreName(dpy, win, ts);
        }
        void clear_screen() {
            //XClearWindow(dpy, backBuffer);
            XSetForeground(dpy, gc, 0x00050505);
            XFillRectangle(dpy, backBuffer, gc, 0, 0, g.xres, g.yres);
        }
        void drawString(int x, int y, const char *message) {
            XDrawString(dpy, backBuffer, gc, x, y, message, strlen(message));
        }
        void drawPoint(int x, int y) {
            XDrawPoint(dpy, backBuffer, gc, x, y);
        }
        void drawLine(int x0, int y0, int x1, int y1) {
            XDrawLine(dpy, backBuffer, gc, x0, y0, x1, y1);
        }
        void drawRectangle(int x, int y, int w, int h) {
            //x,y is upper-left corner
            XDrawRectangle(dpy, backBuffer, gc, x, y, w, h);
        }
        void fillRectangle(int x, int y, int w, int h) {
            //x,y is upper-left corner
            XFillRectangle(dpy, backBuffer, gc, x, y, w, h);
        }
} x11;
//---------------------------
//Global variables.
//You may put these inside the Global class, or make a new class.
//
#define MAX_CONTROL_POINTS 2000
//
struct Mass {
    Flt mass, oomass;
    Flt pos[2];
    Flt vel[2];
    Flt force[2];
    int color[3];
} mass[MAX_CONTROL_POINTS];
int nmasses=0;
//
struct Spring {
    int mass[2];
    Flt length;
    Flt stiffness;
} spring[MAX_CONTROL_POINTS*4];
int nsprings=0;

//Please add these to the Global class
//
Flt sphereCenter[2];
Flt sphereRadius;
Flt sphereRadius2;
int verts[40][2];
int nverts=0;
int cloth=1;
int pull=0;


int main()
{
    srand((unsigned)time(NULL));
    init();
    int done = 0;
    while (!done) {
        //Handle all events in queue...
        while (x11.getPending()) {
            XEvent e;
            x11.getNextEvent(&e);
            check_resize(&e);
            check_mouse(&e);
            done = check_keys(&e);
        }
        //Process physics and rendering every frame
       if(g.clicked==false){ physics();}
        render();
        x11.swapBuffers();
    }
    return 0;
}

void init()
{
    //build points on a circle
    int i, n=40;
    float ang = 0.0;
    float inc = (PI*2.0)/(float)n;
    float rad;
    static int firsttime=1;
    if (firsttime) {
        firsttime=0;
        sphereRadius = (Flt)(g.xres / 8);
        sphereCenter[0] = (Flt)(g.xres / 2);
        sphereCenter[1] = sphereRadius * 2.0;
    }
    rad = sphereRadius - 2.0;
    nverts=0;
    for (i=0; i<n; i++) {
        verts[nverts][0] = (int)(sin(ang) * rad + sphereCenter[0]);
        verts[nverts][1] = (int)(cos(ang) * rad + sphereCenter[1]);
        nverts++;
        ang += inc;
    }
    sphereRadius2 = sphereRadius * sphereRadius;
}

void check_resize(XEvent *e)
{
    //ConfigureNotify is sent when the window is resized.
    if (e->type != ConfigureNotify)
        return;
    XConfigureEvent xce = e->xconfigure;
    g.xres = xce.width;
    g.yres = xce.height;
    init();
    x11.setWindowTitle();
}

void setup_jello()
{
    Flt x = 100.0;
    Flt y = 150.0;
    nmasses = 0;
    for (int i=0; i<g.jwidth; i++) {
        x = 100.0;
        for (int j=0; j<g.jwidth; j++) {
            mass[nmasses].mass = 1.0;
            mass[nmasses].oomass = 1.0 / mass[nmasses].mass;
            mass[nmasses].pos[0] = x;
            mass[nmasses].pos[1] = y;
            mass[nmasses].vel[0] =
                mass[nmasses].vel[1] =
                mass[nmasses].force[0] =
                mass[nmasses].force[1] = 0.0;
            mass[nmasses].color[0] = 200 + rand() % 40;
            mass[nmasses].color[1] = 40 + rand() % 20;
            mass[nmasses].color[2] = 10 + rand() % 10;
            nmasses++;
            g.masses++;
            x += 40.0;
        }
        y += 40.0;
    }
    printf("nmasses: %i\n", nmasses);
    nsprings = 0;
    //
    //Build your structural springs here...

    int mass_num=0;
    for(int i=0; i<g.jwidth;i++){
        for(int j=0;j<g.jwidth;j++){

            //left diagnal
            if(!(j%g.jwidth==0 || i==g.jwidth-1 )){
                spring[nsprings].mass[0] = mass_num;
                spring[nsprings].mass[1] = mass_num+g.jwidth-1;
                spring[nsprings].length = 40.0 *sqrt(2);
                spring[nsprings].stiffness = rnd()*0.2+0.05;
                nsprings++;
            }

            if(!(j==g.jwidth-1||i==g.jwidth-1 )){
                spring[nsprings].mass[0] = mass_num;
                spring[nsprings].mass[1] = mass_num+g.jwidth +1;
                spring[nsprings].length = 40.0 * sqrt(2);
                spring[nsprings].stiffness = rnd()*0.2+0.05;
                nsprings++;
            }

            //right diagonal
            if(!(i==g.jwidth-1)){
                spring[nsprings].mass[0] = mass_num;
                spring[nsprings].mass[1] = mass_num+g.jwidth;
                spring[nsprings].length = 40.0;
                spring[nsprings].stiffness = rnd()*0.2+0.05;
                nsprings++;
            }

            //right
            if(!(j==g.jwidth-1 )){
                spring[nsprings].mass[0] = mass_num;
                spring[nsprings].mass[1] = mass_num+1;
                spring[nsprings].length = 40.0;
                spring[nsprings].stiffness = rnd()*0.2+0.05;
                nsprings++;
            }
            mass_num++;
        }
    }
    /*
       for(int j=0;j<16;j++){
       if(j%2==0 || i==0){
       spring[nsprings].mass[0] = j;
       spring[nsprings].mass[1] = j+2;
       spring[nsprings].length = 40.0;
       spring[nsprings].stiffness = rnd()*0.2+0.1;
       nsprings++;
       }
       */

//Bend springs...
//Join every second mass.

//Join every 3rd mass.

printf("nsprings: %i\n", nsprings);
}

void setup_springs()
{
    int i,j,n;
    Flt x;
    Flt xlen;
    Flt vel[2];
    pull=0;
    n = 180;
    xlen = (Flt)g.xres * 0.9 / n;
    //position the masses
    x = (Flt)(g.xres / 2) - (xlen * n) / 2.0;
    vel[0] *= 0.01;
    vel[1] *= 0.01;
    nmasses = 0;
    for (i=0; i<n; i++) {
        mass[nmasses].mass = 1.0;
        mass[nmasses].oomass = 1.0 / mass[nmasses].mass;
        mass[nmasses].pos[0] = x;
        mass[nmasses].pos[1] = 60.0;
        if (mass[nmasses].pos[1] < 1.0)
            mass[nmasses].pos[1] = 1.0;
        mass[nmasses].vel[0] = vel[0] + rnd() * 0.02-0.01;
        mass[nmasses].vel[1] = vel[1] + rnd() * 0.02-0.01;
        mass[nmasses].force[0] =
            mass[nmasses].force[1] = 0.0;
        mass[nmasses].color[0] = 200 + rand() % 40;
        mass[nmasses].color[1] = 40 + rand() % 20;
        mass[nmasses].color[2] = 10 + rand() % 10;
        nmasses++;
        x += xlen;
    }
    nsprings = 0;
    //Structural springs...
    for (i=0; i<nmasses-1; i++) {
        j=i+1;
        spring[nsprings].mass[0] = i;
        spring[nsprings].mass[1] = j;
        spring[nsprings].length = mass[j].pos[0] - mass[i].pos[0];
        spring[nsprings].stiffness = rnd() * 0.2 + 0.1;
        nsprings++;
    }
    //Bend springs...
    //Join every second mass.
    for (i=0; i<nmasses-2; i++) {
        j=i+2;
        spring[nsprings].mass[0] = i;
        spring[nsprings].mass[1] = j;
        spring[nsprings].length = mass[j].pos[0] - mass[i].pos[0];
        spring[nsprings].stiffness = 0.2;
        nsprings++;
        i=j;
    }
    //Join every 3rd mass.
    for(i=0; i<nmasses-3;i++)
    {
        j=i+3;
        spring[nsprings].mass[0] =i;
        spring[nsprings].mass[1] =j;
        spring[nsprings].length = mass[j].pos[0] - mass[i].pos[0];
        spring[nsprings].stiffness =.2;
        nsprings++;
       i=j;
    }


    //Join every 4th mass.
    for(int i=0; i <nmasses;i++)
    {
      
        j=i+4;
        spring[nsprings].mass[0] =i;
        spring[nsprings].mass[1] =j;
        spring[nsprings].length = mass[j].pos[0] - mass[i].pos[0];
        spring[nsprings].stiffness =.2;
        nsprings++;
       i=j;

    }

    //Join every 10th mass.

    //Whole length stiffener

}

void maintain_springs()
{
    int i,m0,m1;
    Flt dist,oodist,factor;
    Flt springVec[2];
    Flt springforce[2];
    const Flt penalty=0.05f;
    //Move the masses...
    for (i=0; i<nmasses; i++) {
        mass[i].vel[0] += mass[i].force[0] * mass[i].oomass;
        mass[i].vel[1] += mass[i].force[1] * mass[i].oomass;
        mass[i].pos[0] += mass[i].vel[0];
        mass[i].pos[1] += mass[i].vel[1];
        mass[i].force[0] =
            mass[i].force[1] = 0.0;
        if (!pull) {
            if (mass[i].pos[0] < 0.0) {
                mass[i].force[0] = penalty * -mass[i].pos[0];
            }
            if (mass[i].pos[0] > (Flt)g.xres) {
                mass[i].force[0] = penalty * ((Flt)g.xres - mass[i].pos[0]);
            }
        }
        //hit the ceiling?
        if (mass[i].pos[1] < 0.0) {
            mass[i].force[1] = penalty * -mass[i].pos[1];
        }
        //hit the floor?
        if (mass[i].pos[1] > (Flt)g.yres) {
            mass[i].vel[0] *= 0.95;
            mass[i].vel[1] *= 0.95;
            mass[i].force[1] = penalty * ((Flt)g.yres - mass[i].pos[1]);
        }
        //There is a collision sphere in the scene
        Flt d0 = mass[i].pos[0] - sphereCenter[0];
        Flt d1 = mass[i].pos[1] - sphereCenter[1];
        Flt dist = d0*d0+d1*d1;
        if (dist <= sphereRadius2) {
            //Cloth has hit the sphere! Apply a penalty that pushes
            //cloth away from the center of the sphere.
            mass[i].force[1]= penalty * ((Flt)g.yres-mass[i].pos[1]);
            static const Flt penalty = 0.1;
            dist = 1.0 / sqrt(dist);
            d0 *= dist;
            d1 *= dist;
            mass[i].force[0] = d0 * penalty;
            mass[i].force[1] = d1 * penalty;
            //Apply some drag caused by the rough surface of the sphere
            mass[i].vel[0] *= 0.8;
            mass[i].vel[1] *= 0.8;
            //Place mass position on the edge of the sphere
            //Don't let the mass penetrate the sphere surface
            mass[i].pos[0] = sphereCenter[0] + d0 * sphereRadius; 
            mass[i].pos[1] = sphereCenter[1] + d1 * sphereRadius;
        }
        //Max velocity
        if (mass[i].vel[0] > 10.0)
            mass[i].vel[0] = 10.0;
        if (mass[i].vel[1] > 10.0)
            mass[i].vel[1] = 10.0;
        //Air resistance, or some type of damping
        mass[i].vel[0] *= 0.999;
        mass[i].vel[1] *= 0.999;
    }
    //Resolve all springs...
    for (i=0; i<nsprings; i++) {
        m0 = spring[i].mass[0];
        m1 = spring[i].mass[1];
        //forces are applied here
        //vector between the two masses
        springVec[0] = mass[m0].pos[0] - mass[m1].pos[0];
        springVec[1] = mass[m0].pos[1] - mass[m1].pos[1];
        //distance between the two masses
        dist = sqrt(springVec[0]*springVec[0] + springVec[1]*springVec[1]);
        if (dist == 0.0) dist = 0.1;
        oodist = 1.0 / dist; 
        springVec[0] *= oodist;
        springVec[1] *= oodist;
        //the spring force is added to the mass force
        factor = -(dist - spring[i].length) * spring[i].stiffness;
        springforce[0] = springVec[0] * factor;
        springforce[1] = springVec[1] * factor;
        //apply force and negative force to each end of the spring...
        mass[m0].force[0] += springforce[0];
        mass[m0].force[1] += springforce[1];
        mass[m1].force[0] -= springforce[0];
        mass[m1].force[1] -= springforce[1];
        //damping
        springforce[0] = (mass[m1].vel[0] - mass[m0].vel[0]) * 0.002;
        springforce[1] = (mass[m1].vel[1] - mass[m0].vel[1]) * 0.002;
        mass[m0].force[0] += springforce[0];
        mass[m0].force[1] += springforce[1];
        mass[m1].force[0] -= springforce[0];
        mass[m1].force[1] -= springforce[1];
    }
}

void check_mouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;
    static int lbuttonDown=0;
    //
    if (e->type == ButtonRelease) {
        lbuttonDown=0;
        g.clicked=false;
        return;
    }
    if (e->type == ButtonPress) {
        //Log("ButtonPress %i %i\n", e->xbutton.x, e->xbutton.y);
        if (e->xbutton.button==1) {
            //left button is down
            lbuttonDown=1;
        }
        if (e->xbutton.button==3) {
            g.clicked=true;
            double close_dist=9e9;
            int close_idx;
           // for(int i=0;i<g.


        }
    }
    if (savex != e->xbutton.x || savey != e->xbutton.y) {
        //mouse moved
        savex = e->xbutton.x;
        savey = e->xbutton.y;
        if (lbuttonDown) {
            sphereCenter[0] = e->xbutton.x;
            sphereCenter[1] = e->xbutton.y;
            init();
        }
    }
}

int check_keys(XEvent *e)
{
    static int shift=0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyRelease) {
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift=0;
        return 0;
    }
    if (e->type == KeyPress) {
        if (key == XK_Shift_L || key == XK_Shift_R) {
            shift=1;
            return 0;
        }
    } else {
        return 0;
    }
    //a key was pressed
    switch (key) {
        case XK_1:
            pull -= 15;
            break;
        case XK_2:
            pull += 15;
            break;
        case XK_j:
            g.jello = 1;
            g.jwidth +=1;
            if (g.jello) {
                setup_jello();
                setup_jello();
            }
            break;
        case XK_g:
            g.gravity ^= 1;
            break;
        case XK_r:
            setup_springs();
            break;
        case XK_s:
            if (shift) {
                sphereRadius *= (1.0 / 1.01);
            } else {
                sphereRadius *= 1.01;
            }
            init();
            break;
        case XK_Up:
            sphereCenter[1] -= 4.0;
            init();
            break;
        case XK_Down:
            sphereCenter[1] += 4.0;
            init();
            break;
        case XK_Left:
            sphereCenter[0] -= 4.0;
            init();
            break;
        case XK_Right:
            sphereCenter[0] += 4.0;
            init();
            break;
        case XK_z:

            if(g.jello==1)
            {
                for(int i=0; i<nsprings;i++)
                {
                    if(spring[i].stiffness<.5){
                        spring[i].stiffness -=.01;
                        spring[i].length+=1;
                        printf("nspringsstiff: %f\n", spring[i].stiffness);}
                }

            }
            init();
            break;
        case XK_t:

            if(g.jello==1)
            {
                for(int i=0; i<nsprings;i++)
                {
                    if(spring[i].stiffness<.5){
                        spring[i].stiffness +=.01;
                        printf("nspringsstiff: %f\n", spring[i].stiffness);}
                }

            }
            init();
            break;
        case XK_d:
            g.jwidth+=1;
            init();
            break;

        case XK_Escape:
            //quitting the program
            return 1;
    }
    return 0;
}

void myBresenhamLine(int x0, int y0, int x1, int y1)
{
    //Bresenham's line algorithm. integers only!
    int steep = (abs(y1-y0) > abs(x1-x0));
    if (steep) {
        SWAP(x0, y0);
        SWAP(x1, y1);
    }
    if (x0 > x1) {
        SWAP(x0, x1);
        SWAP(y0, y1);
    }
    int ystep = (y1>y0) ? 1 : -1;
    int yDiff = abs(y1-y0);
    int xDiff = x1 - x0;
    int err=xDiff>>1;
    int x, y=y0;
    for	(x=x0; x<=x1; x++) {
        if (steep)
            x11.drawPoint(y, x);
        else
            x11.drawPoint(x, y);
        err -= yDiff;
        if (err <= 0) {
            y += ystep;
            err += xDiff;
        }
    }
}

void show_anchor(int x, int y, int size)
{
    //draw a small rectangle...
    int i,j;
    for (i=-size; i<=size; i++) {
        for (j=-size; j<=size; j++) {
            x11.drawPoint(x+j, y+i);
        }
    }
}

void show_sphere()
{
    int i,j;
    x11.set_color_3i(155,200,0);
    for (i=0; i<nverts; i++) {
        j = i+1;
        if (j >= nverts)
            j -= nverts;
        myBresenhamLine(verts[i][0], verts[i][1],
                verts[j][0], verts[j][1]);
    }
}

void show_lines()
{
    //draw the lines using Bresenham's algorithm
    x11.set_color_3i(mass[0].color[0],mass[0].color[1],mass[0].color[2]);
    //	for (i=0; i<nmasses-1; i++) {
    //		myBresenhamLine((int)mass[i].pos[0], (int)mass[i].pos[1],
    //			(int)mass[i+1].pos[0], (int)mass[i+1].pos[1]);
    //	}
    for (int i=0; i<nsprings; i++) {
        myBresenhamLine(
                (int)mass[spring[i].mass[0]].pos[0],
                (int)mass[spring[i].mass[0]].pos[1],
                (int)mass[spring[i].mass[1]].pos[0],
                (int)mass[spring[i].mass[1]].pos[1]);
    }
    for (int i=0; i<nmasses; i++) {
        x11.set_color_3i(mass[i].color[0],mass[i].color[1],mass[i].color[2]);
        show_anchor((int)mass[i].pos[0], (int)mass[i].pos[1],
                (int)mass[i].mass*2);
    }
}

void physics()
{
    int i;
    //gravity...
    if (g.gravity) {
        for (i=0; i<nmasses; i++) {
            mass[i].force[1] += 0.01;
        }
    }
    if (pull < 0) {
        mass[0].vel[0] -= 1.5;
        ++pull;
    }
    if (pull > 0) {
        mass[nmasses-1].vel[0] += 1.5;
        --pull;
    }
    maintain_springs();
}

void showMenu()
{
    x11.set_color_3i(255,255,0);
    int y = 10;
    x11.drawString(10, y, "Key presses:");
    y += 16;
    x11.drawString(10, y, "R - drop the rope");
    y += 16;
    x11.drawString(10, y, "S - enlarge the sphere. (shift-S to reduce)");
    y += 16;
    x11.drawString(10, y, "1 - pull rope to the left");
    y += 16;
    x11.drawString(10, y, "2 - pull rope to the right");
    y += 16;
    x11.drawString(10, y, "G - gravity");
    y += 16;
    x11.drawString(10, y, "J - Jello cube");
    y += 16;
    x11.drawString(10, y, "Arrow keys - move sphere");
    y += 16;
    x11.drawString(10, y, "Mouse drag sphere with left button");
    y += 16;
    x11.drawString(10, y, "Press Z to Decrease Stiffness");
    y += 16;
    x11.drawString(10, y, "Press J to Increase Stiffness");
}

void render()
{
    x11.clear_screen();
    showMenu();
    show_sphere();
    show_lines();
}

