#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "MathHelper.h"		// NEW!
#include "shapes.h"
#include "shader.h"



#define NUM_VERTICES num_vertices
#define NUM_INDICES num_indices	

// From http://www.opengl.org/registry/specs/EXT/pixel_buffer_object.txt 
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GLuint shaderProgramID;
GLuint vao = 0;
GLuint vbo;
GLuint positionID, colorID;
GLuint indexBufferID; 

GLuint	perspectiveMatrixID, viewMatrixID, modelMatrixID;	// IDs of variables mP, mV and mM in the shader
	
GLfloat* transMatrix;	
GLfloat* tempMatrix1;	
GLfloat* M;				

GLfloat* V;				
GLfloat* P;				

float horizontal = 0.0f;
float vertical = 0.0f;
float thetaY = 0.00f;
float thetaX = 0.00f;
float scaleAmount = 1.0f;

void initMatrices() {

	scaleAmount = 1.0f;

	// Allocate memory for the matrices and initialize them to the Identity matrix
	transMatrix = new GLfloat[16];	MathHelper::makeIdentity(transMatrix);
	tempMatrix1 = new GLfloat[16];	MathHelper::makeIdentity(tempMatrix1);
	M = new GLfloat[16];			MathHelper::makeIdentity(M);
	V = new GLfloat[16];			MathHelper::makeIdentity(V);
	P = new GLfloat[16];			MathHelper::makeIdentity(P);

	MathHelper::makePerspectiveMatrix(P, 60.0f, 1.0f, 1.0f, 1000.0f);
}

void changeViewport(int w, int h){
	glViewport(0, 0, w, h);
}

// Here is the function that gets called each time the window needs to be redrawn.
// It is the "paint" method for our program, and is set up from the glutDisplayFunc in main
void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderProgramID);

	// Fill the matrices with valid data	
	MathHelper::makeTranslate(transMatrix, horizontal, vertical, -2.0f);

	/*multiply the matricies*/	
	MathHelper::copyMatrix (transMatrix, M);
		
	glUniformMatrix4fv(modelMatrixID, 1, GL_TRUE, M);
	glUniformMatrix4fv(viewMatrixID, 1, GL_TRUE, V);
	glUniformMatrix4fv(perspectiveMatrixID, 1, GL_TRUE, P);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	glDrawElements (GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, NULL);
	glutSwapBuffers();
	glutPostRedisplay();
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
	case 'w':
		vertical += 0.1;
		break;
	case 's':
		vertical -= 0.1;
		break;
	case 'q':
		scaleAmount += 0.1;
		break;
	case 033:
	    exit( EXIT_SUCCESS );
	    break;
    }
}


int main (int argc, char** argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Index Buffers");
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glewInit();

	/*set up the matricies*/
	initMatrices(); 

	// Make a shader
	shaderProgramID = createShaders();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Create the buffer, but don't load anything yet
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareColors)+sizeof(squareVertices), NULL, GL_STATIC_DRAW);
	// Load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(squareVertices), squareVertices); 
	// Load the colors right after that
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(squareVertices),sizeof(squareColors), squareColors);

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_INDICES*sizeof(GLuint), squareIndices, GL_STATIC_DRAW);
	
	positionID = glGetAttribLocation(shaderProgramID, "s_vPosition");
	colorID = glGetAttribLocation(shaderProgramID, "s_vColor");
	
	perspectiveMatrixID = glGetUniformLocation(shaderProgramID, "mP");
	viewMatrixID = glGetUniformLocation(shaderProgramID, "mV");
	modelMatrixID = glGetUniformLocation(shaderProgramID, "mM");

	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(squareVertices)));
	
	glutKeyboardFunc(keyboard);

	glUseProgram(shaderProgramID);
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	
	glutMainLoop();
	
	return 0;
}

