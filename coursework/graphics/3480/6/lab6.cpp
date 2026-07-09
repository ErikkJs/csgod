//modified by: Erik Soto
//date: 11-20-17
//
//3480 Computer Graphics
//lab5.cpp
//Author: Gordon Griesel
//Date:   2017
//
//This is a simple orthographic ray tracer.
//To render a scene, you must add code to the following functions...
//
//void vecNormalize()
//int rayPlaneIntersect()
//int rayDiskIntersect()
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>

//Variable types...
typedef double Flt;
typedef Flt Vec[3];
#define rnd() ((Flt)rand() / (Flt)RAND_MAX)
//constant definitions...
const int MAXOBJECTS =  300;
enum {
    TYPE_NONE=0,
    TYPE_DISK,
    TYPE_RING,
    TYPE_SPHERE,
    TYPE_TRIANGLE,
    ORTHO,
    PERSPECTIVE,
    SURF_NONE,
    SURF_CHECKER
};

//Ray tracing structures
struct Ray {
    //Ray origin and direction
    Vec o, d;
};

struct Hit {
    //t     = distance to hit point
    //p     = hit point
    //norm  = normal of surface hit
    //color = color of surface hit
    Flt t;
    Vec p, norm, color;
};

struct Object {
    int type;
    Vec center;
    Vec norm;
    Vec tri[3];
    Flt radius, radius2;
    Vec color;
    bool specular;
    int surface;
};

class Global {
    public:
        int xres, yres;
        int mode;
        Object object[MAXOBJECTS];
        int nobjects;
        int color;
        int tilt;
        int checker;
        int saturn;
        int initials;
        int cornell;
        int xAxis;
        int yAxis;
        int zAxis;
        Vec ambient;
        Vec diffuse;
        Vec lightPos;
        Global() {
            srand((unsigned)time(NULL));
            xres = 640, yres = 480;
            mode = 0;
            nobjects = 0;
            color = 0;
            tilt = 0;
            checker = 0;
            saturn = 0;
            initials = 0;
            cornell = 0;
            xAxis = -50.0;
            yAxis = 230;
            zAxis = 50;
            void vecMake(Flt,Flt,Flt,Vec); 
            vecMake (0.2, 0.2, 0.2, ambient);
            vecMake (0.8, 0.8, 0.8, diffuse);
            vecMake (xAxis, yAxis, zAxis, lightPos);
        }
} g;

class X11_wrapper {
    private:
        Display *dpy;
        Window win;
        GC gc;
    public:
        X11_wrapper() {
            //constructor
            if (!(dpy=XOpenDisplay(NULL))) {
                fprintf(stderr, "ERROR: could not open display\n"); fflush(stderr);
                exit(EXIT_FAILURE);
            }
            int scr = DefaultScreen(dpy);
            win = XCreateSimpleWindow(dpy, RootWindow(dpy, scr), 1, 1,
                    g.xres, g.yres, 0, 0x00ffffff, 0x00000000);
            XStoreName(dpy, win, "3480 ray caster.  Press M for menu.");
            gc = XCreateGC(dpy, win, 0, NULL);
            XMapWindow(dpy, win);
            XSelectInput(dpy, win, ExposureMask | StructureNotifyMask |
                    PointerMotionMask | ButtonPressMask |
                    ButtonReleaseMask | KeyPressMask);
        }
        ~X11_wrapper() {
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);
        }
        void getWindowAttributes(int *width, int *height) {
            XWindowAttributes gwa;
            XGetWindowAttributes(dpy, win, &gwa);
            *width = gwa.width;
            *height = gwa.height;
        }
        XImage *getImage(int width, int height) {
            //XImage *image = XGetImage(dpy, win,
            //            0, 0 , width, height, AllPlanes, ZPixmap);
            return XGetImage(dpy, win, 0, 0, width, height, AllPlanes, ZPixmap);
        }
        void checkResize(XEvent *e) {
            //ConfigureNotify is sent when window size changes.
            if (e->type != ConfigureNotify)
                return;
            XConfigureEvent xce = e->xconfigure;
            g.xres = xce.width;
            g.yres = xce.height;
        }
        void clearScreen() {
            //XClearWindow(dpy, win);
            setColor3i(0,0,0);
            XFillRectangle(dpy, win, gc, 0, 0, g.xres, g.yres);
        }
        bool getXPending() {
            return (XPending(dpy));
        }
        void getXNextEvent(XEvent *e) {
            XNextEvent(dpy, e);
        }
        void setColor3i(int r, int g, int b) {
            unsigned long cref = (r<<16) + (g<<8) + b;
            XSetForeground(dpy, gc, cref);
        }
        unsigned long rgbToLong(Vec rgb) {
            //Convert rgb[3] into an integer
            const float range = 255.999;
            int i;
            unsigned long cref = 0;
            for (i=0; i<3; i++) {
                //Don't let a color get too bright.
                if (rgb[i] > 1.0)
                    rgb[i] = 1.0;
                //Shift left 8 bits
                cref <<= 8;
                //Put next color component in low-order byte
                cref += (int)(rgb[i]*range);
            }
            return cref;
        }
        void drawPixel(int x, int y, Vec rgb) {
            unsigned long cref = rgbToLong(rgb);
            XSetForeground(dpy, gc, cref);
            XDrawPoint(dpy, win, gc, x, y);
        }
        void drawText(int x, int y, const char *text) {
            XDrawString(dpy, win, gc, x, y, text, strlen(text));
        }
} x11;

void init(void);
void checkResize(XEvent *e);
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void render(int projection);
//vector functions...
void vecZero(Vec v);
void vecMake(Flt a, Flt b, Flt c, Vec v);
void vecCopy(Vec source, Vec dest);
void vecSub(Vec v0, Vec v1, Vec dest);
void vecNormalize(Vec v);
Flt vecDotProduct(Vec v0, Vec v1);
Flt vecLength(Vec v);


int main(void)
{
    srand((unsigned)time(NULL));
    init();
    x11.clearScreen();
    int done=0;
    while (!done) {
        while (x11.getXPending()) {
            XEvent e;
            x11.getXNextEvent(&e);
            x11.checkResize(&e);
            checkMouse(&e);
            done = checkKeys(&e);
            //render();
        }
    }
    return 0;
}

void takeScreenshot(const char *filename, int reset)
{
    //This function will capture your current X11 window,
    //and save it to a PPM P6 image file.
    //File names are generated sequentially.
    static int picnum = 0;
    int x,y;
    int width, height;
    x11.getWindowAttributes(&width, &height);
    if (reset)
        picnum = 0;
    XImage *image = x11.getImage(width, height);
    //
    //If filename argument is empty, generate a sequential filename...
    char ts[256] = "";
    strcpy(ts, filename);
    if (ts[0] == '\0') {
        sprintf(ts,"./lab5%02i.ppm", picnum);
        picnum++;
    }
    FILE *fpo = fopen(ts, "w");
    if (fpo) {
        fprintf(fpo, "P6\n%i %i\n255\n", width, height);
        for (y=0; y<height; y++) {
            for (x=0; x<width; x++) {
                unsigned long pixel = XGetPixel(image, x, y);
                fputc(((pixel & 0x00ff0000)>>16), fpo);
                fputc(((pixel & 0x0000ff00)>> 8), fpo);
                fputc(((pixel & 0x000000ff)    ), fpo);
            }
        }
        fclose(fpo);
    }
    XFree(image);
}

void initials()
{
    //Setup a triangle mesh below
    Object *o;
    g.nobjects=0;

    //********************************
    //For The B
    //********************************
}

void init_saturn()
{
    if(g.saturn)
        vecMake (-150.0, 200.0, -1000.0, g.lightPos);
    Object *o;
    g.nobjects=0;
    //
    //saturn
    o = &g.object[g.nobjects];
    o->type = TYPE_SPHERE;
    vecMake(0.0, 0.0, -1000.0, o->center);
    vecMake(0.0, 0.0, -1.0, o->norm);
    vecMake(.8,.7,.3, o->color);
    o->radius = 100.0;
    vecNormalize(o->norm);
    o->surface = SURF_NONE;
    g.nobjects++;
    //rings
    Flt r1=120.0, r2=126.0;
    Flt c[3] = {.8, .4, .8};
    for (int i=0; i<16; i++) {
        o = &g.object[g.nobjects];
        o->type = TYPE_RING;
        vecMake(0.0, 0.0, -1000.0, o->center);
        vecMake(0.82, 1.0, 0.5, o->norm);
        vecMake(c[0],c[1],c[2], o->color);
        o->radius = r1;
        o->radius2 = r2;
        vecNormalize(o->norm);
        g.nobjects++;
        r1 = r2 + rnd()*2.0 + 1.0;
        r2 = r1 + rnd()*4.0 + 3.0;
        c[0] += rnd() * .25 - .125;
        c[1] += rnd() * .25 - .125;
        c[2] += rnd() * .25 - .125;
        if (c[0] < 0.0) c[0] = 0.0;
        if (c[0] > 1.0) c[0] = 1.0;
        if (c[1] < 0.0) c[1] = 0.0;
        if (c[1] > 1.0) c[1] = 1.0;
        if (c[2] < 0.0) c[2] = 0.0;
        if (c[2] > 1.0) c[2] = 1.0;
    }
    for (int i=0; i<100; i++) {
        o = &g.object[g.nobjects];
        o->type = TYPE_DISK;
        vecMake(rnd()*g.xres-g.xres/2, rnd()*g.yres-g.yres/2, -2000.0, o->center);
        vecMake(0.0, 0.0, -1.0, o->norm);
        vecMake(.8+rnd(),.8+rnd(),.8+rnd(), o->color);
        o->radius = 0.5 + rnd()*2.0;
        vecNormalize(o->norm);
        o->surface = SURF_NONE;
        g.nobjects++;
    }
}
void init_cornell() 
{
    Object *o;
    initials();
    //g.nobjects=0;
    o = &g.object[g.nobjects];
    o->type = TYPE_SPHERE;
    vecMake(0.0, 10000.00, -1000.0, o->center);
    vecMake(2.0, 2.0, -1.0, o->norm);
    o->specular = false;
    vecMake(.0,.3,.1, o->color);
    o->radius = 9750.0;
    vecNormalize(o->norm);
    o->surface = SURF_NONE;
    g.nobjects++;
    //------------------------------------------
    o = &g.object[g.nobjects];
    o->type = TYPE_SPHERE;
    vecMake(10000.0, 0.0, -1000.0, o->center);
    vecMake(2.0, 2.0, -1.0, o->norm);
    o->specular = false;
    vecMake(.1,.6,.1, o->color);
    o->radius = 9750.0;
    vecNormalize(o->norm);
    o->surface = SURF_NONE;
    g.nobjects++;
    //-----------------------------------------
    o = &g.object[g.nobjects];
    o->type = TYPE_SPHERE;
    vecMake(0.0, -10000.0, -1000.0, o->center);
    vecMake(2.0, 2.0, -1.0, o->norm);
    o->specular = false;
    vecMake(.5,.0,.5, o->color);
    o->radius = 9750.0;
    vecNormalize(o->norm);
    o->surface = SURF_NONE;
    g.nobjects++;
    //------------------------------------------
    o = &g.object[g.nobjects];
    o->type = TYPE_SPHERE;
    vecMake(-10000.0, 0.0, -1000.0, o->center);
    vecMake(2.0, 2.0, -1.0, o->norm);
    o->specular = false;
    vecMake(.1,.3,.9, o->color);
    o->radius = 9750.0;
    vecNormalize(o->norm);
    o->surface = SURF_NONE;
    g.nobjects++;
    //------------------------------------------
    o = &g.object[g.nobjects];
    o->type = TYPE_SPHERE;
    vecMake(0.0, 0.0, -10500.0, o->center);
    vecMake(2.0, 2.0, -1.0, o->norm);
    o->specular = false;
    vecMake(.1,.9,.9, o->color);
    o->radius = 9750.0;
    vecNormalize(o->norm);
    o->surface = SURF_NONE;
    g.nobjects++;
    
    //------------------------------------------
    o = &g.object[g.nobjects];
    o->type = TYPE_TRIANGLE;
    vecMake(100.0,    100.0, -150.0, o->tri[0]);
    vecMake(160.0,  100.0, -100.0, o->tri[1]);
    vecMake(1-0.0,  0.0, -100.0, o->tri[2]);
    o->specular = true;
    void getTriangleNormal(Vec tri[3], Vec norm);
    Vec norm;
    getTriangleNormal(o->tri, norm);
    vecCopy(norm, o->norm);
    vecMake(1,0,0, o->color);
    g.nobjects++;
   
}

void init(void)
{
    if (g.initials) {
        initials();
        return;
    }
    if (g.saturn) {
        init_saturn();
        return;
    }
    if (g.cornell) {
        init_cornell();
        return;
    }
    //Setup some objects
    Object *o;
    g.nobjects=0;

    /*--------------------------------------------------------------------
      o = &g.object[g.nobjects];
      o->type = TYPE_DISK;
      vecMake(-100.0, -120.0, -1000.0, o->center);
      vecMake(0.0, 0.0, -1.0, o->norm);
      o->radius = 100.0;
      if (g.tilt) {
      vecMake(0.0, 1.0, 0.0, o->norm);
      o->specular = false;
      }
      vecMake(.5,.5,.5, o->color);
      if (g.color) {
      vecMake(0, 51.0/255.0, 160.0/255.0, o->color);
      }
      vecNormalize(o->norm);
      o->surface = SURF_CHECKER;
      g.nobjects++;
      */
    //--------------------------------------------------------------------
    o = &g.object[g.nobjects];
    o->type = TYPE_DISK;
    vecMake(50.0, 0.0, -50.0, o->center);
    vecMake(0.0, 0.0, -1.0, o->norm);
    if (g.tilt) {
        o->type = TYPE_SPHERE;
        vecMake(2.0, 2.0, -1.0, o->norm);
        o->specular = true;
    }
    vecMake(.6,.6,.6, o->color);
    if (g.color) {
        vecMake(.9,.1,.1, o->color);
    }
    o->radius = 50.0;
    vecNormalize(o->norm);
    o->surface = SURF_NONE;
    g.nobjects++;
    //--------------------------------------------------------------------
    o = &g.object[g.nobjects];
    o->type = TYPE_DISK;
    vecMake(0.0, 50.0, -80.0, o->center);
    vecMake(0.0, 0.0, -1.0, o->norm);
    if (g.tilt) {
        o->type = TYPE_SPHERE;
        vecMake(-2.4, 0.0, -1.0, o->norm);
        o->specular = true;
    }
    vecMake(.7,.7,.7, o->color);
    if (g.color) {
        vecMake(.5,205.0/255.0,0, o->color);
    }
    o->radius = 50.0;
    vecNormalize(o->norm);
    o->surface = SURF_NONE;
    g.nobjects++;
}

Flt vecDotProduct(Vec v0, Vec v1)
{
    return (v0[0]*v1[0] + v0[1]*v1[1] + v0[2]*v1[2]);
}

void vecCrossProduct(Vec v0, Vec v1, Vec dest)
{
    dest[0] = v0[1]*v1[2] - v1[1]*v0[2];
    dest[1] = v0[2]*v1[0] - v1[2]*v0[0];
    dest[2] = v0[0]*v1[1] - v1[0]*v0[1];
}

void vecZero(Vec v)
{
    v[0] = v[1] = v[2] = 0.0;
}

void vecMake(Flt a, Flt b, Flt c, Vec v)
{
    v[0] = a;
    v[1] = b;
    v[2] = c;
}

void vecCopy(Vec source, Vec dest)
{
    dest[0] = source[0];
    dest[1] = source[1];
    dest[2] = source[2];
}

Flt vecLength(Vec vec)
{
    return sqrt(vecDotProduct(vec, vec));
}

void vecSub(Vec v0, Vec v1, Vec dest)
{
    dest[0] = v0[0] - v1[0];
    dest[1] = v0[1] - v1[1];
    dest[2] = v0[2] - v1[2];
}

void vecNormalize(Vec vec)
{
    //add code here to normalize a vector.
    Flt len = vecLength(vec);
    if (len == 0.0) {
        vecMake(1,0,0,vec);
        return;
    }
    vec[0] /= len;
    vec[1] /= len;
    vec[2] /= len;
}

void getTriangleNormal(Vec tri[3], Vec norm)
{
    Vec v0,v1;
    vecSub(tri[1], tri[0], v0);
    vecSub(tri[2], tri[0], v1);
    vecCrossProduct(v0, v1, norm);
    vecNormalize(norm);
}

void checkMouse(XEvent *e)
{
    if (e->type == ButtonPress) {
        //No mouse in this program.
    }
}

void showMenu()
{
    int y = 20;
    int inc = 16;
    x11.setColor3i(255, 255, 255);
    x11.drawText(10, y, "Menu");
    y += inc;
    x11.setColor3i(255, 255, 0);
    x11.drawText(10, y, "R - Render");
    y += inc;
    x11.drawText(10, y, "1 - Reset scene");
    y += inc;
    x11.drawText(10, y, "2 - Color the disks");
    y += inc;
    x11.drawText(10, y, "3 - Tilt the disks");
    y += inc;
    x11.drawText(10, y, "5 - Saturn");
    y += inc;
    x11.drawText(10, y, "c - Cornell");
    y += inc;
    x11.drawText(10, y, "i - initials");
}

int checkKeys(XEvent *e)
{
    if (e->type == KeyPress) {
        int key = XLookupKeysym(&e->xkey, 0);
        if (key == XK_m) {
            showMenu();
            return 0;
        }
        if (key == XK_a) {
            takeScreenshot("", 0);
            return 0;
        }
        //----------------------------------------
        if (key == XK_1) {
            g.mode=1;
            g.color =
                g.tilt =
                g.checker =
                g.saturn =
                g.cornell = 
                g.initials = 0;
            return 0;
        }
        if (key == XK_2) {
            g.mode=2;
            g.color = 1;
            return 0;
        }
        if (key == XK_3) {
            g.mode=3;
            g.tilt = 1;
            return 0;
        }
        if (key == XK_4) {
            g.mode=4;
            g.checker = 1;
            return 0;
        }
        if (key == XK_5) {
            g.mode=5;
            g.checker = 0;
            g.saturn = 1;
            return 0;
        }
        if (key == XK_6) {
            g.mode=6;
            g.initials = 1;
            return 0;
        }
        if (key == XK_c) {
            init_cornell();
            render(PERSPECTIVE);
            g.cornell = 1;
            return 0;
        }
        //----------------------------------------
        if (key == XK_r) {
            init();
            render(ORTHO);
            return 0;
        }
        if (key == XK_p) {
            init();
            render(PERSPECTIVE);
            return 0;
        }
        if (key == XK_i) {
            initials();
            render(ORTHO);
            return 0;
        }
        if (key == XK_Escape) {
            if (g.mode) {
                g.mode = 0;
                x11.clearScreen();
                return 0;
            }
            return 1;
        }
    }
    return 0; 
}

Flt getLength(Vec p1, Vec p2)
{
    Flt d0 = p2[0] - p1[0];
    Flt d1 = p2[1] - p1[1];
    Flt d2 = p2[2] - p1[2];
    Flt len = sqrt(d0*d0 + d1*d1 + d2*d2);
    return len;	
}

Flt getArea(Vec v0, Vec v1, Vec v2)
{
    Flt a = getLength(v1, v0);
    Flt b = getLength(v2, v1);
    Flt c = getLength(v0, v2);
    Flt s = (a+b+c) / 2.0;
    return (sqrt(s * (s-a) * (s-b) * (s-c)));
}

bool getBarycentric(Vec tri[3], Hit *hit, Flt *u, Flt *v)
{
    Flt area = getArea(tri[0], tri[1], tri[2]);
    Vec pt;
    vecCopy(hit->p, pt);
    Flt area1 = getArea(tri[0], tri[1], pt);
    Flt area2 = getArea(tri[2], tri[0], pt);
    if (area == 0.0) {
        *u = *v = 0.0;
        return false;
    }
    *u = area1 / area;
    *v = area2 / area;
    return (*u >= 0.0 && *v >= 0.0 && *u + *v <= 1.0);
}

bool pointInTriangle(Vec tri[3], Vec p, Flt *u, Flt *v)
{
    //source: http://blogs.msdn.com/b/rezanour/archive/2011/08/07/
    //
    //This function determines if point p is inside triangle tri.
    //   step 1: 3D half-space tests
    //   step 2: find barycentric coordinates
    //
    Vec cross0, cross1, cross2;
    Vec ba, ca, pa;
    //setup sub-triangles
    vecSub(tri[1], tri[0], ba);
    vecSub(tri[2], tri[0], ca);
    vecSub(p, tri[0], pa);
    //This is a half-space test
    vecCrossProduct(ca, pa, cross1);
    vecCrossProduct(ca, ba, cross0);
    if (vecDotProduct(cross0, cross1) < 0.0)
        return 0;
    //This is a half-space test
    vecCrossProduct(ba,pa,cross2);
    vecCrossProduct(ba,ca,cross0);
    if (vecDotProduct(cross0, cross2) < 0.0)
        return 0;
    //Point is within 2 half-spaces
    //Get area proportions
    //Area is actually length/2, but we just care about the relationship.
    Flt areaABC = vecLength(cross0);
    Flt areaV = vecLength(cross1);
    Flt areaU = vecLength(cross2);
    *u = areaU / areaABC;
    *v = areaV / areaABC;
    //return true if valid barycentric coordinates.
    return (*u >= 0.0 && *v >= 0.0 && *u + *v <= 1.0);
}

void sphereNormal(Vec hitPoint, Vec center, Vec norm)
{
    //Calc normal at hit
    norm[0] = hitPoint[0] - center[0];
    norm[1] = hitPoint[1] - center[1];
    norm[2] = hitPoint[2] - center[2];
    vecNormalize(norm);
}


int raySphereIntersect(Object *o, Ray *ray, Hit *hit)
{
    //printf("raySphereIntersect()...\n");
    //Determine if and where a ray intersects a sphere.
    //
    // sphere equation:
    // (p - c) * (p - c) = r * r
    //
    // where:
    // p = point on sphere surface
    // c = center of sphere
    //
    // ray equation:
    // o + t*d
    //
    // where:
    //   o = ray origin
    //   d = ray direction
    //   t = distance along ray, or scalar
    //
    // substitute ray equation into sphere equation
    //
    // (o + t*d - c) * (o + t*d - c) - r * r = 0
    //
    // we want it in this form:
    // a*t*t + b*t + c = 0
    //
    // (o + d*t - c)
    // (o + d*t - c)
    // -------------
    // o*o + o*d*t - o*c + o*d*t + d*t*d*t - d*t*c - o*c + c*d*t + c*c
    // d*t*d*t + o*o + o*d*t - o*c + o*d*t - d*t*c - o*c + c*d*t + c*c
    // d*t*d*t + 2(o*d*t) - 2(c*d*t) + o*o - o*c - o*c + c*c
    // d*t*d*t + 2(o-c)*d*t + o*o - o*c - o*c + c*c
    // d*t*d*t + 2(o-c)*d*t + (o-c)*(o-c)
    //
    // t*t*d*d + t*2*(o-c)*d + (o-c)*(o-c) - r*r
    //
    // a = dx*dx + dy*dy + dz*dz
    // b = 2(ox-cx)*dx + 2(oy-cy)*dy + 2(oz-cz)*dz
    // c = (ox-cx)*(ox-cx) + (oy-cy)*(oy-cy) + (oz-cz)*(oz-cz) - r*r
    //
    // now put it in quadratic form:
    // t = (-b +/- sqrt(b*b - 4ac)) / 2a
    //
    //
    //1. a, b, and c are given to you just above.
    //2. Create variables named a,b,c, and assign the values you see above.
    //3. Look how a,b,c are used in the quadratic equation.
    //4. Make your code solve for t.
    //5. Remember, a quadratic can have 0, 1, or 2 solutions.
    //
    Flt a = ray->d[0]*ray->d[0] + ray->d[1]*ray->d[1] + ray->d[2]*ray->d[2];
    Flt b = 2.0*(ray->o[0]-o->center[0])*ray->d[0] +
        2.0*(ray->o[1]-o->center[1])*ray->d[1] +
        2.0*(ray->o[2]-o->center[2])*ray->d[2];
    Flt c = (ray->o[0]-o->center[0])*(ray->o[0]-o->center[0]) +
        (ray->o[1]-o->center[1])*(ray->o[1]-o->center[1]) +
        (ray->o[2]-o->center[2])*(ray->o[2]-o->center[2]) -
        o->radius*o->radius;
    Flt t0,t1;
    //discriminant
    Flt disc = b * b - 4.0 * a * c;
    if (disc < 0.0) return 0;
    disc = sqrt(disc);
    t0 = (-b - disc) / (2.0*a);
    t1 = (-b + disc) / (2.0*a);
    if (t0 > 0.0) {
        hit->p[0] = ray->o[0] + ray->d[0] * t0;
        hit->p[1] = ray->o[1] + ray->d[1] * t0;
        hit->p[2] = ray->o[2] + ray->d[2] * t0;
        sphereNormal(hit->p, o->center, hit->norm);
        hit->t = t0;
        return 1;
    }
    if (t1 > 0.0) {
        hit->p[0] = ray->o[0] + ray->d[0] * t1;
        hit->p[1] = ray->o[1] + ray->d[1] * t1;
        hit->p[2] = ray->o[2] + ray->d[2] * t1;
        sphereNormal(hit->p, o->center, hit->norm);
        hit->t = t1;
        return 1;
    }
    return 0;
}

void reflect(Vec I, Vec N, Vec R)
{
    //I = incident vector
    //N = the surface normal
    //R = reflected ray
    Flt dot = -vecDotProduct(I, N);
    Flt len = 2.0 * dot;
    R[0] = len * N[0] + I[0];
    R[1] = len * N[1] + I[1];
    R[2] = len * N[2] + I[2];
}



int rayPlaneIntersect(Vec center, Vec normal, Ray *ray, Hit *hit)
{
    //http://mathworld.wolfram.com/Plane.html
    //
    //Where does the ray intersect the plane?
    //
    //plane equation:
    //
    // (p - center) . normal = 0.0
    //
    //where:
    //center = known point on plane
    //normal = normal vector of plane
    //
    //ray equation:
    //
    // O + tD
    //
    //where:
    //   ray origin = O
    //   ray direction = D
    //
    //Substitute ray equation into plane equation,
    //and solve for t
    //
    // (O + t * D - center) . normal = 0.0               (substitution)
    // 
    // (t * D + O - center) . normal = 0.0               (commutative)
    // 
    // t * (D . normal) + (O - center) . normal = 0.0    (distributive)
    // 
    // t * (D . normal) = - (O - center) . normal        (subtraction)
    // 
    // t = - ((O - center) . normal) / (D . normal)      (division)
    // 
    // notes: period is dot product
    //        solve for t
    //        populate hit->t and hit->p[] below...
    //        remember the unary minus sign above
    Vec v0;
    v0[0] = ray->o[0] - center[0];
    v0[1] = ray->o[1] - center[1];
    v0[2] = ray->o[2] - center[2];
    Flt dot1 = vecDotProduct(v0, normal);
    if (dot1 == 0.0)
        return 0;
    Flt dot2 = vecDotProduct(ray->d, normal);
    if (dot2 == 0.0)
        return 0;
    if (hit->t < 0.0)
        return 0;
    hit->t = -(dot1 / dot2);
    hit->p[0] = ray->o[0] + hit->t * ray->d[0];
    hit->p[1] = ray->o[1] + hit->t * ray->d[1];
    hit->p[2] = ray->o[2] + hit->t * ray->d[2];
    return 1;
}

int rayTriangleIntersect(Object *o, Ray *ray, Hit *hit)
{
    if (rayPlaneIntersect(o->tri[0], o->norm, ray, hit)) {
        Flt u,v;
        //Flt w;
        if (pointInTriangle(o->tri, hit->p, &u, &v)) {
            //w = 1.0 - u - v;
            return 1;
        }
    }
    return 0;
}

int rayDiskIntersect(Object *o, Ray *ray, Hit *hit)
{
    //Does the ray intersect the disk's plane?
    if (rayPlaneIntersect(o->center, o->norm, ray, hit)) {
        //Yes.
        //Check that the hit point is within the disk radius
        //Use the point hit instead of the ray origin
        Flt d0, d1, d2, dist;
        d0 = o->center[0] - hit->p[0];
        d1 = o->center[1] - hit->p[1];
        d2 = o->center[2] - hit->p[2];
        //d2 = 0.0;
        dist = sqrt(d0*d0 + d1*d1 + d2*d2);
        if (dist <= o->radius) {
            //Hit is within radius
            return 1;
        }
    }
    return 0;
}

int rayRingIntersect(Object *o, Ray *ray, Hit *hit)
{
    if (rayPlaneIntersect(o->center, o->norm, ray, hit)) {
        Flt d0 = o->center[0] - hit->p[0];
        Flt d1 = o->center[1] - hit->p[1];
        Flt d2 = o->center[2] - hit->p[2];
        Flt dist = sqrt(d0*d0 + d1*d1 + d2*d2);
        if (dist >= o->radius && dist <= o->radius2) {
            return 1;
        }
    }
    return 0;
}

bool getShadow(Vec p) {
    Ray ray;
    //Vec v;
    vecSub(g.lightPos, p, ray.d);
    vecCopy(p, ray.o);
    Flt lightDistance = vecLength(ray.d);
    vecNormalize(ray.d);
    //nudge the ray
    ray.o[0] += ray.d[0] * 0.001;
    ray.o[1] += ray.d[1] * 0.001;
    ray.o[2] += ray.d[2] * 0.001;

    Hit hit;
    Object *o;
    for (int i=0; i<g.nobjects; i++) {
        o = &g.object[i];
        switch (o->type) {
            case TYPE_DISK:
                if (rayDiskIntersect(o, &ray, &hit)) {
                    if (hit.t < lightDistance) {
                        return true;
                    }
                }
                break;
            case TYPE_RING:
                if (rayRingIntersect(o, &ray, &hit)) {
                    if (hit.t < lightDistance) {
                        return true;
                    }
                }
                break;
            case TYPE_SPHERE:
                if (raySphereIntersect(o, &ray, &hit)) {
                    if (hit.t < lightDistance) {
                        return true;
                    }
                }

                break;
        }
    }
    return false;
}

void trace(Ray *ray, Vec rgb, Flt weight) 
{
    if (weight < 0.1)
        return;
    //Trace a ray through the scene.
    int i;
    Hit hit, closehit;
    Object *o;
    int h = -1;
    closehit.t = 9e9;
    for (i=0; i<g.nobjects; i++) {
        o = &g.object[i];
        switch (o->type) {
            case TYPE_DISK:
                if (rayDiskIntersect(o, ray, &hit)) {
                    if (hit.t < closehit.t) {
                        closehit.t = hit.t;
                        vecCopy(hit.p, closehit.p);
                        vecCopy(o->color, closehit.color);
                        vecCopy(o->norm, closehit.norm);
                        h=i;
                    }
                }
                break;
            case TYPE_RING:
                if (rayRingIntersect(o, ray, &hit)) {
                    if (hit.t < closehit.t) {
                        closehit.t = hit.t;
                        vecCopy(hit.p, closehit.p);
                        vecCopy(o->color, closehit.color);
                        vecCopy(o->norm, closehit.norm);
                        h=i;
                    }
                }
                break;
            case TYPE_SPHERE:
                if (raySphereIntersect(o, ray, &hit)) {
                    if (hit.t < closehit.t) {
                        closehit.t = hit.t;
                        vecCopy(hit.p, closehit.p);
                        vecCopy(o->color, closehit.color);
                        sphereNormal(hit.p, o->center, closehit.norm);
                        h=i;
                    }
                }
                break;
            case TYPE_TRIANGLE:
                if (rayTriangleIntersect(o, ray, &hit)) {
                    if (hit.t < closehit.t) {
                        closehit.t = hit.t;
                        vecCopy(hit.p, closehit.p);
                        vecCopy(o->color, closehit.color);
                        vecCopy(o->norm, closehit.norm);
                        h=i;
                    }
                }
                break;
        }
    }
    if (h < 0) {
        //ray did not hit an object.
        return;
    }
    //The ray hit an object.
    //Set the color of the pixel to the color of the object.
    o = &g.object[h];
    if (o->surface == SURF_CHECKER) {
        int x = 1000 + (closehit.p[0]/2) / 60.0;
        int y = 1000 + (closehit.p[2]/2) / 60.0;
        x = x % 2;
        y = y % 2;
        if (x==y) {
            closehit.color[0] = 0.3;
            closehit.color[1] = 0.3;
            closehit.color[2] = 0.9;
        } else {
            closehit.color[0] = 0.3;
            closehit.color[1] = 0.7;
            closehit.color[2] = 0.8;
        }
    }

    if (o->specular == true) {
        Vec trgb = {0,0,0};
        Ray tray;
        vecCopy(closehit.p, tray.o);
        reflect(ray->d, closehit.norm, tray.d);

        trace(&tray, trgb, weight * 0.5);
        rgb[0] += trgb[0] * weight;
        rgb[1] += trgb[1] * weight;
        rgb[2] += trgb[2] * weight;
    }

    //ambient light
    rgb[0] += closehit.color[0] * g.ambient[0];
    rgb[1] += closehit.color[1] * g.ambient[1];
    rgb[2] += closehit.color[2] * g.ambient[2];

    bool isInShadow = getShadow(closehit.p);
    if (!isInShadow) {
        //diffuse light
        Vec v;
        vecSub(g.lightPos, closehit.p, v);
        vecNormalize(v);
        vecNormalize(closehit.norm);
        Flt dot = vecDotProduct(v, closehit.norm);

        if (dot < 0.0) dot = 0.0;
        rgb[0] += closehit.color[0] * g.diffuse[0] * dot;
        rgb[1] += closehit.color[1] * g.diffuse[1] * dot;
        rgb[2] += closehit.color[2] * g.diffuse[2] * dot;
    }
    //rgb[0] = closehit.color[0];
    //rgb[1] = closehit.color[1];
	//rgb[2] = closehit.color[2];
	return;
}

void setupRay(Vec eye, Vec pixel, Ray *ray)
{
	//Make a ray from eye through a screen pixel
	vecCopy(eye, ray->o);
	vecSub(pixel, eye, ray->d);
	vecNormalize(ray->d);
}

void render(int projection)
{
	//Starting position of pixels is bottom-left corner of screen.
	//Pixel at origin (0,0) is in the middle of the screen.
	Flt xStart = -g.xres / 2.0;
	Flt yStart = -g.yres / 2.0;
	Flt xStep = 1.0;
	Flt yStep = 1.0;
	Vec eye, pixel, rgb;
	Ray ray;
	//Assume orthographic projection.
	//Set eye in front of each pixel.
	//Make a ray straight through each screen pixel.
	pixel[1] = yStart;
	for (int i=g.yres-1; i>=0; i--) {
		pixel[0] = xStart;
		for (int j=0; j<g.xres; j++) {
			vecCopy(pixel, eye);
			if (projection == PERSPECTIVE) {
				//Place code here for perspective projection
				eye[0] = eye[1] = 0.0;
			}
		    //eye[0] = eye[1] = 0.0;
			//Move the eye back from the screen.
			eye[2] = 1000.0;
			//Cast the ray through the pixel.
			setupRay(eye, pixel, &ray);
			vecZero(rgb);
			trace(&ray, rgb, 1.0);
			x11.drawPixel(j, i, rgb);
			pixel[0] += xStep;
		}
		pixel[1] += yStep;
	}
}


