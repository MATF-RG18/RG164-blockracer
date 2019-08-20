// Puno OpenGL funkcija je deprecated u macOS 10.14
// Ovaj define uklanja upozorenja za te funkcije
#define GL_SILENCE_DEPRECATION (1)
#define REFRESH_RATE (1000/60)

#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <list>
#include <chrono>

#include "Grid.h"
#include "Vehicle.h"
#include "Block.h"
#include "Generator.h"

void on_display(void);
void on_reshape(int width, int height);
void on_keyboard(unsigned char key, int x, int y);
void on_keyboard_release(unsigned char key, int x, int y);
void on_arrow_input(int key, int x, int y);
void on_arrow_input_release(int key, int x, int y);
void on_timer(int value);

// Dimenzije prozora
int window_width, window_height;
float Block::speed = 5.0;
float Vehicle::speed = 8.0;
std::chrono::duration<double> last_updated = std::chrono::system_clock::now().time_since_epoch();
std::chrono::duration<double> start_time = std::chrono::system_clock::now().time_since_epoch();


Vehicle yugo = Vehicle(0, 0);
Grid mreza = Grid(100, 100);
std::list<Block*> blokovi;
Generator generator;
Block blk = Block(20);
Block blk2 = Block(18);

bool left_key_pressed = false;
bool right_key_pressed = false;


int main(int argc, char** argv)
{
    // Inicijalizuje se GLUT.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    // Kreira se prozor.
    glutInitWindowSize(768, 432);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("BlockRacer");

    // Registruju se callback funkcije (za obradu dogadjaja).
    glutKeyboardFunc(on_keyboard);
    glutKeyboardUpFunc(on_keyboard_release);
    glutSpecialFunc(on_arrow_input);
    glutSpecialUpFunc(on_arrow_input_release);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    on_timer(0);

    // Obavlja se OpenGL inicijalizacija.
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    //glShadeModel(GL_SMOOTH);
    //glLineWidth(2);

    // Program ulazi u glavnu petlju.
    glutMainLoop();
    return 0;
}


void on_display(void)
{
    // Brise se prethodni sadrzaj prozora.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Inicijalizuje se matrica transformacije i podesava se tacka pogleda
    // glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0, 3, 2,
        0, 0.5, -5,
        0, 1, 0
    );

    std::chrono::duration<double> current = std::chrono::system_clock::now().time_since_epoch();
    std::chrono::duration<double> elapsed = current - last_updated;

    if (std::chrono::system_clock::now().time_since_epoch() - start_time > std::chrono::seconds(20))
        Block::setSpeed(7.0);
    if (std::chrono::system_clock::now().time_since_epoch() - start_time > std::chrono::seconds(30))
        Block::setSpeed(10.0);
    if (std::chrono::system_clock::now().time_since_epoch() - start_time > std::chrono::seconds(40))
        Block::setSpeed(15.0);
    if (std::chrono::system_clock::now().time_since_epoch() - start_time > std::chrono::seconds(60))
        Block::setSpeed(20.0);

    mreza.draw();

    if (left_key_pressed == true)
        yugo.move(Left_direction, elapsed.count());
    if (right_key_pressed == true)
        yugo.move(Right_direction, elapsed.count());
    yugo.draw();

    generator.generate(blokovi);

    for (auto it = blokovi.begin(); it != blokovi.end(); ) {
        bool collisionX = (*it)->getX() + 1 >= yugo.getX() - 0.5 &&
                          yugo.getX() + 0.5 >= (*it)->getX() - 1;
        bool collisionY = (*it)->getY() + 1 >= yugo.getY() &&
                          yugo.getY() + 1 >= (*it)->getY();

        if (collisionX && collisionY || (*it)->getY() <= 0)
            it = blokovi.erase(it);
        else
            ++it;
    }

    for (Block* blok : blokovi) {
        blok->move(elapsed.count());
    }

    for (Block* blok : blokovi) {
        blok->draw();
    }

    // blk.move(elapsed.count());
    // blk2.move(elapsed.count());
    // blk.draw();
    // blk2.draw();

    last_updated = current;
    glutSwapBuffers();
}


void on_reshape(int width, int height)
{
    window_width = width;
    window_height = height;
    // Podesava se viewport.
    glViewport(0, 0, window_width, window_height);

    // Podesava se projekcija.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, window_width / (float) window_height, 1, 30);
    //glFrustum(-0.1, 0.1, -float(window_height)/(10.0*float(window_width)), float(window_height)/(10.0*float(window_width)), 0.2, 9999999.0);
    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
}

void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'a':
    case 'A':
        left_key_pressed = true;
        break;
    case 'd':
    case 'D':
        right_key_pressed = true;
        break;
    }
}

void on_keyboard_release(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'a':
    case 'A':
        left_key_pressed = false;
        break;
    case 'd':
    case 'D':
        right_key_pressed = false;
        break;
    }
}

void on_arrow_input(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        left_key_pressed = true;
        break;
    case GLUT_KEY_RIGHT:
        right_key_pressed = true;
        break;
    }
}

void on_arrow_input_release(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        left_key_pressed = false;
        break;
    case GLUT_KEY_RIGHT:
        right_key_pressed = false;
        break;
    }
}

void on_timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(REFRESH_RATE, on_timer, 0);
}
