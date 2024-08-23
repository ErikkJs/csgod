//
//program: xwin.cpp
//author:  Gordon Griesel
//purpose: Framework for graphics
//         Using X11 (Xlib) for drawing
//         practice program for students
//
//things to do...
//
//add member functions to the X11 class...
//  draw a point
//  draw a rectangle
//  draw a circle or elipse
//  fill a rectangle
//  draw some text
//
//demonstrate your functions
//  click the mouse
//  press the keys
//  draw stuff
//  display some text with instructions
//  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

class Global {
    public:
        int xres, yres;
        int grayscale;
        Global() {
            srand((unsigned)time(NULL));
            xres = 640;
            yres = 480;
            grayscale=0;
        }
} g;
class Image{
    public:
        int width, height;
        unsigned char *data;
        ~Image(){
            delete [] data;
        }

        Image(){
            system("convert goat.jpg goat.ppm");
            FILE *fpi =fopen("goat.ppm","r");
            if(fpi){
                char line[200];
                fgets(line,200,fpi);
                fgets(line,200,fpi);
                while(line[0] == '#'){
                    fgets(line,200,fpi);
                }

                sscanf(line, "%i %i",&width,&height);
                fgets(line,200,fpi);
                int n = width * height * 3;
                data = new unsigned char[n];
                for (int i =0; i<n; i++)
                {
                    data[i] = fgetc(fpi);

                }
                fclose(fpi);


            }
            else{
                printf("ERROOR opening input file");
                exit(0);


                unlink("goat.ppm");
            }


        }
}
img;

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
                    PointerMotionMask | ButtonPressMask |
                    ButtonReleaseMask | KeyPressMask);
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
        void drawText(int x,int y, const char *text){
            XDrawString(dpy,win,gc,x,y,text,strlen(text));
        }

        void drawImage(Image *img, int x, int y){

            int xoffset= img->width/2;
            int yoffset =img->height/2;

            for(int i =0;i<img->height;i++)
            {
                for(int j=0; j<img->width; j++)
                { 
                    int r =img->data[i*img->width*3 + j*3+0];
                    int g2 =img->data[i*img->width*3 + j*3+1];
                    int b =img->data[i*img->width*3 + j*3+2];
                    setColor3i(r,g2,b);
                    if(g.grayscale ==1){
                        int c =(r+g2+b)/3;
                        setColor3i(c,c,c);
                    }

                    XDrawPoint(dpy,win,gc,j+x-xoffset,i+y-yoffset);
                }

            }


        }

} x11;

//function prototypes
void init();
void init_xwindows();
void cleanup_xwindows();
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();
void clear_screen();


int main(void)
{
    int done = 0;
    while (!done) {
        //Check the event queue
        while (x11.getXPending()) {
            //Handle them one-by-one
            XEvent e;
            //XNextEvent(dpy, &e);
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
    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        //A mouse button was pressed.
        //Log("ButtonPress %i %i\n", e->xbutton.x, e->xbutton.y);
        if (e->xbutton.button==1) {
            //Left button pressed

        }
        if (e->xbutton.button==3) {
            //Right button pressed

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
    switch (key) {
        case XK_Escape:
            //program is ending
            return 1;
        case XK_a:
            g.grayscale =1;
            break;
        case XK_r:

            break;
    }
    return 0;
}

void physics(void)
{
    //no physics yet.

}

void render(void)
{
    //render function is always at the bottom of program.
    x11.setColor3i(255, 255, 0);
    x11.drawText(10,20,"press r to rotate");
    //	x11.drawLine(100, 100, 200, 200);
    // x11.drawPixel(g.xres/2,g.yres/2);
    // x11.drawPixel(g.xres/2+1,g.yres/2+1);
    x11.drawImage(&img, g.xres/2,g.yres/2);
}

