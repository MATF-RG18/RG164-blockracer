#include "Vehicle.h"

Vehicle::Vehicle(int pos_x, int pos_y) : x(pos_x), y(pos_y) {};

// Metod za iscrtavanje vozila
void Vehicle::draw() {
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(x, 0.5, y);
    glScalef(1, 1, 1);
    glutSolidCube(1);
    glPopMatrix();
};

// Getter za koordinatu x
float Vehicle::getX() {
    return x;
}

// Getter za koordinatu y
float Vehicle::getY() {
    return y;
}

// Metod za pomeranje vozila u levo ili desno
void Vehicle::move(Direction d, float delta) {
    if (d == Right_direction) 
        x += delta;
    else 
        x -= delta;
    //std::cout << "X is: " << getX() << std::endl;
}