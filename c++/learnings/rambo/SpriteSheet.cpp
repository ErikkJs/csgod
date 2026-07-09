#include "SpriteSheet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

SpriteSheet::~SpriteSheet() { delete [] data; }
SpriteSheet::SpriteSheet(const char *fname, int r, int c) {
    if (fname[0] == '\0')
        return;
    
    rows = r;
    columns = c;
    //CONVERT GIF TO PPM
    char name[40];
    strcpy(name, fname);
    int slen = strlen(name);
    name[slen-4] = '\0';
    char ppmname[80];
    sprintf(ppmname,"%s.ppm", name);
    char ts[100];
    sprintf(ts, "convert %s %s", fname, ppmname); //command that converts
    system(ts);

    FILE *fpi = fopen(ppmname, "r");
    if (fpi) {
        char line[200];
        fgets(line, 200, fpi);
        fgets(line, 200, fpi);
        while (line[0] == '#')
            fgets(line, 200, fpi);
        sscanf(line, "%i %i", &width, &height);
        fgets(line, 200, fpi);

        //get pixel data
        int n = width * height * 3;
        data = new unsigned char[n];
        for (int i=0; i<n; i++)
            data[i] = fgetc(fpi);
        fclose(fpi);
    } else {
        printf("ERROR opening image: %s\n",ppmname);
        exit(0);
    }
    unlink(ppmname);
}

