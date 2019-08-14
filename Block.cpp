#include "Block.h"

Block::Block(int pos_x, int pos_y, float dist) : x(pos_x), y(pos_y), distance_travelled(dist) {};

// Metod za iscrtavanje vozila
void Block::draw() {
    glPushMatrix();
    glColor3f(0.8, 0.5, 0);
    glTranslatef(x, 0.5, -y);
    glScalef(2, 1, 1);
    glutSolidCube(1);
    glPopMatrix();
};

// Getter za koordinatu x
float Block::getX() {
    return x;
}

// Getter za koordinatu y
float Block::getY() {
    return y;
}

// Metod za pomeranje vozila u levo ili desno
void Block::move(float delta) {
        y -= delta;
        distance_travelled += delta;
    //std::cout << "Z is: " << getY() << std::endl;
}