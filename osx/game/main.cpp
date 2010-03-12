#include <stdlib.h>
#include <stdio.h>
#include <GLUT/glut.h>

struct primitive_el {
	float x;
	float y;
	float z;
	struct	primitive_el *next;
};

typedef struct primitive_el primitive;


primitive *head;

void load_primitive(void) {
	FILE *file = fopen("test.raw", "rb");
	int sides;
	 primitive *current;
	head = NULL;
	while(!feof(file)) {
		current = (primitive *)malloc(sizeof(primitive));
		fscanf(file, "%f %f %f", &current->x, &current->y, &current->z);
		current->next = head;
		head = current;
		
	}
	
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	primitive *current;
	current = head;
	glPushMatrix();
	glTranslatef(1.0f,-1.0f, -10.0f);
	glBegin(GL_QUADS);
	while(current) {
		printf("%f %f %f\n", current->x, current->y, current->z);
		glVertex3f(current->x, current->y, current->z);
		current = current->next;
	}
	glEnd();
	glPopMatrix();
	
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char *argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Game");
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	load_primitive();
	glutMainLoop();
	return EXIT_SUCCESS;
}