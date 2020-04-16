#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

// wp�rz�dne po�o�enia obserwatora
GLdouble eyex = 0;
GLdouble eyey = 0;
GLdouble eyez = 3;

// wsp�rz�dne punktu w kt�rego kierunku jest zwr�cony obserwator,
GLdouble centerx = 0;
GLdouble centery = 0;
GLdouble centerz = 0;

struct Edge {
    GLfloat xpos, ypos, zpos;
};

class Figure {
public:
    int edgeCount = 0;
    int shiftCount = 0;
    Edge* edges;
    Edge* shifts;
    Figure() {
        edges = new Edge[0];
    }
    Figure(string file) {
        ifstream fin(file);
        if (fin.good()) {
            fin >> edgeCount;
            edges = new Edge[2 * edgeCount];
            for (int i = 0; i < 2 * edgeCount; i++) {
                fin >> edges[i].xpos >> edges[i].ypos >> edges[i].zpos;
            }
            fin >> shiftCount;
            shifts = new Edge[shiftCount];
            for (int i = 0; i < shiftCount; i++) {
                fin >> shifts[i].xpos >> shifts[i].ypos >> shifts[i].zpos;
            }
        }
        else {
            cerr << "Blad otwarcia pliku" << endl;
        }
    }

    void draw(float r, float g, float b) {
        glBegin(GL_LINES);
        glColor3f(r, g, b);
        for (int j = 0; j < 2 * edgeCount; j++) {
            glVertex3f(edges[j].xpos, edges[j].ypos, edges[j].zpos);
        }
        glEnd();
    }

    ~Figure() {
        delete[] edges;
        delete[] shifts;
    }
};
Figure shape("edges.txt");
// funkcja generuj�ca scen� 3D
void Display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ustawienie obserwatora
    gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0);
    glPushMatrix();
    for (int i = shape.shiftCount - 1; i >= 0; i--) {
        for (float j = 0.0; j <= 1.0; j += 0.02) {
            shape.draw(j, 0, 0);
            glTranslatef(shape.shifts[i].xpos * j, shape.shifts[i].ypos * j, shape.shifts[i].zpos * j);
        }
    }
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

// zmiana wielko�ci okna

void Reshape(int width, int height)
{
    // obszar renderingu - ca�e okno
    glViewport(0, 0, width, height);

    // wyb�r macierzy rzutowania
    glMatrixMode(GL_PROJECTION);

    // macierz rzutowania = macierz jednostkowa
    glLoadIdentity();

    // wysoko�� okna wi�ksza od wysoko�ci okna
    if (width < height && width > 0)
        glFrustum(-5.0, 5.0, -5.0 * height / width, 5.0 * height / width, 1.0, 1000.0);
    else
        if (width >= height && height > 0)
            glFrustum(-5.0 * width / height, 5.0 * width / height, -5.0, 5.0, 1.0, 1000.0);


    // generowanie sceny 3D
    Display();
}

// obs�uga klawiatury

void Keyboard(unsigned char key, int x, int y)
{
    // klawisz +
    if (key == '+')
        eyez -= 0.1;
    else

        // klawisz -
        if (key == '-')
            eyez += 0.1;

    // odrysowanie okna
    Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obs�uga klawiszy funkcyjnych i klawiszy kursora

void SpecialKeys(int key, int x, int y)
{
    switch (key)
    {
        // kursor w lewo
    case GLUT_KEY_LEFT:
        eyex += 0.1;
        break;

        // kursor w g�r�
    case GLUT_KEY_UP:
        eyey -= 0.1;
        break;

        // kursor w prawo
    case GLUT_KEY_RIGHT:
        eyex -= 0.1;
        break;

        // kursor w d�
    case GLUT_KEY_DOWN:
        eyey += 0.1;
        break;
    }

    // odrysowanie okna
    Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}


int main(int argc, char* argv[])
{
    // inicjalizacja biblioteki GLUT
    glutInit(&argc, argv);

    // inicjalizacja bufora ramki
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // rozmiary g��wnego okna programu
    glutInitWindowSize(800, 800);

    glutCreateWindow("Modelowanie");


    // do��czenie funkcji generuj�cej scen� 3D
    glutDisplayFunc(Display);

    // do��czenie funkcji wywo�ywanej przy zmianie rozmiaru okna
    glutReshapeFunc(Reshape);

    // do��czenie funkcji obs�ugi klawiatury
    glutKeyboardFunc(Keyboard);

    // do��czenie funkcji obs�ugi klawiszy funkcyjnych i klawiszy kursora
    glutSpecialFunc(SpecialKeys);


    // wprowadzenie programu do obs�ugi p�tli komunikat�w
    glutMainLoop();
    return 0;
}