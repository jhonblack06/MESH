#include <GL/glut.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <GL/glut.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include "MHalfEdge.h"

using namespace std;
char path[] = "C:/Users/USUARIO/source/repos/Mesh/Mesh/myObjs/human.obj";
HalfEdgeM<ToolBox> miMalla;
float resize = 3;
float rY = 0, rX = 0;
template <class G>
void Face<G>::draw() { //graficamos las caras
	glMatrixMode(GL_MODELVIEW);
	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.5f, 0.0f);
	float a = edge->tail->x, b = edge->tail->y, c = edge->tail->z;
	glVertex3d(a/resize, b/resize, c/resize);
	a = edge->next->tail->x, b = edge->next->tail->y, c = edge->next->tail->z;
	glVertex3d(a/resize, b / resize, c / resize);
	a = edge->next->next->tail->x, b = edge->next->next->tail->y, c = edge->next->next->tail->z;
	glVertex3d(a / resize, b / resize, c / resize);
	
	glEnd();
}
template <class G>
void Face<G>::drawEdge() { //graficamos aristas
	glMatrixMode(GL_MODELVIEW);
	glBegin(GL_LINES);
	glColor3f(0.5f, 1.0f, 0.5f);
	float a = edge->tail->x, b = edge->tail->y, c = edge->tail->z;
	glVertex3d(a / resize, b / resize, c / resize);
	a = edge->head->x, b = edge->head->y, c = edge->head->z;
	glVertex3d(a / resize, b / resize, c / resize);
	a = edge->next->tail->x, b = edge->next->tail->y, c = edge->next->tail->z;
	glVertex3d(a / resize, b / resize, c / resize);
	a = edge->next->head->x, b = edge->next->head->y, c = edge->next->head->z;
	glVertex3d(a / resize, b / resize, c / resize);
	a = edge->next->next->tail->x, b = edge->next->next->tail->y, c = edge->next->next->tail->z;
	glVertex3d(a / resize, b / resize, c / resize);
	a = edge->next->next->head->x, b = edge->next->next->head->y, c = edge->next->next->head->z;
	glVertex3d(a / resize, b / resize, c / resize);
	glEnd();
}
template <class G>
void Face<G>::drawAdy() {
	glMatrixMode(GL_MODELVIEW);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	float a = edge->tail->x, b = edge->tail->y, c = edge->tail->z;
	glVertex3d(a / resize, b / resize, c / resize);
	a = edge->next->tail->x, b = edge->next->tail->y, c = edge->next->tail->z;
	glVertex3d(a / resize, b / resize, c / resize);
	a = edge->next->next->tail->x, b = edge->next->next->tail->y, c = edge->next->next->tail->z;
	glVertex3d(a / resize, b / resize, c / resize);

	glEnd();

}
template <class G>
void Vertex<G>::vertexdraw() {
	edge->face->drawAdy();
	//edge->twin->face->drawAdy();
}


void idle() { // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}
void OnMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}

}
void OnMouseMotion(int x, int y) {

}
void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		rY += 10; break;
	case GLUT_KEY_LEFT:
		rY -= 10; break;
	case GLUT_KEY_UP:
		rX += 10; break;
	case GLUT_KEY_DOWN:
		rX -= 10; break;
	}
}
void init_GL(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}
GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //tecla esc
		exit(0);
		break;

	default:
		break;
	}
}
void glPaint() {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glRotatef(rX, 1.0, 0.0, 0.0);
	glRotatef(rY, 0.0, 1.0, 0.0);

	glOrtho(-15.0f, 15.0f, -15.0f, 15.0f, -15.0f, 15.0f);
	miMalla.print();
	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}



int main(int argc, char** argv)
{
	miMalla.loadOBJ(path);
    //miMalla.print();
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(800, 600);   // Set the window's initial width & height
	glutInitWindowPosition(10, 10); // Position the window's initial top-left corner
	glutCreateWindow("Mesh");          // Create window with the given title
	glutKeyboardFunc(&window_key); //salir del programa
	glutSpecialFunc(&specialKeys);
	//glutMouseFunc(&OnMouseClick);
	//glutPassiveMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);
	glutDisplayFunc(glPaint);                      // Our own OpenGL initialization
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}
