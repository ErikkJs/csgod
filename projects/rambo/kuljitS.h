// 3350 Project Rambo - kuljitS.h
// author: Kuljit Singh
// last modified: Feb 28, 2018


#ifndef KULJITS_H
#define KULJITS_H
void kuljitS_physics();
void kuljitS_render();
void printKuljitS(int, int, int, int);
double printRamboCenter();
double printRamborCenterOpt();
void checkMouseMainMenu(XEvent*);
void checkKeysMainMenu();
void renderMainMenu();
void checkKeysPauseMenu();
void renderPauseMenu();
int acceptGameState(int);
#endif
