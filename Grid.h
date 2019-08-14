#define GL_SILENCE_DEPRECATION (1)

#ifndef _GRID_H_
#define _GRID_H_

#include <GL/glut.h>
#include <iostream>


class Grid {
private:
    int repeat_x;
    int repeat_y;

public:
    Grid(int repeat_x = 100, int repeat_y = 100);
    void draw();
};

#endif