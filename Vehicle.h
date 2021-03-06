#define GL_SILENCE_DEPRECATION (1)

#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include <GL/glut.h>

enum Direction { Left_direction, Right_direction };

class Vehicle {
private:
    float x;
    float y;
    static float speed;

public:
    Vehicle(int pos_x = 0, int pos_y = 0);
    float getX();
    float getY();
    void draw();
    void move(Direction d, float delta_time);
};

#endif