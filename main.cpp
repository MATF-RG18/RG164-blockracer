// Puno OpenGL funkcija je deprected u macOS 10.14
// Ovaj define uklanja upozorenja za te funkcije
#define GL_SILENCE_DEPRECATION (1)

#include <iostream>
#include <GL/glut.h>

#include "Grid.h"
#include "Vehicle.h"

void on_display(void);
void on_reshape(int width, int height);
void on_keyboard(unsigned char key, int x, int y);

/* Dimenzije prozora */
int window_width, window_height;

Vehicle yugo = Vehicle(0, 0);
Grid mreza = Grid(100, 100);

int main(int argc, char** argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    // GLUT_DEPTH flag poshtuje "dubinu", a ne redosled iscrtavanja
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    /* Kreira se prozor. */
    glutInitWindowSize(768, 432);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("BlockRacer");

    /* Registruju se callback funkcije (za obradu dogadjaja). */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);




    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0, 0, 0, 0);
    //
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glLineWidth(2);

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();
    return 0;
}


void on_display(void)
{
    /* Brise se prethodni sadrzaj 'prozora'. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Podesava se viewport. */
    glViewport(0, 0, window_width, window_height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        80,
        window_width / (float)window_height,
        1, 30);

    /* Podesava se tacka pogleda. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0, 3, 2,
        0, 0.5, -5,
        0, 1, 0
    );


    mreza.draw();
    yugo.draw();

    glutSwapBuffers();
}


void on_reshape(int width, int height)
{
    window_width = width;
    window_height = height;
    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-0.1, 0.1, -float(window_height)/(10.0*float(window_width)), float(window_height)/(10.0*float(window_width)), 0.2, 9999999.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'a':
    case 'A':
    case 75:
        yugo.move(Right_direction, 0.1);
        glutPostRedisplay();
        break;
    case 'd':
    case 'D':
    case 77:
        yugo.move(Left_direction, 0.1);
        glutPostRedisplay();
        break;
    }
}

