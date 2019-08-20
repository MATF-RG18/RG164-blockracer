#define GL_SILENCE_DEPRECATION (1)

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <GL/glut.h>
#include <iostream>
#include <random>

class Block {
private:
    float x;
    float y;
    const float column_x[3] = {-2.5, 0, 2.5};
    static float speed;
    static int generate_random_index();

public:
    Block(int pos_y = 10);
    Block(int pos_x, int pos_y);
    float getX();
    float getY();
    static float getSpeed();
    static void setSpeed(float spd);
    void draw();
    void move(float delta_time);
};

#endif