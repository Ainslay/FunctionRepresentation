#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

GLdouble eyeX = 0;
GLdouble eyeY = 4;
GLdouble eyeZ = 4;

GLdouble centerX = 0;
GLdouble centerY = 0;
GLdouble centerZ = 0;

struct Vertex
{
	GLdouble X, Y, Z;
};

struct Edge
{
	Vertex A, B;
};

class Block
{
public:
	Edge* edges = nullptr;
	Vertex* shifts = nullptr;
	
	int edgeCount = 0;
	int shiftCount = 0;

	Block() = delete;

	Block(const string& inputDataFilePath)
	{
		LoadData(inputDataFilePath);
	}

	~Block()
	{
		delete[] edges;
		delete[] shifts;
	}

	void Draw()
	{
		glBegin(GL_LINES);

		for (int i = 0; i < edgeCount; i++)
		{
			glVertex3d(edges[i].A.X, edges[i].A.Y, edges[i].A.Z);
			glVertex3d(edges[i].B.X, edges[i].B.Y, edges[i].B.Z);
		}
		glEnd();
	}

	void Shift()
	{
		double colors[6][3] =
		{
			{1.0,0.0,0.0},
			{1.0,0.5,0.0},
			{1.0,1.0,0.0},
			{0.0,0.6,0.1},
			{0.0,0.0,1.0},
			{0.7,0.0,1.0}
		};

		for (int i = 0; i < shiftCount; i++)
		{
			for (double j = 0.0; j <= 1.0; j += 0.01)
			{
				glColor3dv(colors[i % 6]);
				Draw();
				glTranslated(shifts[i].X * j, shifts[i].Y * j, shifts[i].Z * j);
			}
		}
	}

private:
	void LoadData(const string& inputDataFilePath)
	{
		fstream inputData(inputDataFilePath);

		if (!inputData.good())
		{
			cerr << "Could not load data. Path is incorrect or file does not exist." << endl;
			exit(-1);
		}
		
		inputData >> edgeCount;
		edges = new Edge[edgeCount];

		for (int i = 0; i < edgeCount; i++)
		{
			inputData >> edges[i].A.X >> edges[i].A.Y >> edges[i].A.Z;
			inputData >> edges[i].B.X >> edges[i].B.Y >> edges[i].B.Z;
		}

		inputData >> shiftCount;
		shifts = new Vertex[shiftCount];

		for (int i = 0; i < shiftCount; i++)
		{
			inputData >> shifts[i].X >> shifts[i].Y >> shifts[i].Z;
		}

		inputData.close();
	}
};

void ChangeViewPort(int width, int height)
{
	const float aspectRatio = (float)width / (float)height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (width < height && width > 0)
	{
		glFrustum(-5.0, 5.0, -5.0 * height / width, 5.0 * height / width, 1.0, 20.0);
	}
	else if (width >= height && height > 0)
	{
		glFrustum(-5.0 * width / height, 5.0 * width / height, -5.0, 5.0, 1.0, 20.0);
	}
}

Block square("data.txt");

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(2.5);
	glColor3d(1.0, 0.2, 0.0);

	glShadeModel(GL_FLAT);

	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);

	square.Shift();

	glFlush();
	glutSwapBuffers();
}

void Key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+': eyeZ -= 0.1; break;
	case '-': eyeZ += 0.1; break;
	default: break;
	}
	ChangeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void SpecialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:  eyeX += 0.1; break;
	case GLUT_KEY_RIGHT: eyeX -= 0.1; break;
	case GLUT_KEY_UP:    eyeY -= 0.1; break;
	case GLUT_KEY_DOWN:  eyeY += 0.1; break;
	default: break;
	}
	ChangeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void Idle()
{
	glutPostRedisplay();
}

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(300, 150);

	glutCreateWindow("Shift Representation - Jakub Spalek Lab1 Gr1 INF II");

	glutReshapeFunc(ChangeViewPort);
	glutDisplayFunc(Render);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKey);

	glClearColor(0, 0, 0, 0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);


	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	cout << "Poruszanie kamera: klawisze strzalek" << endl;
	cout << "Zoom: [-]/[+]\n" << endl;

	
	glutMainLoop();

	return 0;
}

// Zawartoœæ pliku data.txt
//4
//- 2 - 2 1   2 - 2 1
//2 - 2 1   2  2 1
//2  2 1 - 2  2 1
//- 2  2 1 - 2 - 2 1
//5
//0 0 - 0.1
//0.1 0 0
//0 0 - 0.1
//- 0.1 0 0
//0 0 - 0.1