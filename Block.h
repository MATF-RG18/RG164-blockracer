#define GL_SILENCE_DEPRECATION (1)

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <GL/glut.h>
#include <iostream>

class Block {
private:
    float x;
    float y;
    float distance_travelled;

public:
    Block(int pos_x = 0, int pos_y = 10, float dist = 0);
    float getX();
    float getY();
    void draw();
    void move(float delta);
};

#endif