#include "Grid.h"

Grid::Grid(int rep_x, int rep_y) : repeat_x(rep_x), repeat_y(rep_y) {};

void Grid::draw() {
    // Postavljamo debljinu linija
    glLineWidth(1);
    // Postavljamo boju linija
    glColor3f(135.0 / 255.0, 90.0 / 255.0, 149.0 / 255.0);
    // Iskljucujemo sencenje
    glDisable(GL_LIGHTING);

    glPushMatrix();
    // Transliramo mrezu da bi centar mreze bio u centru koordinatnog sistema
    glTranslatef(-repeat_x / 2.0, 0, -repeat_y / 2.0);

    int i = 0;
    // Crtamo linije paralelne sa x-osom
    for (i = 0; i <= repeat_x; i++) {
        // Postavljamo opciju za crtanje linija
        glBegin(GL_LINES);
        // Crtamo pocetnu tacku
        glVertex3f(0, 0, i);
        // Crtamo krajnju tacku
        glVertex3f(repeat_x, 0, i);
        glEnd();
    }

    // Crtamo linije paralelne sa z-osom
    for (i = 0; i <= repeat_y; i++) {
        // Postavljamo opciju za crtanje linija
        glBegin(GL_LINES);
        // Crtamo pocetnu tacku
        glVertex3f(i, 0, 0);
        // Crtamo krajnju tacku
        glVertex3f(i, 0, repeat_y);
        glEnd();
    }
    glPopMatrix();
}