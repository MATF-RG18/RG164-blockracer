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

#include "image.h"
#define FILENAME0 "backdrop.bmp"
GLuint names[2];

// GLUT callback funkcije
void on_display(void);
void on_reshape(int width, int height);
void on_keyboard(unsigned char key, int x, int y);
void on_keyboard_release(unsigned char key, int x, int y);
void on_arrow_input(int key, int x, int y);
void on_arrow_input_release(int key, int x, int y);
void on_timer(int value);

// Pomocne funkcije
void draw_backdrop();
void draw_bitmap_string(float x, float y, float z, void *font, std::string string);
void set_ortho_projection();
void restore_perspective_projection();
void draw_stats();
void draw_theend();

// Dimenzije prozora
int window_width, window_height;
// Promenljive za cuvanje poena i zivota
int points = 0;
int lives = 5;
// Flag koji oznacava
bool game_ongoing = true;
// Promenljive za brzine blokova i vozila
float Block::speed = 5.0;
float Vehicle::speed = 8.0;

// Promenljive za cuvanje pocetnog vremena i vremena proteklog od poslednjeg iscrtavanja prozora
std::chrono::duration<double> start_time = std::chrono::system_clock::now().time_since_epoch();
std::chrono::duration<double> last_updated = std::chrono::system_clock::now().time_since_epoch();

// Objekti za vozilo, mrezu, blokove i generator blokova
Vehicle yugo = Vehicle(0, 0);
Grid mreza = Grid(100, 100);
std::list<Block*> blokovi;
Generator generator;

// Promenljive za cuvanje stanja tastera za kretanje vozila
bool left_key_pressed = false;
bool right_key_pressed = false;


int main(int argc, char** argv)
{
    // Inicijalizuje se GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    // Kreira se prozor
    glutInitWindowSize(768, 432);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("BlockRacer");

    // Registruju se callback funkcije (za obradu dogadjaja)
    glutKeyboardFunc(on_keyboard);
    glutKeyboardUpFunc(on_keyboard_release);
    glutSpecialFunc(on_arrow_input);
    glutSpecialUpFunc(on_arrow_input_release);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    on_timer(0);

    // Obavlja se OpenGL inicijalizacija
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    // Ukljucuju se teksture
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    Image * image;
    image = image_init(1440, 414);

    // Kreira se tekstura za pozadinu
    image_read(image, FILENAME0);

    // Generise se identifikator teksture
    glGenTextures(2, names);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    image_done(image);

    // Program ulazi u glavnu petlju.
    glutMainLoop();
    return 0;
}


void on_display(void)
{
    // Brise se prethodni sadrzaj prozora.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Inicijalizuje se matrica transformacije i podesava se tacka pogleda
    glLoadIdentity();
    gluLookAt(
        0, 3, 2,
        0, 0.5, -5,
        0, 1, 0
    );

    // Postavljamo trenutno vreme i vreme proteklo od poslednjeg iscrtavanja ekrana
    std::chrono::duration<double> current = std::chrono::system_clock::now().time_since_epoch();
    std::chrono::duration<double> elapsed = current - last_updated;

    // Menjamo brzinu blokova u zavisnosti od proteklog vremena od pokretanja igre
    if (std::chrono::system_clock::now().time_since_epoch() - start_time > std::chrono::seconds(20))
        Block::setSpeed(7.0);
    if (std::chrono::system_clock::now().time_since_epoch() - start_time > std::chrono::seconds(30))
        Block::setSpeed(10.0);
    if (std::chrono::system_clock::now().time_since_epoch() - start_time > std::chrono::seconds(40))
        Block::setSpeed(15.0);
    if (std::chrono::system_clock::now().time_since_epoch() - start_time > std::chrono::seconds(60))
        Block::setSpeed(20.0);

    // Iscrtavamo mrezu
    mreza.draw();
    // Iscrtavamo pozadinu
    draw_backdrop();

    // Ako je igra i dalje u toku, vrsimo sve potrebne operacije
    if (game_ongoing) {
        // Proveravamo da li je pritisnut neki od tastera za kretanje vozila
        // i ako jeste, pomeramo vozilo
        if (left_key_pressed == true)
            yugo.move(Left_direction, elapsed.count());
        if (right_key_pressed == true)
            yugo.move(Right_direction, elapsed.count());

        // Iscrtavamo vozilo
        yugo.draw();

        // Generisemo blokove
        generator.generate(blokovi);

        // Detekcija kolizije blokova i vozila
        for (auto it = blokovi.begin(); it != blokovi.end(); ) {
            bool collisionX = (*it)->getX() + 1 >= yugo.getX() - 0.5 &&
                              yugo.getX() + 0.5 >= (*it)->getX() - 1;
            bool collisionY = (*it)->getY() + 1 >= yugo.getY() &&
                              yugo.getY() + 1 >= (*it)->getY();

            // Ako se blok sudari sa vozilom,
            if ((collisionX && collisionY)) {
                it = blokovi.erase(it);
                points++;
            }
            // Ako blok prodje iza vozila, smanjujemo broj "zivota"
            if ((*it)->getY() <= 0) {
                it = blokovi.erase(it);
                lives--;
            }
            ++it;
        }

        for (Block* blok : blokovi) {
            blok->move(elapsed.count());
        }

        for (Block* blok : blokovi) {
            blok->draw();
        }

        // Iscrtavamo broj poena i zivota
        draw_stats();
    }
    // Ako je igra zavrsena tj. broj preostalih "zivota" je 0, ispisujemo KRAJ i broj osvojenih poena
    else {
        draw_theend();
    }

    if (lives <= 0)
        game_ongoing = false;

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
    gluPerspective(80, window_width / (float) window_height, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}


void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        glDeleteTextures(2, names);
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


void draw_backdrop() {
    glPushMatrix();
    glTranslatef(-75, 0, -45);
    glScalef(10, 10, 1);
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);

    glTexCoord2f(1, 0);
    glVertex3f(14.4, 0, 0);

    glTexCoord2f(1, 1);
    glVertex3f(14.4, 4.14, 0);

    glTexCoord2f(0, 1);
    glVertex3f(0, 4.14, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}


void draw_bitmap_string(float x, float y, float z, void *font, std::string text) {
    glRasterPos3f(x, y, z);
    for (char const &c : text) {
        glutBitmapCharacter(font, (char) c);
    }
}

void set_ortho_projection() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, window_width, window_height, 0);
    glMatrixMode(GL_MODELVIEW);
}


void restore_perspective_projection() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


void draw_stats() {
    set_ortho_projection();
    std::string s1 = "Poeni: " + std::to_string(points);
    std::string s2 = "Zivoti: " + std::to_string(lives);

    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    draw_bitmap_string(5, 15, 0, GLUT_BITMAP_HELVETICA_12, s1);
    draw_bitmap_string(5, 30, 0, GLUT_BITMAP_HELVETICA_12, s2);
    glPopMatrix();

    restore_perspective_projection();
}


void draw_theend() {
    set_ortho_projection();

    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0, 0, 0);
    draw_bitmap_string(window_width / 2 - 25, window_height / 2, 0, GLUT_BITMAP_HELVETICA_18, "KRAJ");
    draw_bitmap_string(window_width / 2 - 25, window_height / 2 + 15, 0, GLUT_BITMAP_HELVETICA_12, "Poeni: " + std::to_string(points));
    glPopMatrix();

    restore_perspective_projection();
}
