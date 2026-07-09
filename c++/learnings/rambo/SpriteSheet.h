#ifndef SPRITESHEET_H
#define SPRITESHEET_H

class SpriteSheet{
    public:
        int width;
        int height;
        int rows;
        int columns;
        unsigned char *data;

        ~SpriteSheet();
        SpriteSheet(const char *, int, int);

};

#endif
