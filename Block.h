#define GL_SILENCE_DEPRECATION (1)

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <GL/glut.h>
#include <iostream>
#include <random>

class Block {
private:
    // Koordinate bloka
    float x;
    float y;
    // Tri moguce x-koordinate novogenerisanih blokova
    const float column_x[3] = {-2.5, 0, 2.5};
    // Brzina kretanja
    static float speed;
    // Pomocni metod za generisanje nasumicnog indeksa
    // za biranje x-kordinate pri instanciranju novog bloka
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