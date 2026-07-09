//modified by:
//date:
//
//program: lab-11 assignment
//author:  Gordon Griesel
//purpose: Fill areas with noise
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
//type defines
typedef double Flt;
typedef Flt Vec[3];
//some macros
#define random(a) rand()%(a)
#define rnd() (double)rand()/(double)RAND_MAX
#define SWAP(a,b) (a)^=(b);(b)^=(a);(a)^=(b)
#define ABS(a) (((a)<0)?(-(a)):(a))
#define SGN(a) (((a)<0)?(-1):(1))

class Global {
    public:
        int xres, yres;
        int mode;
        int nPixels;
        Vec scale;
        Vec center;
        Flt radius_min;
        Flt radius_max;
        //triangle_vertices;
        Vec tri[3];
        Global() {
            srand((unsigned)time(NULL));
            xres = 800; yres = 800;
            xres = 400; yres = 400;
            mode = 0;
            nPixels = 1;
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
            XStoreName(dpy, win, "3480 lab-11");
            gc = XCreateGC(dpy, win, 0, NULL);
            XMapWindow(dpy, win);
            XSelectInput(dpy, win, ExposureMask | StructureNotifyMask |
                    PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                    KeyPressMask | KeyReleaseMask);
        }
        ~X11() {
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);
        }
        void flush() { XFlush(dpy); }
        void check_resize(XEvent *e) {
            //ConfigureNotify is sent when window size changes.
            if (e->type != ConfigureNotify)
                return;
            XConfigureEvent xce = e->xconfigure;
            g.xres = xce.width;
            g.yres = xce.height;
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
        void set_title() {
            char ts[200];
            sprintf(ts, "3480 lab-11    nPixels: %i  mode: %i",
                    g.nPixels, g.mode);
            XStoreName(dpy, win, ts);
        }
        void clear_screen() {
            XClearWindow(dpy, win);
        }
        void setColor3i(int r, int g, int b) {
            XSetForeground(dpy, gc, ((r<<16) + (g<<8) + b));
        }
        void setColor3f(float r, float g, float b) {
            int c0 = r * 255;
            int c1 = g * 255;
            int c2 = b * 255;
            XSetForeground(dpy, gc, ((c0<<16) + (c1<<8) + c2));
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
        void fillRectangle(int x, int y, int w, int h) {
            XFillRectangle(dpy, win, gc, x, y, w, h);
        }
        void drawRectangle(int x, int y, int w, int h) {
            XDrawRectangle(dpy, win, gc, x, y, w, h);
        }
        void drawText(int x, int y, const char *text) {
            XDrawString(dpy, win, gc, x, y, text, strlen(text));
        }
} x11;

//function prototypes
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();

int main()
{
    int done = 0;
    while (!done) {
        //Check the event queue
        while (x11.getXPending()) {
            //Handle them one-by-one
            XEvent e;
            x11.getXNextEvent(&e);
            x11.check_resize(&e);
            check_mouse(&e);
            done = check_keys(&e);
        }
        for (int i=0; i<g.nPixels; i++)
            render();
        x11.flush();
        usleep(99);
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
    char ppmname[256] = "";
    strcpy(ppmname, filename);
    if (ppmname[0] == '\0') {
        sprintf(ppmname, "./lab11%02i.ppm", picnum);
        while (access(ppmname, F_OK) == 0) {
            picnum++;
            sprintf(ppmname, "./lab11%02i.ppm", picnum);
        }
        picnum++;
    }
    FILE *fpo = fopen(ppmname, "w");
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
        char jpgname[256] = "";
        sprintf(jpgname,"lab11%02i.jpg", picnum);
        char ts[256];
        sprintf(ts, "convert %s -quality 100 %s", ppmname, jpgname);
        system(ts);
        printf("takeScreenshot(%s)\n", jpgname);
        unlink(ppmname);
    }
    XFree(image);
}

void getPerpendicular(double *v1, double *v2)
{
    v2[0] =  v1[1];
    v2[1] = -v1[0];
}

void check_mouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;
    int mx = e->xbutton.x;
    int my = e->xbutton.y;
    //
    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        //A mouse button was pressed.
        if (e->xbutton.button==1) {
            //Left button pressed
        }
        if (e->xbutton.button==3) {
            //Right button pressed. make a point.
        }
    }
    if (savex != mx || savey != my) {
        //mouse moved
        savex = mx;
        savey = my;
    }
}

int check_keys(XEvent *e)
{
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyRelease)
        return 0;
    switch (key) {
        case XK_a:
            takeScreenshot("",0);
            break;
        case XK_0:
            g.mode = 0;
            break;
        case XK_1:
            g.mode = 1;
            break;
        case XK_2:
            g.mode = 2;
            g.center[0] = rand() % g.xres;
            g.center[1] = rand() % g.yres;
            g.radius_min = rand() % 200 + 80;
            break;
        case XK_p:
            g.mode = 11;
            break;
        case XK_s:
            g.mode = 12;
            break;
        case XK_b:
            g.mode = 13;
            break;
        case XK_o:
            g.mode = 14;
            break;
        case XK_e:
            g.mode = 15;
            break;
        case XK_w:
            g.mode = 16;
            break;
        case XK_c:
            x11.clear_screen();
            break;
        case XK_m:
            void showMenu();
            showMenu();
            break;
        case XK_minus:
            if (g.nPixels > 0)
                --g.nPixels;
            break;
        case XK_equal:
            g.nPixels+=10000;
            break;
        case XK_Escape:
            //end the program
            return 1;
    }
    x11.set_title();
    return 0;
}

void physics()
{
    //no physics in this program.
}

void showMenu()
{
    int x=4, y=20;
    const char *text[] = {
        "Program Options",
        "0 = standby",
        "1 = fill screen",
        "2 = fill a disk (circle)",
        "3 = fill a ring",
        "P = Perlin Noise",
        "S = Sine wave",
        "T = Noise with turbulence",
        "O = Noise with octaves",
        ""
    };
    x11.setColor3i(255,255,255);
    int i=0;
    while (text[i][0] != '\0') {
        x11.drawText(x, y, text[i++]);
        y+=16;
    }
    return;
}

void render()
{
    //printf("render mode: %i\n", g.mode);
    int x = random(g.xres);
    int y = random(g.yres);
    x11.setColor3i(200, 20, 20);
    int d0, d1, dist;
    //
    switch(g.mode) {
        case 0:
            return;
        case 1:
            g.mode=1;
            break;
        case 2:
            //disk
            d0 = g.center[0] - x;
            d1 = g.center[1] - y;
            dist = d0*d0+d1*d1;
            if (dist <= (g.radius_min*g.radius_min))
                x11.setColor3i(20, 25, 150);
            break;
        case 3:
            //ring

            break;
        case 11:
            {
                //Perlin noise
                extern float noise2(float vec[2]);

                float fx = (float)x/(float)g.xres*54.0;
                float fy = (float)y/(float)g.yres*54.0;
                float v[2] = {fx,fy};
                static float lo = 9e9 , hi= -9e9;

                float color = noise2(v);
                if(color < lo){
                    lo = color;
                    printf("%lf\n",lo);
                }
                if(color > hi){
                    hi = color;
                    printf("%lf\n",hi);
                }
                color+=fabs(lo);
                color /= fabs(hi-lo);
                int c =color *255;
                x11.setColor3i(c,c,c);
            }
            break;
        case 12:
            {
                //Sine wave function
                //http://www.upvector.com/
                //?section=Tutorials&subsection=Intro%20to%20Procedural%20Textures
                float fx = (float)x/(float)g.xres;
                float color = (1.0 + sin(fx * 50.0))/2.0;
                int c =color *255;
                x11.setColor3i(c,c,c);



            }
            break;
        case 13:
            {
                //Turbulence
                //http://www.upvector.com/
                //?section=Tutorials&subsection=Intro%20to%20Procedural%20Textures
                extern float noise2(float vec[2]);


                float fx = (float)x/(float)g.xres*4.0;
                float fy = (float)y/(float)g.yres*4.0;
                float v[2] = {fx,fy};
                static float lo = 9e9 , hi= -9e9;

                float color = noise2(v);
                if(color < lo){
                    lo = color;
                    printf("%lf\n",lo);
                }
                if(color > hi){
                    hi = color;
                    printf("%lf\n",hi);
                }

                color+=fabs(lo);
                color /= fabs(hi-lo);
                float tcolor = (1.0 +sin(color*50.0))/2.0;
                int c =tcolor *255;
                x11.setColor3i(c,c,c);



            }
            break;
        case 14:
            {
                //Noise with multiple octaves
                extern float noise2(float vec[2]);
                float o=0;
                float scale =500;
                for( int i=0; i <9; i++){
                    float fx = (float)x/(float)g.xres*scale;
                    float fy = (float)y/(float)g.yres*scale;
                    float v[2] = {fx,fy};
                    static float lo = 9e9 , hi= -9e9;

                    float color = noise2(v);
                    if(color < lo){
                        lo = color;
                        printf("%lf\n",lo);
                    }
                    if(color > hi){
                        hi = color;
                        printf("%lf\n",hi);
                    }

                    color+=fabs(lo);
                    color /= fabs(hi-lo);
                    o+=color; 
                    scale/=2;


                }

                float tcolor = o/6;
                int c =tcolor *255;
                x11.setColor3i(c,c,c);

            }
            break;
        case 15:
            {
                //Noise with multiple octaves
                extern float noise2(float vec[2]);
                float o=0;
                float scale =500;
                for( int i=0; i <10; i++){
                    float fx = (float)x/(float)g.xres*scale;
                    float fy = (float)y/(float)g.yres*scale;
                    float v[2] = {fx,fy};
                    static float lo = 9e9 , hi= -9e9;

                    float color = noise2(v);
                    if(color < lo){
                        // to avoid the blue dots  
                        lo = color*3.0;
                        printf("%lf\n",lo);
                    }
                    if(color > hi){
                        // to avoid blue dots multiply by 3
                        hi = color*3.0;
                        printf("%lf\n",hi);
                    }

                    color+=fabs(lo);
                    color /= fabs(hi-lo);
                    o+=color; 
                    scale/=1.78;


                }

                float tcolor = o/6;
                int c =tcolor *255;
                x11.setColor3i(c+20,c,c);

            }
            break;
        case 16:
            {
                //Noise with multiple octaves
                extern float noise2(float vec[2]);
                float o=0;
                float scale =500;
                for( int i=0; i <10; i++){
                    float fx = (float)x/(float)g.xres*scale;
                    float fy = (float)y/(float)g.yres*scale;
                    float v[2] = {fx,fy};
                    static float lo = 9e9 , hi= -9e9;

                    float color = noise2(v);
                    if(color < lo){
                        // to avoid the blue dots  
                        lo = color*3.0;
                        printf("%lf\n",lo);
                    }
                    if(color > hi){
                        // to avoid blue dots multiply by 3
                        hi = color*3.0;
                        printf("%lf\n",hi);
                    }

                    color+=fabs(lo);
                    color /= fabs(hi-lo);
                    o+=color; 
                    scale/=1.78;


                }

                float tcolor = o/6;
                int c =tcolor *255;

                x11.setColor3i(c+20,c,c);

            }
            break;
    }
    x11.drawPixel(x, y);
}















