//modified by:
//date:
//
//3480 Computer Graphics
//lab5.cpp
//Author: Gordon Griesel (do not change the author's name)
//Date:   2017
//
//This is a simple orthographic ray tracer.
//To render scenes properly, you must add code to the following functions...
//
//  void vecNormalize()
//  int rayPlaneIntersect()
//  int rayDiskIntersect()
//  void trace()
//  void render()
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
//constant definitions...
const int MAXOBJECTS =  64;
enum {
	TYPE_NONE=0,
	TYPE_DISK,
	TYPE_RING,
	TYPE_SPHERE,
	TYPE_TRIANGLE,
	ORTHO,
	PERSPECTIVE
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
	Flt radius, radius2;
	Vec color;
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
		XStoreName(dpy, win, "3480 ray caster.  Press R to render.");
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
void initsat()
{
    int iring=105;
	Object *o;
	g.nobjects=0;
	//
	o = &g.object[g.nobjects];
	o->type = TYPE_DISK;
	vecMake(0, 0, 0, o->center);
   vecMake(0.0, 0.0, -1.0, o->norm);
   vecMake(.5,.5,.5, o->color);
	 if(g.color==1){
        vecMake(.04,0,1, o->color);
    } 
	o->radius = 100.0;
	vecNormalize(o->norm);
	g.nobjects++;

//------------------------
for(int i=0; i<13;i++){
   o=&g.object[g.nobjects];
   o->type=TYPE_RING;
  
	vecMake(0, 0.0,0.0, o->center);
   vecMake(5.0, 10.0, 5.0, o->norm);
   vecMake(0,255,0, o->color);
   o->radius2=iring;
	o->radius = iring+5;
	vecNormalize(o->norm);
	g.nobjects++;
   iring+=10;
}
   
}

void init(void)
{

	//Setup some objects
	Object *o;
	g.nobjects=0;
	//
	o = &g.object[g.nobjects];
	o->type = TYPE_DISK;
	vecMake(-100.0, -120.0, -3000.0, o->center);
   vecMake(0.0, 0.0, -1.0, o->norm);
	if(g.tilt){
       vecMake(0, 9.0, -2.0, o->norm);
   }
   vecMake(.5,.5,.5, o->color);
	 if(g.color==1){
        vecMake(.04,0,1, o->color);
    } 
	o->radius = 100.0;
	vecNormalize(o->norm);
	g.nobjects++;
	//
	o = &g.object[g.nobjects];
	o->type = TYPE_DISK;
	vecMake(0.0, 0.0, -2000.0, o->center);
	vecMake(0.0, 0.0, -1.0, o->norm);
	if(g.tilt){
       vecMake(6.0, 16.0, 4.0, o->norm);
   }
	vecMake(.6,.6,.6, o->color);
	 if(g.color==1){
        vecMake(1,0,0, o->color);
    } 
	o->radius = 100.0;
	vecNormalize(o->norm);
	g.nobjects++;
	//
	o = &g.object[g.nobjects];
	o->type = TYPE_DISK;
	vecMake(100.0, 120.0, -1000.0, o->center);
	vecMake(0.0, 0.0, -1.0, o->norm);
	if(g.tilt){
       vecMake(-5.0, 0.0, -1.0, o->norm);
   }
	vecMake(.7,.7,.7, o->color);
	 if(g.color==1){
        vecMake(1,.9,0, o->color);
    } 
	o->radius = 100.0;
	vecNormalize(o->norm);
	g.nobjects++;

}

Flt vecDotProduct(Vec v0, Vec v1)
{
	return (v0[0]*v1[0] + v0[1]*v1[1] + v0[2]*v1[2]);
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
	//Normalize a vector. Can be optimized a little.
	//We will write this function in class together





}

void checkMouse(XEvent *e)
{
	if (e->type == ButtonPress) {
		//No mouse in this program.
	}
}

int checkKeys(XEvent *e)
{
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		if (key == XK_a) {
			takeScreenshot("", 0);
			return 0;
		}
		//------------------------------------------------------
		if (key == XK_1) {
			g.mode=1;
			g.color =
			g.tilt =
			g.checker =
			g.saturn =
			g.initials = 0;
			return 0;
		}
		//------------------------------------------------------
		if (key == XK_2) { g.mode=2; g.color = 1; init();   return 0; }
		if (key == XK_3) { g.mode=3; g.tilt = 1;  init();   return 0; }
		if (key == XK_4) { g.mode=4; g.checker = 1;init();  return 0; }
		if (key == XK_5) { g.mode=5; g.saturn = 1;  initsat(); return 0; }
		if (key == XK_6) { g.mode=6; g.initials = 1; return 0; }
		//------------------------------------------------------
		if (key == XK_r) {
			render(ORTHO);
			return 0;
		}
		if (key == XK_p) {
	
			render(PERSPECTIVE);
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

int rayPlaneIntersect(Vec center, Vec normal, Ray *ray, Hit *hit)
{
//-----------------------------------------------------------------
//Comment this code when you are ready to write the lab assignment.
/*if(!(g.mode=3)){  
 hit->p[0] = ray->o[0];
hit->p[1] = ray->o[1];
hit->p[2] = center[2];
hit->t = center[2] - ray->o[2];
return 1;
}*/
//-----------------------------------------------------------------

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
	//        remember the unary minus sign
   //

      Vec v; 
      v[0] = ray->o[0] - center[0];
      v[1] = ray->o[1] -center[1];
      v[2] = ray->o[2] - center[2];
      Flt dot1 = vecDotProduct(v,normal);

      Flt dot2 = vecDotProduct(ray->d,normal);
      Flt t;
      t = -(dot1/dot2);
      hit->p[0]= ray->o[0] + ray->d[0] *t;
      hit->p[1]= ray->o[1] + ray->d[1] *t;
      hit->p[2]= ray->o[2] + ray->d[2] *t;
      hit->t=t;


return 1;

}

int rayRingIntersect(Object *o, Ray *ray, Hit *hit)
{
	//Does the ray intersect the disk's plane?
	if (rayPlaneIntersect(o->center, o->norm, ray, hit)) {
		//Yes
		//Check that the hit point is within the disk radius.
		//--------------------------------------------------------------
		//This is temporary code so you will see disks on the screen.
		//This must be changed for the assignment.
		//Use the point hit instead of the ray origin.

		Flt d0, d1,d2, dist;
		d0 = o->center[0] - hit->p[0];
		d1 = o->center[1] - hit->p[1];
      d2=  o->center[2] - hit->p[2];
		dist = d0*d0+d1*d1+d2*d2;

		if (dist <= o->radius * o->radius && dist >= (o->radius2 * o->radius2)) {
			//Hit is within radius
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

		//Check that the hit point is within the disk radius.
		//--------------------------------------------------------------
		//This is temporary code so you will see disks on the screen.
		//This must be changed for the assignment.
		//Use the point hit instead of the ray origin.

		Flt d0, d1,d2, dist;
		d0 = o->center[0] - hit->p[0];
		d1 = o->center[1] - hit->p[1];
      d2=  o->center[2] - hit->p[2];
		dist = d0*d0+d1*d1+d2*d2;

		if (dist <= o->radius * o->radius) {
			//Hit is within radius
			return 1;
		}
	}
	return 0;
}

void trace(Ray *ray, Vec rgb)
{
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
						h=i;
					}
				}

				break;
			case TYPE_RING:
            if(rayRingIntersect(o,ray,&hit)){
                if (hit.t < closehit.t) {
						closehit.t = hit.t;
						vecCopy(hit.p, closehit.p);
						vecCopy(o->color, closehit.color);
						h=i;
					}
            }

				break;
			case TYPE_SPHERE:
				break;
			case TYPE_TRIANGLE:
				break;
		}
	}
	if (h < 0 && g.checker==1) {
		//ray did not hit an object.
      int px,py;
      px= ray->o[0]/sqrt(g.xres);
      py= ray->o[1]/sqrt(g.yres);

      px=(int)px%2;
      py=(int)py%2;
      if(px==0 && py==0 )
      {
          rgb[0]=200;
          rgb[1]=232;
          rgb[2]=229;
      }else if(px==1 && py==1)
      {
          rgb[0]=200;
          rgb[1]=232;
          rgb[2]=229;
      }
      else if(px==2 && py==2)
      {
          rgb[0]=200;
          rgb[1]=232;
          rgb[2]=229;}
      else 
          rgb[0]=0;
          rgb[1]=32;
          rgb[2]=29;


		return;
	}
	//The ray hit an object.
	//Set the color of the pixel to the color of the object.
	rgb[0] = closehit.color[0];
	rgb[1] = closehit.color[1];
	rgb[2] = closehit.color[2];
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
	//Start at bottom of screen and go up.
	for (int i=g.yres-1; i>=0; i--) {
		pixel[0] = xStart;
		for (int j=0; j<g.xres; j++) {
			vecCopy(pixel, eye);
			if (projection == PERSPECTIVE) {
				//Place code here for perspective projection
             eye[0] =0;
             eye[1] =0;
             eye[2] =0; 
			}
			//Move the eye back from the screen.
			eye[2] = 1000.0;
			//Cast the ray through the pixel.
			setupRay(eye, pixel, &ray);
			vecZero(rgb);
			trace(&ray, rgb);
			x11.drawPixel(j, i, rgb);
			pixel[0] += xStep;
		}
		pixel[1] += yStep;
	}
}


