#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "Timers.h"

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt     Matrix[4][4];
extern  int MAX_JUGGERNAUTS;

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)     ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]



Timers::Timers() {
    physicsRate = 1.0 / 30.0;
    oobillion = 1.0 / 1e9;
    recordTime(&timeStart);    
    recordTime(&timeEnd);    
    recordTime(&timeCurrent);    
    recordTime(&walkTime);    
    recordTime(&ramboWeaponOutTime);    
    recordTime(&menuSelectionTime);    
    recordTime(&pirateSpawnTime);    
    recordTime(&batSpawnTime);    
    recordTime(&ramboCollisionTime);
    recordTime(&juggernautSpawnTime);
    juggernautChargeTime = new timespec[MAX_JUGGERNAUTS];
    for (int i=0; i< MAX_JUGGERNAUTS; i++) {
        recordTime(&juggernautChargeTime[i]);
    }
}

double Timers::timeDiff(struct timespec *start, struct timespec *end) {
    return (double)(end->tv_sec - start->tv_sec ) +
        (double)(end->tv_nsec - start->tv_nsec) * oobillion;
}

void Timers::timeCopy(struct timespec *dest, struct timespec *source) {
    memcpy(dest, source, sizeof(struct timespec));
}

void Timers::recordTime(struct timespec *t) {
    clock_gettime(CLOCK_REALTIME, t);
}
