//3350
//program: rambo.cpp
//authors: Joshua Cancellier, Kuljit Singh, Fernando Montes de Oca, Erik Soto, Rafael Noriega
//walk Framework byd:  Gordon Griesel
//date:    summer 2017 - 2018
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "Global.h"
#include "SpriteSheet.h"
#include "Timers.h"
#include "Character.h"
#include "Enemy1.h"
#include "Bat.h"
#include "Hulk.h"
#include "Explosion.h"
#include "joshuaC.h"
#include "erikS.h"
#include "kuljitS.h"
#include "fernandoM.h"
#include "fonts.h"
#include "Bullet.h"
#include "PowerUp.h"
#include "rafaelN.h"
#include <vector>

#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND
//globals
int nbullets = 0;
int keys[65536];
//const float timeslice = 1.0f;
float gravity = -0.5f;
float cx = 100; //Sprite x postion
float cy = 200; //Sprite y postion
int flipped = 0;
bool debug_mode = false;
bool display_hitbox = false;
bool hit = true;
int gameState = MAINMENU;
int selectedOption = NEWGAME;
int MAX_BULLETS = 30;
int MAX_JUGGERNAUTS = 20;
int MAX_PIRATES = 30;
int MAX_BATS = 50;
int level = 1;
int nPirates = 0;
int nBats = 0;
int nJuggernauts = 0;
int done = 0;
int cursorPosition[2];
std::string player_name="";
bool getName = true;
  
#define ALPHA 1

//load textures (filename, rows, columns)
SpriteSheet img[] = {SpriteSheet("images/walk.gif", 4, 7), 
                    SpriteSheet("images/ramboLogo.gif", 1, 1),
                    SpriteSheet("images/background.gif", 1, 1),
                    SpriteSheet("images/spacePirate.gif", 1, 8),
                    SpriteSheet("images/bat.gif", 1, 6),
                    SpriteSheet("images/batShiny.gif", 1, 6),
                    SpriteSheet("images/explosion.gif", 9, 9),
                    SpriteSheet("images/aimCursor.gif", 3, 1),
                    SpriteSheet("images/mushroomHead.gif", 1, 1),
                    SpriteSheet("images/healthBar.gif", 1, 5),
                    SpriteSheet("images/enemyHealthBars.gif", 4, 4),
                    SpriteSheet("images/powerUps.gif", 1, 6),
                    SpriteSheet("images/hulk.gif", 2, 6),
                    SpriteSheet("images/wasted.gif", 1, 1),
                    SpriteSheet("images/platform.gif", 1, 1)
					};


//Global class
Global g;
Level lev;
//Main Character (rambo)
Character rambo(0);
//should rename enemy1 to pirates
Enemy1* pirates = new Enemy1[MAX_PIRATES];
//bats
Bat* bats = new Bat[MAX_BATS];
//juggrnauts
Hulk* juggernauts = new Hulk[MAX_JUGGERNAUTS];

std::vector<Explosion> explosions;
std::vector<PowerUp> powerUps;

//Setup timers
Timers timers;
Platform platforms[3]= {Platform(50,200,50,210,200,210,200,200),
   Platform(g.xres-50,200,g.xres-50,210,g.xres-200,210,g.xres-20,200),
   Platform((g.xres/2)+100,200,(g.xres/2)+100,210,(g.xres/2)-100,210,(g.xres/2)-100,200)};


class X11_wrapper
{
  private:
    Display *dpy;
    Window win;

  public:
    X11_wrapper()
    {
        GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
        //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
        XSetWindowAttributes swa;
        setupScreenRes(g.xres, g.yres);
        dpy = XOpenDisplay(NULL);
        if (dpy == NULL) {
            printf("\n\tcannot connect to X server\n\n");
            exit(EXIT_FAILURE);
        }
        Window root = DefaultRootWindow(dpy);
        XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
        if (vi == NULL) {
            printf("\n\tno appropriate visual found\n\n");
            exit(EXIT_FAILURE);
        }
        Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
        swa.colormap = cmap;
        swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
            PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |             
			StructureNotifyMask | SubstructureNotifyMask;
        win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
                            vi->depth, InputOutput, vi->visual,
                            CWColormap | CWEventMask, &swa);
        GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
        glXMakeCurrent(dpy, win, glc);
        show_mouse_cursor(0);
        //disable autorepeat on key presses
        int mCheck;
        XkbSetDetectableAutoRepeat(dpy, false, &mCheck);
        setTitle();
    }
    ~X11_wrapper()
    {
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    }
    void setTitle()
    {
        //Set the window title bar.
        XMapWindow(dpy, win);
        XStoreName(dpy, win, "Rambo");
    }
    void setupScreenRes(const int w, const int h)
    {
        g.xres = w;
        g.yres = h;
    }
    void reshapeWindow(int width, int height)
    {
        //window has been resized.
        setupScreenRes(width, height);
        glViewport(0, 0, (GLint)width, (GLint)height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glOrtho(0, g.xres, 0, g.yres, -1, 1);
        setTitle();
    }
    void checkResize(XEvent *e)
    {
        //The ConfigureNotify is sent by the
        //server if the window is resized.
        if (e->type != ConfigureNotify)
            return;
        XConfigureEvent xce = e->xconfigure;
        if (xce.width != g.xres || xce.height != g.yres)
        {
            //Window size did change.
            reshapeWindow(xce.width, xce.height);
        }
    }
    bool getXPending()
    {
        return XPending(dpy);
    }
    XEvent getXNextEvent()
    {
        XEvent e;
        XNextEvent(dpy, &e);
        return e;
    }
    void swapBuffers()
    {
        glXSwapBuffers(dpy, win);
    }
	void show_mouse_cursor(const int onoff) {
        if (onoff) {
            //this removes our own blank cursor.
            XUndefineCursor(dpy, win);
            return;
        }
        //vars to make blank cursor
        Pixmap blank;
        XColor dummy;
        char data[1] = {0};
        Cursor cursor;
        //make a blank cursor
        blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
        if (blank == None)
            std::cout << "error: out of memory." << std::endl;
        cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
        XFreePixmap(dpy, blank);
        //this makes you the cursor. then set it using this function
        XDefineCursor(dpy, win, cursor);
        //after you do not need the cursor anymore use this function.
        //it will undo the last change done by XDefineCursor
        //(thus do only use ONCE XDefineCursor and then XUndefineCursor):
    }

} x11;

//function prototypes
void initOpengl(void);
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics(void);
void render(void);

int main(void)
{
    if (getName) {
        printf("Enter name for leaderboard: ");
        std::cin >> player_name;
        getName = false;
    }
    initOpengl();
    #ifdef USE_OPENAL_SOUND
    alutInit(0,NULL);
    #endif
    while (!done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.checkResize(&e);
            checkMouse(&e);
            done = checkKeys(&e);
        }
        physics();
        render();
        x11.swapBuffers();
    }
    //cleanup_fonts();
    #ifdef USE_OPENAL_SOUND
    alutExit();
    #endif
     
   return 0;
}

//This function removes background from sprite sheet
unsigned char *buildAlphaData(SpriteSheet *img)
{
    //add 4th component to RGB stream...
    int i;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    unsigned char a, b, c;
    //use the first pixel in the image as the transparent color.
    unsigned char t0 = *(data + 0);
    unsigned char t1 = *(data + 1);
    unsigned char t2 = *(data + 2);
    for (i = 0; i < img->width * img->height * 3; i += 3)
    {
        a = *(data + 0);
        b = *(data + 1);
        c = *(data + 2);
        *(ptr + 0) = a;
        *(ptr + 1) = b;
        *(ptr + 2) = c;
        *(ptr + 3) = 1;
        //if pixel is color of background set the 4th component to 0
        if (a == t0 && b == t1 && c == t2)
            *(ptr + 3) = 0;
        //-----------------------------------------------

        //move to next pixel
        ptr += 4;
        data += 3;
    }
    return newdata;
}

void initOpengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, g.xres, g.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    //
    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
    //
    //load the images file into a ppm structure.
    //
    int w = img[0].width;
    int h = img[0].height;
    //printf("width: %d \n height: %d\n", w, h);
    //
    //create opengl texture elements
    //1st param: # of textures
    //2nd param: specifies where generated texture names are stored
    glGenTextures(1, &g.walkTexture);
    glGenTextures(1, &g.ramboLogoTexture);
    glGenTextures(1, &g.RamboTexture);
    glGenTextures(1, &g.spacePirateTexture);
    glGenTextures(1, &g.batEnemyTexture);
    glGenTextures(1, &g.batEnemyShinyTexture);
    glGenTextures(1, &g.explosionTexture);
    glGenTextures(1, &g.cursorTexture);
    glGenTextures(1, &g.powerUpTexture);
    glGenTextures(1, &g.healthBarTexture);
    glGenTextures(1, &g.enemyHealthBarTexture);
    glGenTextures(1, &g.powerUpsTexture);
    glGenTextures(1, &g.hulkTexture);
    glGenTextures(1, &g.wasted);
    glGenTextures(1, &g.platform);

    
    //-------------------------------------------------------------------------
    //silhouette
    //this is similar to a sprite graphic
    //
    glBindTexture(GL_TEXTURE_2D, g.walkTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    unsigned char *walkData = buildAlphaData(&img[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);
    //free(walkData);
    //unlink("./images/walk.ppm");
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    //ramboLogo
    //this is similar to a sprite graphic
    //
    w = img[1].width;
    h = img[1].height;
    glBindTexture(GL_TEXTURE_2D, g.ramboLogoTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);
    //Background texture
    w = img[2].width;
    h = img[2].height;
    glBindTexture(GL_TEXTURE_2D, g.RamboTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);
    //Enemy1 Texture (Space Pirate)
    w = img[3].width;
    h = img[3].height;
    glBindTexture(GL_TEXTURE_2D, g.spacePirateTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);

    // Bat enemy Texture 
    w = img[4].width;
    h = img[4].height;
    glBindTexture(GL_TEXTURE_2D, g.batEnemyTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);

    // Bat shiny enemy Texture 
    w = img[5].width;
    h = img[5].height;
    glBindTexture(GL_TEXTURE_2D, g.batEnemyShinyTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[5]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);


    // explosion texture 
    w = img[6].width;
    h = img[6].height;
    glBindTexture(GL_TEXTURE_2D, g.explosionTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[6]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);

    // cursor texture 
    w = img[7].width;
    h = img[7].height;
    glBindTexture(GL_TEXTURE_2D, g.cursorTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[7]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);


    // power up texture 
    w = img[8].width;
    h = img[8].height;
    glBindTexture(GL_TEXTURE_2D, g.powerUpTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[8]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);

    // health bar texture 
    w = img[9].width;
    h = img[9].height;
    glBindTexture(GL_TEXTURE_2D, g.healthBarTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[9]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);
    
    w = img[10].width;
    h = img[10].height;
    glBindTexture(GL_TEXTURE_2D, g.enemyHealthBarTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[10]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);

    // power ups texture 
    w = img[11].width;
    h = img[11].height;
    glBindTexture(GL_TEXTURE_2D, g.powerUpsTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[11]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);
    // hulk texture 
    w = img[12].width;
    h = img[12].height;
    glBindTexture(GL_TEXTURE_2D, g.hulkTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[12]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);
 // wasted texture 
    w = img[13].width;
    h = img[13].height;
    glBindTexture(GL_TEXTURE_2D, g.wasted);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[13]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);
// wasted texture 
    w = img[14].width;
    h = img[14].height;
    glBindTexture(GL_TEXTURE_2D, g.platform);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //must build a new set of data...
    //This is where the texture is initialized in OpenGL (full sheet)
    walkData = buildAlphaData(&img[14]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, walkData);


    
    //free(walkData);
    //unlink("./images/walk.ppm");
    //-------------------------------------------------------------------------
}

void checkMouse(XEvent *e)
{
    switch (gameState)
    {
    case MAINMENU:
        checkMouseMainMenu(e);
        break;
    case PAUSEMENU:
        break;
    case DEATH:
        break;
    case INGAME:
        //Did the mouse move?
        //Was a mouse button clicked?
        static int savex = 0;
        static int savey = 0;
        //
        if (e->type == ButtonRelease) {
            return;
        }
        if (e->type == ButtonPress) {
            if (e->xbutton.button == 1) {
                //Left button is down
            }
            if (e->xbutton.button == 3) {
                //Right button is down
            }
        }
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
            //Mouse moved
            savex = e->xbutton.x;
            savey = e->xbutton.y;
        }
        cursorPosition[0] = savex;
        cursorPosition[1] = savey;

        break;
    default:
        printf("Fatal Error in game state\n\n");
        exit(1);
    }
}

int checkKeys(XEvent *e)
{

    int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
    if (e->type == KeyPress) {
        keys[key] = 1;
    }
    if (e->type == KeyRelease) {
        keys[key] = 0;
    }
    switch (gameState)
    {
    case MAINMENU:
        break;
    case PAUSEMENU:
        break;
    case DEATH:
	break;
    case INGAME:
               //Toggle debug mode
        if (e->type == KeyPress && key == XK_h)
        {
            debug_mode = !debug_mode;
        }

        //Toggle hitboxes
        if (e->type == KeyPress && key == XK_6)
        {
            display_hitbox = !display_hitbox;
        }

        //(void)shift;
        switch (key)
        {
        case XK_w:
            timers.recordTime(&timers.walkTime);
            g.walk ^= 1;
            break;
        case XK_Left:
            break;
        case XK_Right:
            break;
        case XK_Up:
            break;
        case XK_Down:
            break;
        case XK_equal:
            g.delay -= 0.005;
            if (g.delay < 0.005)
                g.delay = 0.005;
            break;
        case XK_minus:
            //
            rambo.setHeight(rambo.getHeight() * .5);
            rambo.setWidth(rambo.getWidth() * .5);
            break;
        case XK_u:
            //
            rambo.setHeight(rambo.getHeight() * 2);
            rambo.setWidth(rambo.getWidth() * 2);
            break;
        case XK_Escape:
            gameState = PAUSEMENU;
            selectedOption = RESUMEGAME;
            //printf("EXITING GAME\n");
//            delete [] enemies;
  //          enemies = NULL;
            //return 1;
            break;
        default:
            break;
        }
        return 0;
        break;
    default:
        printf("FATAL ERROR IN GAME STATE\n\n");
        exit(1);
    }

    return 0;
}

Flt VecNormalize(Vec vec)
{
    Flt len, tlen;
    Flt xlen = vec[0];
    Flt ylen = vec[1];
    Flt zlen = vec[2];
    len = xlen * xlen + ylen * ylen + zlen * zlen;
    if (len == 0.0) {
        MakeVector(0.0, 0.0, 1.0, vec);
        return 1.0;
    }
    len = sqrt(len);
    tlen = 1.0 / len;
    vec[0] = xlen * tlen;
    vec[1] = ylen * tlen;
    vec[2] = zlen * tlen;
    return (len);
}

void physics(void)
{
    switch (gameState)
    {
    case MAINMENU:
        checkKeysMainMenu();
        break;
    case PAUSEMENU:
        checkKeysPauseMenu();
        break;
    case DEATH:
	checkKeysDeath();
	break;
    case INGAME:
        joshuaCInput();
        kuljitS_physics();
        fernandoPhysics();
	teleportCheck();
        spaceButton();
	platformPhysics();
        break;
    default:
        printf("FATAL ERROR IN GAME STATE\n\n");
        exit(1);
    }
}

void render(void)
{
    switch (gameState)
    {
    case MAINMENU:
        renderMainMenu();
	drawControls();
        break;
    case PAUSEMENU:
        renderPauseMenu();
	drawControls();
        break;
    case DEATH:
	renderDeath();
	break;
    case INGAME:{
        Rect r;
        //Clear the screen
        glClearColor(0.0, 0.5, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

	// inits 
	float ws = (float)1/img[2].columns;
        float hs = (float)1/img[2].rows;
	float w  = img[2].width/1.4;
	float h  = img[2].height/1.5;
	
	renderBackground(ws,hs,w,h);
	//Lives(g.xres, g.yres);	

        //draw explosions
        cleanExplosions();
        for(unsigned int i = 0; i < explosions.size(); i++){
            explosions[i].draw();
        }

        //draw powerUps
        cleanPowerUps();
        for(unsigned int i = 0; i < powerUps.size(); i++){
            powerUps[i].draw();
        }
        //DRAW BULLET 
	erikRender();
        Bullet *b; 
        for (int i = 0; i < nbullets; i++) {
            b = &g.ramboBullets[i];
            b->draw();
        }
        
        //draw Rambo
        rambo.draw();
        kuljitS_render();

        
	//rambo.drawOptimized();

        //Rambo hitbox center
        // glPointSize(10);
        // glBegin(GL_POINTS);
        // glColor3f(0, 0, 0);
        // glVertex3f(rambo.getCenterX(), rambo.getCenterY(), 0);
        // glEnd();
        
        //Lives(g.xres, g.yres);

        if (debug_mode) {
            unsigned int c = 0x00ffff44;
            r.bot = g.yres - 20;
            r.left = 10;
            r.center = 0;
            ggprint8b(&r, 16, c, "right arrow -> walk right");
            ggprint8b(&r, 16, c, "left arrow  <- walk left");
            ggprint8b(&r, 16, c, "f key to shoot");
            ggprint8b(&r, 16, c, "d key to jump");
            ggprint8b(&r, 16, c, "minus key to shrink Rambo");
            ggprint8b(&r, 16, c, "u key to expand Rambo");
            ggprint8b(&r, 16, c, "6 key to toggle hitboxes");
            ggprint8b(&r, 16, c, "h key to toggle debug mode");
            printKuljitS(g.xres - 100, g.yres - 20, 16, 0);
            printJoshuaC(g.xres/2, g.yres-100, 30, 255);

        }
        
        break;
    }
    default:
        printf("FATAL ERROR IN GAME STATE\n\n");
        exit(1);
    }
}
