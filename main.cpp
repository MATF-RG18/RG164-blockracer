// Puno OpenGL funkcija je deprected u macOS 10.14
// Ovaj define uklanja upozorenja za te funkcije
#define GL_SILENCE_DEPRECATION (1)

#include <iostream>
#include <GL/glut.h>


void on_display(void);
void on_reshape(int width, int height);
void on_keyboard(unsigned char key, int x, int y);

/* Dimenzije prozora */
int window_width, window_height;

using namespace std;

int main(int argc, char** argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    // GLUT_DEPTH flag poshtuje "dubinu", a ne redosled iscrtavanja
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    /* Kreira se prozor. */
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("BlockRacer");

    /* Registruju se callback funkcije (za obradu dogadjaja). */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);




    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0.75, 0.75, 0.75, 0);
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
        1, 50);

    /* Podesava se tacka pogleda. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0, 3, 4,
        0, 0.5, 0,
        0, 1, 0
    );


    int rep_x = 100;
    int rep_z = 100;
    // Postavljamo debljinu linija
    glLineWidth(1);
    // Postavljamo boju linija
    glColor3f(1.0, 0.4, 0.4);
    // Iskljucujemo sencenje
    glDisable(GL_LIGHTING);

    glPushMatrix();
    // Transliramo mrezu da bi centar mreze bio u centru koord sistema
    glTranslatef(-rep_x / 2.0, 0, -rep_z / 2.0);

    int i = 0;
    // Crtamo linije paralelne sa x-osom
    for (i = 0; i <= rep_x; i++) {
        // Postavljamo opciju za crtanje linija
        glBegin(GL_LINES);
        // Crtamo pocetnu tacku
        glVertex3f(0, 0, i);
        // Crtamo krajnju tacku
        glVertex3f(rep_x, 0, i);
        glEnd();
    }

    // Crtamo linije paralelne sa z-osom
    for (i = 0; i <= rep_z; i++) {
        // Postavljamo opciju za crtanje linija
        glBegin(GL_LINES);
        // Crtamo pocetnu tacku
        glVertex3f(i, 0, 0);
        // Crtamo krajnju tacku
        glVertex3f(i, 0, rep_z);
        glEnd();
    }
    glPopMatrix();
    /*
     * Kreira se kocka i primenjuje se geometrijska transformacija na
     * istu.
     */
    glColor3f(0, 0, 1);
    glTranslatef(0, 1, 0);
    glScalef(1, 2, 1);
    glutSolidCube(1);
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
    }
}

