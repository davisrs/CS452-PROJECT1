#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "shader.h"
#include "shapes.h"
#include "MathHelper.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GLuint shaderProgramID;
GLuint vao;
GLuint vbo;
GLuint positionID, colorID;

//Matricies for translations
GLfloat* transMatrix;	
GLfloat* tempMatrix1;		
GLfloat* M;				
GLfloat* V;				
GLfloat* P;	

GLuint	perspectiveMatrixID, viewMatrixID, modelMatrixID;

GLfloat horizontal = 0.0f;
GLfloat vertical = -0.0f;

void initMatrices() {

	// Allocate memory for the matrices and initialize them to the Identity matrix
	tempMatrix1 = new GLfloat[16];	MathHelper::makeIdentity(tempMatrix1);
	M = new GLfloat[16];			MathHelper::makeIdentity(M);
	V = new GLfloat[16];			MathHelper::makeIdentity(V);
	P = new GLfloat[16];			MathHelper::makeIdentity(P);

	MathHelper::makePerspectiveMatrix(P, 60.0f, 1.0f, 1.0f, 1000.0f);
}



void render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(shaderProgramID);
	MathHelper::makeTranslate(transMatrix, horizontal, vertical, -2.0f);
	MathHelper::matrixMult4x4(M, transMatrix, transMatrix);

	glUniformMatrix4fv(modelMatrixID, 1, GL_TRUE, M);
	glUniformMatrix4fv(viewMatrixID, 1, GL_TRUE, V);
	glUniformMatrix4fv(perspectiveMatrixID, 1, GL_TRUE, P);

	glDrawArrays(GL_POLYGON, 0, 4);
	glutSwapBuffers();
}
void keyboard( unsigned char key, int x, int y ){
    /*update the variables based on key press*/
    switch( key ) {
	case 'a':
		horizontal -= 0.1;
		break;
	case 'd':
		horizontal += 0.1;
		break;
	case 033:
	    exit( EXIT_SUCCESS );
	    break;
    }
}


int main (int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutCreateWindow("Alex Kelly Lab2");
	glutDisplayFunc(render);
	glewInit();

	fprintf(stderr, "Make shader..\n");
	shaderProgramID = createShaders();
	printf("Made shader\n");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	printf("1\n");
	// Create the buffer, but don't load anything yet
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareColors)*sizeof(squareVertices), NULL, GL_STATIC_DRAW);
	// Load the vertex points
	printf("2\n");
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(squareVertices), squareVertices); 
	// Load the colors right after that
	printf("3\n");
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(squareVertices),sizeof(squareColors), squareColors);
	
	// Find the position of the variables in the shader
	positionID = glGetAttribLocation(shaderProgramID, "s_vPosition");
	colorID = glGetAttribLocation(shaderProgramID, "s_vColor");

	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(3*4*sizeof(GLfloat)));
	
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	
	glutMainLoop();


	glutMainLoop();

	return 0;

}