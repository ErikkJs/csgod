// 3350 Project Rambo - kuljitS.cpp
// author: Kuljit Singh
// last modified: Feb 28, 2018

#include<X11/keysym.h>
#include "Global.h"
#include "Timers.h"
#include "SpriteSheet.h"
#include "Character.h"
#include "Enemy1.h"
#include "Bat.h"
#include "Hulk.h"
#include "fonts.h"
#include <sstream>
#include "joshuaC.h"
#include "fernandoM.h"
#include "erikS.h"
#include <cmath>

extern int flipped;
extern float cx;
extern Timers timers;
extern Global g;
extern int keys[];
extern Character rambo;
extern float gravity;
extern int gameState;
extern int selectedOption;
extern SpriteSheet img[];
extern int MAX_PIRATES;
extern int nPirates;
extern Enemy1* pirates;
extern int MAX_BATS;
extern int nBats;
extern Bat* bats;
extern int MAX_JUGGERNAUTS;
extern int nJuggernauts;
extern Hulk* juggernauts;
extern int nbullets;
extern void deleteBullet(int);
extern int done;
extern int cursorPosition[];
extern int level;
extern void sound(int select);

double juggernautSpawnDelay = 1.0;
double menuSelectionDelay = 0.15;
double pirateSpawnDelay = 1.0;
double batSpawnDelay = 2.0;
double ramboCollisionDelay = 1.0;
bool activateRamboFlicker = false;

void kuljitS_physics() 
{   
    if (level <=100) {
        level = g.score/500 + 1;
    }

    //check for bullet collision with enemy
    for (int i=0; i<nbullets; i++) {
        for (int j=0; j<nPirates; j++) {
            if (g.ramboBullets[i].pos[0] > pirates[j].hitBox->getLeft() &&
                    g.ramboBullets[i].pos[0] < pirates[j].hitBox->getRight() &&
                    g.ramboBullets[i].pos[1] > pirates[j].hitBox->getBottom() &&
                    g.ramboBullets[i].pos[1] < pirates[j].hitBox->getTop()) {
                if (rambo.headshot) {
                    pirates[j].health = 0;
                }
                else {
                    pirates[j].health--;
                    pirates[j].health--;
                }

                deleteBullet(i);
                g.score+=25;
                if (pirates[j].health <= 0) {
                    createExplosion(pirates[j].centerX, pirates[j].centerY);
                    pirates[j].centerY = pirates[nPirates-1].centerY;
                    pirates[j].centerX = pirates[nPirates-1].centerX;
                    pirates[j].velocityY = pirates[nPirates-1].velocityY;
                    pirates[j].velocityX = pirates[nPirates-1].velocityX;
                    pirates[j].flipped = pirates[nPirates-1].flipped;
                    pirates[j].health = pirates[nPirates-1].health;
                    pirates[j].hitBox->updateHitBox(
                            pirates[nPirates-1].hitBox->getTop(),
                            pirates[nPirates-1].hitBox->getBottom(),
                            pirates[nPirates-1].hitBox->getLeft(),
                            pirates[nPirates-1].hitBox->getRight());
                    nPirates--;
                }
            }
        }

        for (int j=0; j<nBats; j++) {
            if (g.ramboBullets[i].pos[0] > bats[j].hitBox->getLeft() &&
                    g.ramboBullets[i].pos[0] < bats[j].hitBox->getRight() &&
                    g.ramboBullets[i].pos[1] > bats[j].hitBox->getBottom() &&
                    g.ramboBullets[i].pos[1] < bats[j].hitBox->getTop()) {
                deleteBullet(i);
                if(bats[j].shiny) {
                    createPowerUp(bats[j].centerX, bats[j].centerY, 0.1, 0.1, 0);
                }
                bats[j].centerY = bats[nBats-1].centerY;
                bats[j].centerX = bats[nBats-1].centerX;
                bats[j].velocityY = bats[nBats-1].velocityY;
                bats[j].velocityX = bats[nBats-1].velocityX;
                bats[j].flipped = bats[nBats-1].flipped;
                bats[j].health = bats[nBats-1].health;
                bats[j].shiny = bats[nBats-1].shiny;
                g.score+=50;
                nBats--;
            }
        }

        for (int j=0; j<nJuggernauts; j++) {
            if (g.ramboBullets[i].pos[0] > juggernauts[j].hitBox->getLeft() &&
                    g.ramboBullets[i].pos[0]<juggernauts[j].hitBox->getRight() &&
                    g.ramboBullets[i].pos[1]>juggernauts[j].hitBox->getBottom() &&
                    g.ramboBullets[i].pos[1] < juggernauts[j].hitBox->getTop()) {

                if(rambo.headshot) {
                    juggernauts[j].health = 0;
                }
                else {
                    juggernauts[j].health--;
                }
                deleteBullet(i);
                if (juggernauts[j].health <= 0) {
                    createExplosion(juggernauts[j].centerX, 
                        juggernauts[j].centerY);
                    juggernauts[j].centerY = juggernauts[nJuggernauts-1].centerY;
                    juggernauts[j].centerX = juggernauts[nJuggernauts-1].centerX;
                    juggernauts[j].velocityY=juggernauts[nJuggernauts-1].velocityY;
                    juggernauts[j].velocityX=juggernauts[nJuggernauts-1].velocityX;
                    juggernauts[j].flipped = juggernauts[nJuggernauts-1].flipped;
                    juggernauts[j].health = juggernauts[nJuggernauts-1].health;
                    juggernauts[j].hitBox->updateHitBox(
                            juggernauts[nJuggernauts-1].hitBox->getTop(),
                            juggernauts[nJuggernauts-1].hitBox->getBottom(),
                            juggernauts[nJuggernauts-1].hitBox->getLeft(),
                            juggernauts[nJuggernauts-1].hitBox->getRight());
                    g.score+=50;
                    nJuggernauts--;
                }
            }
        }
    }

    //create new piratesif not at maxpirates 
    if (nPirates< level*1 && nPirates < MAX_PIRATES) {
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.pirateSpawnTime,
                &timers.timeCurrent);
        if (timeSpan > pirateSpawnDelay) {
            pirates[nPirates].centerY = 100;  
            if (rnd() < .5) {
                pirates[nPirates].centerX = 0 - rnd()*50; 
                pirates[nPirates].velocityX = rnd()*2 + 2;  
                pirates[nPirates].flipped=true;
            } else {
                pirates[nPirates].centerX = g.xres + rnd()*50;  
                pirates[nPirates].velocityX = -1*(rnd()*2 + 2);
                pirates[nPirates].flipped=false;  
            }
            pirates[nPirates].hitBox->updateHitBox(0,0,0,0);
            pirates[nPirates].health = 4;
            timers.recordTime(&timers.pirateSpawnTime); 
            nPirates++; 
        }  
    }    

    if (nBats< level*2 && nBats < MAX_BATS) {
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.batSpawnTime,
                &timers.timeCurrent);
        if (timeSpan > batSpawnDelay) {
            bats[nBats].centerY = 500; 
            if (rnd() < .5) {
                bats[nBats].shiny = true;
            } else {
                bats[nBats].shiny = false;
            } 
            if (rnd() < .5) {
                bats[nBats].centerX = 0 - rnd()*50; 
                bats[nBats].velocityX = rnd()*2 + 2;  
                bats[nBats].flipped=true;
            } else {
                bats[nBats].centerX = g.xres + rnd()*50;  
                bats[nBats].velocityX = -1*(rnd()*2 + 2);
                bats[nBats].flipped=false;  
            }
            bats[nBats].hitBox->updateHitBox(0,0,0,0);
            timers.recordTime(&timers.batSpawnTime); 
            nBats++; 
        }  
    }    

    if (nJuggernauts< (level>>2) && nJuggernauts < MAX_JUGGERNAUTS) {
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.juggernautSpawnTime,
                &timers.timeCurrent);
        if (timeSpan > juggernautSpawnDelay) {
            juggernauts[nJuggernauts].centerY = 100;  
            if (rnd() < .5) {
                juggernauts[nJuggernauts].centerX = 0 - rnd()*50; 
                juggernauts[nJuggernauts].velocityX = rnd()*2 + 0.5;  
                juggernauts[nJuggernauts].flipped=false;
            } else {
                juggernauts[nJuggernauts].centerX = g.xres + rnd()*50;  
                juggernauts[nJuggernauts].velocityX = -1*(rnd()*2 + 0.5);
                juggernauts[nJuggernauts].flipped=true;  
            }
            juggernauts[nJuggernauts].hitBox->updateHitBox(0,0,0,0);
            juggernauts[nJuggernauts].health = 4;
            timers.recordTime(&timers.juggernautSpawnTime); 
            nJuggernauts++; 
        }  
    }

    //check for enemy collisions
    for (int i=0; i<nPirates; i++) {
        if (pirates[i].hitBox->getLeft() <= rambo.hitBox->getRight() &&
                pirates[i].hitBox->getRight() >= rambo.hitBox->getLeft() &&
                pirates[i].hitBox->getTop() >= rambo.hitBox->getBottom() &&
                pirates[i].hitBox->getBottom() <= rambo.hitBox->getTop()) {
            timers.recordTime(&timers.timeCurrent);
            double timeSpan = timers.timeDiff(&timers.ramboCollisionTime,
                    &timers.timeCurrent);
            if (timeSpan > ramboCollisionDelay) {
                activateRamboFlicker = true;
                printf("Enemy Collision\n");
                rambo.health--;
                sound(3);
                if (rambo.health<=0) {
                    printf("RAMBO DEAD\n");
                    //temporary until death menu exists
                    gameState=DEATH; 
                }
                timers.recordTime(&timers.ramboCollisionTime);
            }

        }
    } 

    for (int i=0; i<nBats; i++) {
        if (bats[i].hitBox->getLeft() <= rambo.hitBox->getRight() &&
                bats[i].hitBox->getRight() >= rambo.hitBox->getLeft() &&
                bats[i].hitBox->getTop() >= rambo.hitBox->getBottom() &&
                bats[i].hitBox->getBottom() <= rambo.hitBox->getTop()) {
            timers.recordTime(&timers.timeCurrent);
            double timeSpan = timers.timeDiff(&timers.ramboCollisionTime,
                    &timers.timeCurrent);
            if (timeSpan > ramboCollisionDelay) {
                activateRamboFlicker = true;
                printf("Enemy Collision\n");
                rambo.health--;
                sound(3);
                if (rambo.health<=0) {
                    printf("RAMBO DEAD\n");
                    //temporary until death menu exists
                    gameState=DEATH; 
                }
                timers.recordTime(&timers.ramboCollisionTime);
            }

        }
    } 

    for (int i=0; i<nJuggernauts; i++) {
        if (juggernauts[i].hitBox->getLeft() <= rambo.hitBox->getRight() &&
                juggernauts[i].hitBox->getRight() >= rambo.hitBox->getLeft() &&
                juggernauts[i].hitBox->getTop() >= rambo.hitBox->getBottom() &&
                juggernauts[i].hitBox->getBottom() <= rambo.hitBox->getTop()) {
            timers.recordTime(&timers.timeCurrent);
            double timeSpan = timers.timeDiff(&timers.ramboCollisionTime,
                    &timers.timeCurrent);
            if (timeSpan > ramboCollisionDelay) {
                activateRamboFlicker = true;
                printf("Enemy Collision\n");
                rambo.health--;
                sound(3);
                if (rambo.health<=0) {
                    printf("RAMBO DEAD\n");
                    //temporary until death menu exists
                    gameState=DEATH; 
                }
                timers.recordTime(&timers.ramboCollisionTime);
            }

        }
    } 

    //check for a jump
    if (keys[XK_d] && !rambo.jumping) {
        rambo.jumping = true;
        rambo.velocityY=JUMP_STRENGTH;
    }

    //apply gravity
    if (rambo.jumping) {
        rambo.velocityY += gravity;
    }

    //update rambo position
    rambo.centerY +=rambo.velocityY;
    rambo.centerX +=rambo.velocityX;

    //update enemy positions
    for (int i=0; i<nPirates; i++) {
        if (pirates[i].centerX < 0 && pirates[i].velocityX<0) {
            pirates[i].velocityX *= -1;
            pirates[i].flipped=true;
        }

        if (pirates[i].centerX > g.xres && pirates[i].velocityX>0) {
            pirates[i].velocityX *= -1;
            pirates[i].flipped=false;
        }
        pirates[i].centerX += pirates[i].velocityX;
        pirates[i].centerY += pirates[i].velocityY;
    }

    for (int i=0; i<nBats; i++) {
        if (bats[i].centerX < 0 && bats[i].velocityX<0) {
            bats[i].velocityX *= -1;
            bats[i].flipped=true;
        }

        if (bats[i].centerX > g.xres && bats[i].velocityX>0) {
            bats[i].velocityX *= -1;
            bats[i].flipped=false;
        }

        if (bats[i].centerY >= 500) {
            bats[i].centerY = 500;
            bats[i].velocityY = 0;
            float angle = 0;
            if (abs(bats[i].centerX - rambo.centerX) != 0) {
                angle = atan(400/abs(bats[i].centerX - rambo.centerX));
                angle = angle*180/PI;
            }
            if (angle > 42 && angle < 48) {
                if ((bats[i].velocityX>0 && rambo.centerX>bats[i].centerX) || 
                        (bats[i].velocityX<0 && rambo.centerX<bats[i].centerX)) {
                    bats[i].velocityY = -5;    
                }
            }
        } else if (bats[i].centerY <= 100) {
            bats[i].velocityY = 3;
        }

        bats[i].centerX += bats[i].velocityX;
        bats[i].centerY += bats[i].velocityY;
    }

    for (int i=0; i<nJuggernauts; i++) {
        if (juggernauts[i].centerX < 0 && juggernauts[i].velocityX<0) {
            juggernauts[i].velocityX *= -1;
            juggernauts[i].flipped=false;
        }

        if (juggernauts[i].centerX > g.xres && juggernauts[i].velocityX>0) {
            juggernauts[i].velocityX *= -1;
            juggernauts[i].flipped=true;
        }

        if (juggernauts[i].centerY <= 100) {
            juggernauts[i].centerY = 100;
            juggernauts[i].velocityY = 0;
            if (juggernauts[i].velocityX > 2.5) {
                juggernauts[i].velocityX = (rnd()*2 + 0.5);
            }
            if (juggernauts[i].velocityX < -2.5) {
                juggernauts[i].velocityX = -1*(rnd()*2 + 0.5);
            }
            if ((juggernauts[i].centerX - rambo.centerX < -400 &&
                        juggernauts[i].velocityX > 0) ||
                    (juggernauts[i].centerX - rambo.centerX > 400 && 
                     juggernauts[i].velocityX < 0)) {
                juggernauts[i].velocityY = (JUMP_STRENGTH-2)*2;
                float timetofall = abs(2*(JUMP_STRENGTH-2)*2/gravity);
                juggernauts[i].velocityX = -juggernauts[i].centerX;
                juggernauts[i].velocityX+= rambo.centerX;
                juggernauts[i].velocityX/=timetofall;
            }

        } else {
            juggernauts[i].velocityY += gravity;
        }

        juggernauts[i].centerX += juggernauts[i].velocityX;
        juggernauts[i].centerY += juggernauts[i].velocityY;
    }

    //velocity == 0 if standing on a platform
    if (rambo.centerY <= 100) {
        rambo.centerY = 100;
        rambo.velocityY = 0;
        rambo.jumping = false;
    }

}

void kuljitS_render()
{
    Rect r;

    //draw pirates+ enemy ID's
    for (int i=0; i<nPirates; i++) {
        pirates[i].draw();
        r.left = pirates[i].centerX;
        r.bot = pirates[i].centerY + 100;
        //ggprint8b(&r, 16, 0xffffff, "%i", i);

        //draw enemy health bar //////////////////////////////////////
        if (pirates[i].health < 4) {
            glPushMatrix();
            glColor3f(1.0, 1.0, 1.0);
            glBindTexture(GL_TEXTURE_2D, g.enemyHealthBarTexture);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glColor4ub(255,255,255,255);

            float ssWidth = (float)1.0/img[10].columns;
            float ssHeight = (float)1.0/img[10].rows;

            int ix = 0;
            int iy = 0;

            switch(pirates[i].health) {
                case 4:
                    ix = 0;
                    iy = 0;
                    break;
                case 3:
                    ix = 0;
                    iy = 1;
                    break;
                case 2:
                    ix = 3;
                    iy = 1;
                    break;
                case 1:
                    ix = 2;
                    iy = 2;
                    break;
                case 0:
                    ix = 3;
                    iy = 3;
                    break;
                default:
                    ix = 2;
                    iy = 3;
                    break;
            };

            float textureX = (float)ix / img[10].columns;
            float textureY = (float)iy / img[10].rows;

            float centerX = pirates[i].centerX;
            float centerY = pirates[i].centerY + 150;

            float width = img[10].width*.1;
            float height = img[10].height*.1;

            glBegin(GL_QUADS);
            glTexCoord2f(textureX, textureY+ssHeight);
            glVertex2i(centerX-width, centerY-height);

            glTexCoord2f(textureX, textureY);
            glVertex2i(centerX-width, centerY+height);

            glTexCoord2f(textureX+ssWidth, textureY);
            glVertex2i(centerX+width, centerY+height);

            glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
            glVertex2i(centerX+width, centerY-height);
            glEnd();

            glPopMatrix();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_ALPHA_TEST);
        }

        ////////////////////////////////////////////
    }

    for (int i=0; i<nBats; i++) {
        bats[i].draw();
        r.left = bats[i].centerX;
        r.bot = bats[i].centerY + 100;
        //ggprint8b(&r, 16, 0xffffff, "%i", i);

        //draw enemy health bar //////////////////////////////////////
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, g.enemyHealthBarTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);

        float ssWidth = (float)1.0/img[10].columns;
        float ssHeight = (float)1.0/img[10].rows;

        int ix = 0;
        int iy = 0;

        if (bats[i].health < 4) { 
            switch(bats[i].health) {
                case 4:
                    ix = 0;
                    iy = 0;
                    break;
                case 3:
                    ix = 0;
                    iy = 1;
                    break;
                case 2:
                    ix = 3;
                    iy = 1;
                    break;
                case 1:
                    ix = 2;
                    iy = 2;
                    break;
                case 0:
                    ix = 3;
                    iy = 3;
                    break;
                default:
                    ix = 2;
                    iy = 3;
                    break;
            };

            float textureX = (float)ix / img[10].columns;
            float textureY = (float)iy / img[10].rows;

            float centerX = bats[i].centerX;
            float centerY = bats[i].centerY + 150;

            float width = img[10].width*.1;
            float height = img[10].height*.1;

            glBegin(GL_QUADS);
            glTexCoord2f(textureX, textureY+ssHeight);
            glVertex2i(centerX-width, centerY-height);

            glTexCoord2f(textureX, textureY);
            glVertex2i(centerX-width, centerY+height);

            glTexCoord2f(textureX+ssWidth, textureY);
            glVertex2i(centerX+width, centerY+height);

            glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
            glVertex2i(centerX+width, centerY-height);
            glEnd();

            glPopMatrix();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_ALPHA_TEST);
        }
        ////////////////////////////////////////////
    }

    for (int i=0; i<nJuggernauts; i++) {
        juggernauts[i].draw();
        r.left = juggernauts[i].centerX;
        r.bot = juggernauts[i].centerY + 100;
        //ggprint8b(&r, 16, 0xffffff, "%i", i);

        //draw enemy health bar //////////////////////////////////////
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, g.enemyHealthBarTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);

        float ssWidth = (float)1.0/img[10].columns;
        float ssHeight = (float)1.0/img[10].rows;

        int ix = 0;
        int iy = 0;

        if (juggernauts[i].health < 4) {
            switch(juggernauts[i].health) {
                case 4:
                    ix = 0;
                    iy = 0;
                    break;
                case 3:
                    ix = 0;
                    iy = 1;
                    break;
                case 2:
                    ix = 3;
                    iy = 1;
                    break;
                case 1:
                    ix = 2;
                    iy = 2;
                    break;
                case 0:
                    ix = 3;
                    iy = 3;
                    break;
                default:
                    ix = 2;
                    iy = 3;
                    break;
            };

            float textureX = (float)ix / img[10].columns;
            float textureY = (float)iy / img[10].rows;

            float centerX = juggernauts[i].centerX;
            float centerY = juggernauts[i].centerY + 150;

            float width = img[10].width*.1;
            float height = img[10].height*.1;

            glBegin(GL_QUADS);
            glTexCoord2f(textureX, textureY+ssHeight);
            glVertex2i(centerX-width, centerY-height);

            glTexCoord2f(textureX, textureY);
            glVertex2i(centerX-width, centerY+height);

            glTexCoord2f(textureX+ssWidth, textureY);
            glVertex2i(centerX+width, centerY+height);

            glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
            glVertex2i(centerX+width, centerY-height);
            glEnd();

            glPopMatrix();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_ALPHA_TEST);
        }
        ////////////////////////////////////////////
    }

    //print score
    r.bot = g.yres - 20;
    r.left = g.xres/2;
    r.center = 1;
    ggprint13(&r, 16, 0xffffff, "Score: %i", g.score);
    ggprint13(&r, 16, 0xffffff, "Level: %i", level);


    //draw rambo logo //////////////////////////////////////
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.healthBarTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    float ssWidth = (float)1.0/img[9].columns;
    float ssHeight = (float)1.0/img[9].rows;

    int ix = 0;

    switch(rambo.health) {
        case 4:
            ix = 0;
            break;
        case 3:
            ix = 1;
            break;
        case 2:
            ix = 2;
            break;
        case 1:
            ix = 3;
            break;
        default:
            ix = 4;
            break;
    };

    float textureX = (float)ix / img[9].columns;
    float textureY = 0;

    float centerX = g.xres/2;
    float centerY = g.yres*2/3; 

    float width = img[9].width*0.25;
    float height = img[9].height*0.25;

    glBegin(GL_QUADS);
    glTexCoord2f(textureX, textureY+ssHeight);
    glVertex2i(centerX-width, centerY-height);

    glTexCoord2f(textureX, textureY);
    glVertex2i(centerX-width, centerY+height);

    glTexCoord2f(textureX+ssWidth, textureY);
    glVertex2i(centerX+width, centerY+height);

    glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
    glVertex2i(centerX+width, centerY-height);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);

    ////////////////////////////////////////////
}

double printRamboCenter()
{

    static double td = 0.0;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    Rect r;
    r.bot = g.yres*2/4;
    r.left = g.xres/2;
    r.center = 1;
    ggprint8b(&r, 16, 0xffffff, "RAMBO");

    clock_gettime(CLOCK_REALTIME, &end);
    td += timers.timeDiff(&start, &end);

    return td;
}

double printRamboCenterOpt()
{

    static double td = 0.0;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    Rect r;
    r.bot = g.yres << 1;
    r.bot = r.bot >> 2;
    r.left = g.xres >> 1;
    r.center = 1;
    ggprint8b(&r, 16, 0xffffff, "RAMBO");

    clock_gettime(CLOCK_REALTIME, &end);
    td += timers.timeDiff(&start, &end);

    return td;
}

void printKuljitS(int x, int y, int size, int color)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, size, color, "Kuljit Singh");
    ggprint8b(&r, size, color, "RAMBO: %lf",printRamboCenter());
    ggprint8b(&r, size, color, "R_OPT: %lf",printRamboCenterOpt());
}

int acceptGameState(int selectedOption)
{
    //update game state to selected option in main menu
    switch (selectedOption) {
        case 0:
            newGame();
            gameState = INGAME;
            break;
        case 1:
            //open HTML LEADERBOARD
            system("firefox cs.csubak.edu/~rnoriega/3350/leaderboard");
            break;
        case 2:
            done=1;
            return 1;
        case 3:
            gameState = INGAME;
            break;
        default:
            printf("FATAL ERROR IN GAME STATE\n\n");
            exit(1);
    }
    return 0;
}

void checkMouseMainMenu(XEvent *e)
{    
    //Did the mouse move?
    //Was a mouse button clicked?
    static int savex = -100;
    static int savey = -100;
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

}

void checkKeysMainMenu()
{
    if (keys[XK_Up]) {
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
                &timers.timeCurrent);
        if (timeSpan > menuSelectionDelay) {
            selectedOption = ((selectedOption-1)+3)%3;
            timers.recordTime(&timers.menuSelectionTime);
        }
    }

    if (keys[XK_Down]) {
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
                &timers.timeCurrent);
        if (timeSpan > menuSelectionDelay) {
            selectedOption = ((selectedOption+1)+3)%3;
            timers.recordTime(&timers.menuSelectionTime);
        }
    }

    if (keys[XK_Return]) {
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
                &timers.timeCurrent);
        if (timeSpan > menuSelectionDelay) {
            acceptGameState(selectedOption);
            timers.recordTime(&timers.menuSelectionTime);
        }
    }

}

void checkKeysPauseMenu()
{
    if (keys[XK_Up]) {
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
                &timers.timeCurrent);
        if (timeSpan > menuSelectionDelay) {
            selectedOption = ((selectedOption-1)+4)%4;
            timers.recordTime(&timers.menuSelectionTime);
        }
    }

    if (keys[XK_Down]) {
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
                &timers.timeCurrent);
        if (timeSpan > menuSelectionDelay) {
            selectedOption = ((selectedOption+1)+4)%4;
            timers.recordTime(&timers.menuSelectionTime);
        }
    }

    if (keys[XK_Return]) {
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.menuSelectionTime,
                &timers.timeCurrent);
        if (timeSpan > menuSelectionDelay) {
            acceptGameState(selectedOption);
            timers.recordTime(&timers.menuSelectionTime);
        }
    }

}
void renderMainMenu()
{
    sound(1);
    //set background to game background
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.RamboTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255, 255, 255, 255);

    float ssWidth = (float)1.0 / img[2].columns;
    float ssHeight = (float)1.0 / img[2].rows;

    float textureX = 0;
    float textureY = 0;

    float centerX = g.xres / 2;
    float centerY = (g.yres / 2) + g.yres*0.20833;

    float width = img[2].width/1.4;
    float height = img[2].height/1.5;

    glBegin(GL_QUADS);
    glTexCoord2f(textureX, textureY + ssHeight);
    glVertex2i(centerX - width, centerY - height);

    glTexCoord2f(textureX, textureY);
    glVertex2i(centerX - width, centerY + height);

    glTexCoord2f(textureX + ssWidth, textureY);
    glVertex2i(centerX + width, centerY + height);

    glTexCoord2f(textureX + ssWidth, textureY + ssHeight);
    glVertex2i(centerX + width, centerY - height);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);	

    //draw rambo logo //////////////////////////////////////
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.ramboLogoTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    ssWidth = (float)1.0/img[1].columns;
    ssHeight = (float)1.0/img[1].rows;

    textureX = 0;
    textureY = 0;

    centerX = g.xres/2;
    centerY = g.yres*2/3; 

    width = floor(((float)g.xres/1280)*img[1].width);
    height = floor(((float)g.yres/720)*img[1].height);

    glBegin(GL_QUADS);
    glTexCoord2f(textureX, textureY+ssHeight);
    glVertex2i(centerX-width, centerY-height);

    glTexCoord2f(textureX, textureY);
    glVertex2i(centerX-width, centerY+height);

    glTexCoord2f(textureX+ssWidth, textureY);
    glVertex2i(centerX+width, centerY+height);

    glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
    glVertex2i(centerX+width, centerY-height);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);

    ////////////////////////////////////////////

    //display meny options
    Rect r;
    r.bot = g.yres/3;
    r.left = g.xres/2;
    r.center = 1;

    switch (selectedOption) {
        case 0:
            ggprint8b(&r, 16, 0x123fff, "NEW GAME");
            ggprint8b(&r, 16, 0xffffff, "LEADERBOARD");
            ggprint8b(&r, 16, 0xffffff, "EXIT");
            break;
        case 1:
            ggprint8b(&r, 16, 0xffffff, "NEW GAME");
            ggprint8b(&r, 16, 0x123fff, "LEADERBOARD");
            ggprint8b(&r, 16, 0xffffff, "EXIT");
            break;
        case 2:
            ggprint8b(&r, 16, 0xffffff, "NEW GAME");
            ggprint8b(&r, 16, 0xffffff, "LEADERBOARD");
            ggprint8b(&r, 16, 0x123fff, "EXIT");
            break;
        default:
            //            printf("FATAL GAME ERROR\n\n");
            break;
    }

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.cursorTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    ssWidth = (float)1.0/img[7].columns;
    ssHeight = (float)1.0/img[7].rows;

    textureX = 0;
    textureY = 0;

    centerX = cursorPosition[0];
    centerY = g.yres - cursorPosition[1];

    width = (img[7].width/img[7].columns)*0.5;
    height = (img[7].height/img[7].rows)*0.5;

    glBegin(GL_QUADS);
    glTexCoord2f(textureX, textureY+ssHeight);
    glVertex2i(centerX-width, centerY-height);

    glTexCoord2f(textureX, textureY);
    glVertex2i(centerX-width, centerY+height);

    glTexCoord2f(textureX+ssWidth, textureY);
    glVertex2i(centerX+width, centerY+height);

    glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
    glVertex2i(centerX+width, centerY-height);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);

    ////////////////////////////////////////////
}

void renderPauseMenu()
{
    //draw rambo logo //////////////////////////////////////
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, g.ramboLogoTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    float ssWidth = (float)1.0/img[1].columns;
    float ssHeight = (float)1.0/img[1].rows;

    float textureX = 0;
    float textureY = 0;

    float centerX = g.xres/2;
    float centerY = g.yres*2/3; 

    float width = floor(((float)g.xres/1280)*img[1].width);
    float height = floor(((float)g.yres/720)*img[1].height);

    glBegin(GL_QUADS);
    glTexCoord2f(textureX, textureY+ssHeight);
    glVertex2i(centerX-width, centerY-height);

    glTexCoord2f(textureX, textureY);
    glVertex2i(centerX-width, centerY+height);

    glTexCoord2f(textureX+ssWidth, textureY);
    glVertex2i(centerX+width, centerY+height);

    glTexCoord2f(textureX+ssWidth, textureY+ssHeight);
    glVertex2i(centerX+width, centerY-height);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);

    ////////////////////////////////////////////

    //display meny options
    Rect r;
    r.bot = g.yres/3;
    r.left = g.xres/2;
    r.center = 1;

    switch (selectedOption) {
        case 0:
            if (rambo.centerY!=800) {
                ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
            }
            ggprint8b(&r, 16, 0x123fff, "NEW GAME");
            ggprint8b(&r, 16, 0xffffff, "LEADERBOARD");
            ggprint8b(&r, 16, 0xffffff, "EXIT");
            break;
        case 1:
            if (rambo.centerY!=800) {
                ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
            }
            ggprint8b(&r, 16, 0xffffff, "NEW GAME");
            ggprint8b(&r, 16, 0x123fff, "LEADERBOARD");
            ggprint8b(&r, 16, 0xffffff, "EXIT");
            break;
        case 2:
            if (rambo.centerY!=800) {
                ggprint8b(&r, 16, 0xffffff, "RESUME GAME");
            }
            ggprint8b(&r, 16, 0xffffff, "NEW GAME");
            ggprint8b(&r, 16, 0xffffff, "LEADERBOARD");
            ggprint8b(&r, 16, 0x123fff, "EXIT");
            break;
        case 3:
            if (rambo.centerY!=800) {
                ggprint8b(&r, 16, 0x123fff, "RESUME GAME");
            }
            ggprint8b(&r, 16, 0xffffff, "NEW GAME");
            ggprint8b(&r, 16, 0xffffff, "LEADERBOARD");
            ggprint8b(&r, 16, 0xffffff, "EXIT");
            break;
        default:
            //            printf("FATAL GAME ERROR\n\n");
            break;
    }
}
