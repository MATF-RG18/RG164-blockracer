// Puno OpenGL funkcija je deprected u macOS 10.14
// Ovaj define uklanja upozorenja za te funkcije
#define GL_SILENCE_DEPRECATION (1)
#define REFRESH_RATE (1000/60)

#include <iostream>
#include <GL/glut.h>
#include <vector>

#include "Grid.h"
#include "Vehicle.h"
#include "Block.h"

void on_display(void);
void on_reshape(int width, int height);
void on_keyboard(unsigned char key, int x, int y);
void on_keyboard_release(unsigned char key, int x, int y);
void on_arrow_input(int key, int x, int y);
void on_arrow_input_release(int key, int x, int y);
void on_timer(int value);

// Dimenzije prozora
int window_width, window_height;
int animation_parameter = 0;

Vehicle yugo = Vehicle(0, 0);
Grid mreza = Grid(100, 100);
std::vector<Block> blokovi[20];
Block blk = Block(0, 50, 0);

bool left = false;
bool right = false;


int main(int argc, char** argv)
{
    // Inicijalizuje se GLUT.
    glutInit(&argc, argv);
    // GLUT_DEPTH flag poshtuje "dubinu", a ne redosled iscrtavanja
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
    //
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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0, 3, 2,
        0, 0.5, -5,
        0, 1, 0
    );

    mreza.draw();
    yugo.draw();
    if (left == true)
        yugo.move(Left_direction, 0.2);
    if (right == true)
        yugo.move(Right_direction, 0.2);
    blk.draw();

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
    //glMatrixMode(GL_MODELVIEW);
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
        left = true;
        break;
    case 'd':
    case 'D':
        right = true;
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
        left = false;
        break;
    case 'd':
    case 'D':
        right = false;
        break;
    }
}

void on_arrow_input(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        break;
    }
}

void on_arrow_input_release(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        left = false;
        break;
    case GLUT_KEY_RIGHT:
        right = false;
        break;
    }
}

void on_timer(int value) {
    blk.move(0.01 * animation_parameter);
    animation_parameter += 1;
    glutPostRedisplay();

    glutTimerFunc(REFRESH_RATE, on_timer, 0);
}
