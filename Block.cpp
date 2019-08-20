#include "Block.h"

Block::Block(int pos_y) : y(pos_y) {
    x = column_x[generate_random_index()];
};
Block::Block(int pos_x, int pos_y) : x(pos_x), y(pos_y) {};

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

float Block::getSpeed() {
    return speed;
}

void Block::setSpeed(float spd) {
    speed = spd;
}

// Metod za pomeranje vozila u levo ili desno
void Block::move(float delta_time) {
    y -= speed * delta_time;
    // if y < -1 delete object or move it
    //std::cout << "Z is: " << getY() << std::endl;
}

int Block::generate_random_index() {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_int_distribution<int> uni(0, 2);

    return (int) uni(rng);
}